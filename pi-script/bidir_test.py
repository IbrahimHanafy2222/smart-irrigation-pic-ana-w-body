#!/usr/bin/env python3
"""
bidir_test.py  —  Bidirectional UART test: Pi <-> PIC16F877A
Protocol:
  Pi  -> PIC : [0xBB] [0x10] [0xAA]   (sent with 100 ms gaps between bytes)
  PIC -> Pi  : [0xAA] [0x10] [0xBB]   (ACK, sent immediately)

Run:
  python3 bidir_test.py

Pass:  every TX line is followed by an RX OK line.
Fail:  TIMEOUT means PIC did not send the ACK back.
"""

import serial
import time
import sys

PORT          = '/dev/ttyS0'   # use /dev/ttyAMA0 on Pi Zero / Pi 1 / Pi 2
BAUD          = 9600
BYTE_GAP_S    = 0.10           # 100 ms between bytes — lets PIC read each one
INTERVAL_S    = 3.0            # seconds between handshake attempts
ACK_TIMEOUT_S = 2.0            # how long to wait for the ACK

TX_BYTES  = [0xBB, 0x10, 0xAA]
ACK_BYTES = bytes([0xAA, 0x10, 0xBB])


def wait_for_ack(ser, timeout):
    buf = bytearray()
    deadline = time.time() + timeout
    while time.time() < deadline:
        chunk = ser.read(ser.in_waiting or 1)
        if chunk:
            buf.extend(chunk)
        while len(buf) >= 3:
            if bytes(buf[:3]) == ACK_BYTES:
                return True, bytes(buf[:3])
            buf.pop(0)
    return False, bytes()


def main():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.05)
    except serial.SerialException as e:
        print(f"[ERROR] Cannot open {PORT}: {e}")
        sys.exit(1)

    time.sleep(0.1)
    ser.reset_input_buffer()

    sent = 0
    ok   = 0

    print(f"Bidirectional test  {PORT} @ {BAUD} baud")
    print(f"TX every {INTERVAL_S} s  |  {BYTE_GAP_S*1000:.0f} ms gap between bytes")
    print("Ctrl-C to stop\n")

    try:
        while True:
            sent += 1

            # Send the 3 bytes with a gap so PIC FIFO doesn't overflow
            for byte in TX_BYTES:
                ser.write(bytes([byte]))
                time.sleep(BYTE_GAP_S)

            tx_str = ' '.join(f'{b:02X}' for b in TX_BYTES)
            print(f"[TX #{sent:4d}]  {tx_str}", end="  ", flush=True)

            success, ack = wait_for_ack(ser, ACK_TIMEOUT_S)
            if success:
                ok += 1
                ack_str = ' '.join(f'{b:02X}' for b in ack)
                print(f"[RX OK]  {ack_str}   ({ok}/{sent})")
            else:
                print(f"[TIMEOUT]  no ACK   ({ok}/{sent})")

            time.sleep(INTERVAL_S)

    except KeyboardInterrupt:
        print(f"\nStopped.  {ok}/{sent} successful round-trips.")
        ser.close()


if __name__ == '__main__':
    main()
