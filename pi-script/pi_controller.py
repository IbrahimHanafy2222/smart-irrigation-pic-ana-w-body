#!/usr/bin/env python3
"""
pi_controller.py
Raspberry Pi bridge: PIC16F877A <-> Firebase

Firebase schema (from export.json):
  sensors/
    current          float  (amps)
    humidity         int    (%)
    soil_moisture_pct int   (%)
    temperature      float  (°C)

  status/
    gantry_x         int    (plant index)
    mode             str    "AUTOMATIC" | "MANUAL"
    mode_changed_at  int    (ms timestamp)
    pump             str    "ON" | "OFF"
    system_state     str    "NORMAL" | "FAULT"

  commands/
    emergency_stop   bool
    pump/
      state          str    "ON" | "OFF"
      source         str    "app"
      timestamp      int
    gantry_move/
      x              int    (plant index)
      source         str
      timestamp      int

  ai/
    latest_detection/
      class          str    e.g. "Late_Blight"
      confidence     float
      image_url      str
      timestamp      str
    active_protocol/
      status         str    "idle" | "running"
    action_log/{id}/
      class, confidence, action_taken, timestamp, zone_x

  alerts/history/{id}/
    message, severity, timestamp

Required packages:
  pip install pyserial firebase-admin --break-system-packages
"""

import serial
import threading
import time
import subprocess
from datetime import datetime, timezone
from pathlib import Path
from typing import Optional

import firebase_admin
from firebase_admin import credentials, db, storage

# ── Configuration ─────────────────────────────────────────────────────────────
SERIAL_PORT     = "/dev/ttyAMA0"
BAUD_RATE       = 9600
FIREBASE_CRED   = "/home/akai2004/firebase-key.json"
FIREBASE_DB_URL = "https://YOUR-PROJECT-default-rtdb.firebaseio.com"
FIREBASE_BUCKET = "YOUR-PROJECT.appspot.com"
PHOTO_DIR       = Path("/tmp/irrigation_photos")

# ── UART protocol (mirrors config.h) ─────────────────────────────────────────
HDR_PIC_TO_PI = 0xAA
HDR_PI_TO_PIC = 0xBB

TYPE_SENSORS   = 0x01   # [0xAA][0x01][soil][temp][hum][curr_h][curr_l][water]
TYPE_AT_PLANT  = 0x02   # [0xAA][0x02][plant_index]
TYPE_STATUS    = 0x03   # [0xAA][0x03][mode][lockout]
TYPE_HANDSHAKE = 0x10   # [0xAA][0x10][0xBB]

CMD_HANDSHAKE  = 0x10
CMD_MODE       = 0x01
CMD_IRRIGATE   = 0x02
CMD_ESTOP      = 0x03

MODE_AUTO   = 0x00
MODE_MANUAL = 0x01

# ── Globals ───────────────────────────────────────────────────────────────────
ser      = None
ser_lock = threading.Lock()

_prev_estop     = False
_prev_pump      = "OFF"
_prev_gantry_ts = 0
_prev_mode      = "AUTOMATIC"

# ── Firebase init ─────────────────────────────────────────────────────────────
def init_firebase():
    cred = credentials.Certificate(FIREBASE_CRED)
    firebase_admin.initialize_app(cred, {
        "databaseURL":   FIREBASE_DB_URL,
        "storageBucket": FIREBASE_BUCKET,
    })

# ── UART helpers ──────────────────────────────────────────────────────────────
def uart_send(data: bytes):
    with ser_lock:
        ser.write(data)

def send_handshake():
    uart_send(bytes([HDR_PI_TO_PIC, CMD_HANDSHAKE, 0xAA]))

def send_mode(mode_byte: int):
    uart_send(bytes([HDR_PI_TO_PIC, CMD_MODE, mode_byte & 0xFF]))

def send_irrigate(plant_index: int):
    uart_send(bytes([HDR_PI_TO_PIC, CMD_IRRIGATE, plant_index & 0xFF]))

def send_estop(active: bool):
    uart_send(bytes([HDR_PI_TO_PIC, CMD_ESTOP, 0x01 if active else 0x00]))

# ── Camera ────────────────────────────────────────────────────────────────────
def take_photo(plant_index: int) -> Optional[Path]:
    PHOTO_DIR.mkdir(parents=True, exist_ok=True)
    ts   = datetime.now().strftime("%Y%m%d_%H%M%S")
    path = PHOTO_DIR / f"plant{plant_index}_{ts}.jpg"
    try:
        subprocess.run(
            ["libcamera-jpeg", "-o", str(path),
             "--width", "1280", "--height", "720",
             "--nopreview", "-t", "1000"],
            check=True, timeout=15
        )
        print(f"[CAMERA] Captured {path.name}")
        return path
    except Exception as e:
        print(f"[CAMERA] Error: {e}")
        return None

def upload_photo_and_detect(path: Path, plant_index: int):
    """Upload photo to Storage, write URL + detection placeholder to ai/latest_detection."""
    try:
        bucket  = storage.bucket()
        ts_str  = datetime.now(timezone.utc).strftime("%Y%m%d_%H%M%S")
        blob    = bucket.blob(f"detections/plant{plant_index}_{ts_str}.jpg")
        blob.upload_from_filename(str(path))
        blob.make_public()
        url = blob.public_url

        now_utc = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")
        db.reference("ai/latest_detection").set({
            "class":      "Pending",
            "confidence": 0.0,
            "image_url":  url,
            "timestamp":  now_utc,
        })
        print(f"[PHOTO] plant{plant_index} uploaded → {url}")

        # Push alert
        db.reference("alerts/history").push({
            "message":   f"Photo captured at plant {plant_index} — awaiting AI analysis",
            "severity":  "info",
            "timestamp": int(time.time() * 1000),
        })
    except Exception as e:
        print(f"[PHOTO] Upload error: {e}")

