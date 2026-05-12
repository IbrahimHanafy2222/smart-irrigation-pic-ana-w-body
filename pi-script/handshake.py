#!/usr/bin/env python3
"""
handshake.py
Pi-side handshake with PIC16F877A smart irrigation controller.

Protocol:
  Pi  → PIC : [0xBB][0x10][0xAA]  (handshake request)
  PIC → Pi  : [0xAA][0x10][0xBB]  (handshake ACK)

Run on Pi:
  python3 handshake.py

Requires: pip3 install pyserial
"""

import serial
import time
import sys

# ── Config ────────────────────────────────────────────────────────────────────
PORT      = '/dev/ttyS0'
BAUD      = 9600
TIMEOUT_S = 30             # seconds to wait for ACK before giving up
RETRY_MS  = 200            # resend handshake every 200 ms until ACK arrives

# Protocol constants (must match config.h)
HDR_PI_TO_PIC  = 0xBB
CMD_HANDSHAKE  = 0x10
DATA_HANDSHAKE = 0xAA

HDR_PIC_TO_PI  = 0xAA
TYPE_HANDSHAKE = 0x10
ACK_BYTE       = 0xBB
# ─────────────────────────────────────────────────────────────────────────────


def open_serial():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.1)
        time.sleep(0.1)          # let port settle
        ser.reset_input_buffer()
        return ser
    except serial.SerialException as e:
        print(f"[ERROR] Cannot open {PORT}: {e}")
        sys.exit(1)


def do_handshake(ser):
    """
    Retry sending [0xBB][0x10][0xAA] every RETRY_MS ms.
    Returns True when ACK [0xAA][0x10][0xBB] received, False on timeout.
    """
    packet = bytes([HDR_PI_TO_PIC, CMD_HANDSHAKE, DATA_HANDSHAKE])
    deadline = time.time() + TIMEOUT_S
    buf = bytearray()

    print(f"[INFO] Sending handshake on {PORT} @ {BAUD} baud ...")

    while time.time() < deadline:
        ser.write(packet)
        print(f"[TX]   {packet.hex(' ')}")

        wait_until = time.time() + (RETRY_MS / 1000.0)
        while time.time() < wait_until:
            data = ser.read(ser.in_waiting or 1)
            if data:
                buf.extend(data)

            # Scan buffer for ACK pattern [0xAA][0x10][0xBB]
            while len(buf) >= 3:
                if buf[0] == HDR_PIC_TO_PI and buf[1] == TYPE_HANDSHAKE and buf[2] == ACK_BYTE:
                    print(f"[RX]   {bytes(buf[:3]).hex(' ')}")
                    print("[OK]   Handshake complete — PIC is ready.")
                    return True
                buf.pop(0)   # discard non-matching leading byte

    print(f"[FAIL] No ACK received within {TIMEOUT_S} s.")
    return False


def main():
    ser = open_serial()
    success = do_handshake(ser)
    ser.close()
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
