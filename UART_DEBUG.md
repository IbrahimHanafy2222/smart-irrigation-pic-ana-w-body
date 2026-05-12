# UART Debug — PIC16F877A ↔ Raspberry Pi 4

## Project Overview

Smart irrigation controller. PIC16F877A @ 8 MHz (XC8 v2.36) controls pump relay, A4988 stepper motor, LCD, DHT11, soil moisture sensor, HC-SR04 water level, ACS712 current sensor. Raspberry Pi 4 Model B Rev 1.1 is the master — sends commands, receives sensor data.

Communication: 9600 baud 8N1, polling (no interrupts).

---

## The Problem

**PIC never receives bytes from Pi.** LCD RX counter never increments. Pi IS transmitting.

---

## Hardware Path

```
Pi GPIO14 (TX, 3.3V)
  → TXI (converter low/LV side)
  → TXO (converter high/HV side)
  → PIC RC7 (RX input, 5V MCU)

PIC RC6 (TX, 5V)
  → RXI (converter high/HV side)
  → RXO (converter low/LV side)
  → Pi GPIO15 (RX, 3.3V)
```

Level converter: bidirectional UART module, BSS138-based, 2-channel.
- LV pin = 3.3V (Pi side)
- HV pin = 5V (PIC side)
- GND common

---

## Pi UART Configuration

Pi 4 Model B Rev 1.1.  
`/boot/firmware/config.txt` contains:

```
dtoverlay=disable-bt
```

This maps `ttyAMA0` (PL011) to GPIO14/15. Currently using `/dev/ttyAMA0`.

`pinctrl get 14 15` shows GPIO14 = TXD0, GPIO15 = RXD0 — correct.

Verified port free: `fuser /dev/ttyS0` returns nothing.

---

## Pi Transmit — Confirmed Working

`handshake.py` sends `[0xBB][0x10][0xAA]` every 200 ms, waits up to 30 s for ACK `[0xAA][0x10][0xBB]`.

Loopback test (GPIO14 shorted to GPIO15): Pi receives its own bytes → Python serial stack works.

GPIO14 voltage: 3.28V idle (3.3V nominal — consistent with UART idle HIGH). Drops to ~3.26V while script runs (low duty cycle: 3 bytes / 200ms ≈ 1.5% → tiny average drop). Looks correct.

---

## PIC Receive — Never Triggers

Flashed standalone UART test firmware (`APP/Tests/uart_test.c` or the UART test `main()` in `MyProject.c`):

- LCD shows "UART Test Ready / Waiting bytes..."
- Runs `UART_DataAvailable()` in tight loop
- On any received byte: displays hex value (row 1) + running count (row 2)
- **Counter never increments regardless of what Pi sends**

---

## PIC UART Init

```c
void UART_Init(void)
{
    TRISC |= (1u << 7u);   /* RC7 = RX input */
    TRISC &= ~(1u << 6u);  /* RC6 = TX output */

    SPBRG  = 51u;          /* 9600 @ 8MHz: SPBRG = (8000000/(16*9600))-1 = 51.08 */
    TXSTA  = 0x24u;        /* TXEN=1, BRGH=1, async */
    RCSTA  = 0x90u;        /* SPEN=1, CREN=1 */
}
```

SPBRG=51, BRGH=1 → actual baud = 8000000 / (16 × 52) = 9615 baud (~0.16% error). Fine.

`UART_DataAvailable()` checks `PIR1<5>` (RCIF bit).

`UART_ClearOverrun()` exists — clears OERR by toggling CREN if set.

---

## TRIS / Port Config in Test Main

```c
ADCON1 = 0x06u;          /* AN0-AN2 analog, rest digital */
TRISA  = 0x07u;
TRISB  = 0b00011011u;
TRISC  = 0b10000000u;    /* RC7 = RX input; RC6 = TX output (cleared) */
TRISD  = 0x00u;
TRISE  = 0x00u;
CLR_BIT(OPTION_REG, 7u); /* PORTB pull-ups enabled */
PORTD  = 0xFFu;          /* Relays OFF */
SET_BIT(PORTC, 6u);      /* UART TX idle HIGH */
```

---

## What Has Been Tried

| Test | Result |
|------|--------|
| Pi loopback (GPIO14→GPIO15) | PASS — Pi receives own bytes |
| Pi → ttyS0 | Same result — no RX on PIC |
| Pi → ttyAMA0 | Same result — no RX on PIC |
| Pi → ttyAMA3 (GPIO4/5) via dtoverlay=uart3 | Same result |
| Check fuser on port | Free (no process holding it) |
| Voltage on GPIO14 idle | 3.28V ✓ |
| Voltage on GPIO14 during TX | 3.26V (expected for low duty cycle) |
| Level converter LV=3.3V, HV=5V | Verified |
| UART overrun fix (UART_ClearOverrun) | Added, does not fix RX |
| Brown-out reset loop | Fixed (was PSU issue, now bench supply) |
| Standalone UART test firmware | Still no RX |

---

## Suspected Causes (Not Yet Confirmed)

1. **Level converter wired wrong** — TXI/RXI swapped, or LV/HV swapped. Easy to mis-wire the 4-pin header.
2. **Level converter faulty/dead channel** — BSS138 gate threshold issue; one direction works, other doesn't.
3. **RC7 not actually input** — TRISC<7> must be 1; verify RCSTA.SPEN not overriding.
4. **RCSTA.SPEN=0** — if SPEN not set, RC7 is GPIO not UART RX. Check RCSTA after init.
5. **Pi serial console still active on ttyAMA0** — `/boot/firmware/cmdline.txt` may still have `console=serial0,115200`. This would corrupt Pi TX with kernel messages.
6. **Ground loop / floating ground** — if GND not common between Pi, converter, and PIC.

---

## Files

```
E:\temp_xc8\
├── pic-firmware\
│   ├── APP\
│   │   ├── MyProject.c          ← UART test main active (#if 0 guards production main)
│   │   └── Tests\
│   │       ├── uart_test.c      ← standalone UART test
│   │       └── stepper_test.c
│   ├── MCAL\USART\
│   │   ├── USART.c              ← UART_Init, UART_ClearOverrun
│   │   └── USART_interface.h
│   ├── HAL\Motor\Motor.c
│   └── CIRCUIT_PINOUT.md        ← full pinout + level converter wiring
├── pi-script\
│   └── handshake.py             ← Pi handshake sender (PORT=/dev/ttyAMA0)
└── PROGRESS.md
```

---

## Next Diagnostic Steps (Suggested)

1. **Check Pi cmdline.txt** — `cat /boot/firmware/cmdline.txt` — remove `console=serial0,115200` if present, reboot.
2. **Verify converter continuity** — multimeter: measure voltage on TXO (HV side) while Pi sends. Should toggle. If TXO stays HIGH, converter channel dead/miswired.
3. **Bypass converter** — temporarily connect GPIO14 (3.3V) directly to RC7. PIC VIH ≈ 2.0V @ 5V supply, so 3.3V is sufficient. Risky to 5V pins but RC7 is input only. If PIC receives bytes → converter is the problem.
4. **Verify RCSTA after init** — add LCD debug: display RCSTA value after UART_Init(). Should be 0x90.
5. **Scope or logic analyser on RC7** — confirm signal actually arrives at PIC pin.