# ── Firebase push helpers ─────────────────────────────────────────────────────
def push_sensors(soil: int, temp: int, hum: int, curr_ma: int, water: int):
    db.reference("sensors").set({
        "soil_moisture_pct": soil,
        "temperature":       round(temp, 1),
        "humidity":          hum,
        "current":           round(curr_ma / 1000.0, 5),  # mA → A to match schema
    })
    print(f"[SENSORS] soil={soil}% temp={temp}C hum={hum}% "
          f"curr={curr_ma}mA water={water}cm")

def push_status(mode_byte: int, lockout: int):
    mode_str = "AUTOMATIC" if mode_byte == MODE_AUTO else "MANUAL"
    db.reference("status").update({
        "mode":           mode_str,
        "mode_changed_at": int(time.time() * 1000),
        "system_state":   "FAULT" if lockout else "NORMAL",
    })

def push_pump_status(state: str):
    db.reference("status").update({"pump": state})

# ── UART packet reader (thread) ───────────────────────────────────────────────
def uart_reader():
    buf = bytearray()
    while True:
        try:
            raw = ser.read(1)
            if not raw:
                continue
            b = raw[0]

            if b == HDR_PIC_TO_PI:
                buf.clear()
                buf.append(b)
                continue

            if not buf:
                continue

            buf.append(b)

            if len(buf) < 2:
                continue

            pkt_type = buf[1]

            # [0xAA][0x01][soil][temp][hum][curr_h][curr_l][water]
            if pkt_type == TYPE_SENSORS and len(buf) >= 8:
                soil   = buf[2]
                temp   = buf[3]
                hum    = buf[4]
                curr   = (buf[5] << 8) | buf[6]
                water  = buf[7]
                push_sensors(soil, temp, hum, curr, water)
                buf.clear()

            # [0xAA][0x02][plant_index]
            elif pkt_type == TYPE_AT_PLANT and len(buf) >= 3:
                plant = buf[2]
                db.reference("status").update({
                    "gantry_x": plant,
                })
                print(f"[UART] At plant {plant} — taking photo")
                photo = take_photo(plant)
                if photo:
                    upload_photo_and_detect(photo, plant)
                buf.clear()

            # [0xAA][0x03][mode][lockout]
            elif pkt_type == TYPE_STATUS and len(buf) >= 4:
                push_status(buf[2], buf[3])
                buf.clear()

            # [0xAA][0x10][0xBB]
            elif pkt_type == TYPE_HANDSHAKE and len(buf) >= 3:
                if buf[2] == 0xBB:
                    print("[UART] PIC handshake ACK")
                buf.clear()

        except Exception as e:
            print(f"[UART] Reader error: {e}")
            time.sleep(0.1)

# ── Firebase command poller (thread) ─────────────────────────────────────────
def firebase_command_listener():
    global _prev_estop, _prev_pump, _prev_gantry_ts, _prev_mode

    while True:
        try:
            cmds   = db.reference("commands").get() or {}
            status = db.reference("status").get() or {}

            # ── Mode (read from status/mode, app writes it directly) ──────────
            mode_str = status.get("mode", "AUTOMATIC")
            if mode_str != _prev_mode:
                _prev_mode = mode_str
                mode_byte  = MODE_MANUAL if mode_str == "MANUAL" else MODE_AUTO
                send_mode(mode_byte)
                print(f"[CMD] Mode -> {mode_str}")

            # ── Emergency stop ────────────────────────────────────────────────
            estop = bool(cmds.get("emergency_stop", False))
            if estop != _prev_estop:
                _prev_estop = estop
                send_estop(estop)
                print(f"[CMD] E-stop -> {estop}")

            # ── Pump (manual mode only) ───────────────────────────────────────
            pump_cmd = cmds.get("pump") or {}
            pump_state = pump_cmd.get("state", "OFF")
            pump_ts    = pump_cmd.get("timestamp", 0)

            if pump_state == "ON" and pump_ts != _prev_gantry_ts:
                _prev_gantry_ts = pump_ts
                # gantry_move/x tells which plant to irrigate
                gantry = cmds.get("gantry_move") or {}
                plant  = int(gantry.get("x", 0))
                send_irrigate(plant)
                push_pump_status("ON")
                print(f"[CMD] Irrigate plant {plant}")

            elif pump_state == "OFF" and _prev_pump == "ON":
                push_pump_status("OFF")

            _prev_pump = pump_state

        except Exception as e:
            print(f"[FIREBASE] Listener error: {e}")

        time.sleep(1.0)

# ── Handshake ─────────────────────────────────────────────────────────────────
def do_handshake():
    print("[INIT] Sending handshake...")
    for _ in range(20):
        send_handshake()
        time.sleep(0.3)
    print("[INIT] Handshake done")

# ── Entry point ───────────────────────────────────────────────────────────────
def main():
    global ser

    PHOTO_DIR.mkdir(parents=True, exist_ok=True)

    print("[INIT] Firebase...")
    init_firebase()

    # Seed status node with safe defaults
    db.reference("status").update({
        "mode":         "AUTOMATIC",
        "pump":         "OFF",
        "gantry_x":     0,
        "system_state": "NORMAL",
    })

    print(f"[INIT] Serial {SERIAL_PORT} @ {BAUD_RATE}...")
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)

    threading.Thread(target=uart_reader, daemon=True).start()
    do_handshake()
    threading.Thread(target=firebase_command_listener, daemon=True).start()

    print("[INIT] Running. Ctrl-C to stop.")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n[EXIT] Stopped.")
        ser.close()

if __name__ == "__main__":
    main()
