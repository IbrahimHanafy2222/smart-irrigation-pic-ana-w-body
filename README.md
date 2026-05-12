# Smart Irrigation Controller

PIC16F877A @ 8 MHz + Raspberry Pi 4 system. Soil/temp/humidity/water-level monitoring, stepper-driven gantry over 5 plant positions, app-controlled via UART.

**Status:** firmware compiles + flashes. Stepper motor + sensors verified working. **UART handshake Pi ↔ PIC currently broken** — see `UART_DEBUG_PLAN.md`.

---

## Repo Layout

```
.
├── pic-firmware/        PIC16F877A firmware (XC8 v2.36)
│   ├── APP/
│   │   ├── MyProject.c              Main (UART test main active, production main #if 0)
│   │   ├── Safety/                  Safety lockout state machine
│   │   ├── Comms/                   UART packet protocol Pi ↔ PIC
│   │   ├── Irrigation/              Irrigation cycle logic
│   │   └── Tests/                   Standalone test mains (uart_test, stepper_test)
│   ├── HAL/                         LCD, Buzzer, Button, Fan, Humidity, Ultrasonic, Motor
│   ├── MCAL/                        ADC, USART, MCU_Registers
│   ├── SERVICES/                    STD_TYPES, BIT_MATH
│   ├── config.h                     Pin assignments, thresholds, _XTAL_FREQ
│   └── CIRCUIT_PINOUT.md            Full pinout + wiring reference
│
├── pi-script/
│   └── handshake.py                 Pi-side handshake sender (currently failing)
│
├── UART_DEBUG_PLAN.md               ★ Comprehensive UART debug plan — START HERE
├── UART_DEBUG.md                    Original UART problem summary
├── PROGRESS.md                      Session-by-session progress notes
├── stepper_motor_docs.md            A4988 + 17HS4401 reference
└── README.md                        This file
```

---

## Hardware

- **MCU:** PIC16F877A @ 8 MHz (HS crystal), 5 V supply
- **Compiler:** MPLAB XC8 v2.36
- **Programmer:** PICkit 3/4 (any ICSP)
- **Master:** Raspberry Pi 4 Model B Rev 1.1, RPi OS Bookworm
- **Level converter:** bidirectional BSS138 4-channel module
- **Stepper:** 17HS4401 NEMA17 + A4988 driver @ 1/16 microstepping
- **LCD:** HD44780 16x2, 4-bit mode
- **Sensors:** DHT11, soil moisture (capacitive v1.2), HC-SR04, ACS712-05B
- **Actuators:** 2× active-low relay (pump, motor power), buzzer, cooling fan, 2× indicator LED

Full pin mapping → `pic-firmware/CIRCUIT_PINOUT.md`.

---

## Build

### PIC firmware (Windows, MPLAB X IDE)

1. Open MPLAB X → File → Open Project → `pic-firmware/`
2. Set device: PIC16F877A
3. Compiler: XC8 v2.36
4. Build (F11). Output `.hex` lives in `dist/default/production/`.
5. Flash via PICkit at 5 V, target NOT powered by programmer (use external).

### Pi script

```bash
sudo apt install python3-serial
cd pi-script
python3 handshake.py
```

---

## UART Protocol

**9600 baud, 8N1, polling (no interrupts on PIC).**

Handshake (must succeed before main loop runs):
- Pi → PIC: `[0xBB][0x10][0xAA]`
- PIC → Pi: `[0xAA][0x10][0xBB]`

Runtime packets:
- Pi → PIC: `[0xBB][CMD][DATA]` — 3 bytes
  - CMD 0x01: mode (0x00=auto, 0x01=manual)
  - CMD 0x02: manual irrigate plant N (0-4)
  - CMD 0x03: app e-stop (0x01=set, 0x00=clear)
- PIC → Pi: `[0xAA][TYPE][...data]` — variable length
  - TYPE 0x01 (6 bytes): soil%, temp°C, hum%, curr_hi, curr_lo, water_cm
  - TYPE 0x02 (1 byte): plant index — "take photo now"
  - TYPE 0x03 (2 bytes): mode, lockout flag

---

## Current Problem (Pickup Point for Friend)

**PIC never receives bytes from Pi.** Pi confirmed transmitting (loopback test passes, GPIO14 voltage correct). PIC standalone UART test counter never increments.

Suspected causes (ranked) in `UART_DEBUG_PLAN.md`:

1. Level converter miswired / dead channel / unpowered
2. PIC VIH = 4.0 V at 5 V supply — 3.3 V on RC7 is below spec; converter must lift to 5 V
3. Pi serial console still bound to ttyAMA0 (corrupts TX stream)
4. PIC oscillator not running at 8 MHz (baud mismatch)
5. Common ground missing between Pi + converter + PIC

**`UART_DEBUG_PLAN.md` has a full triage:**
- Stage A: PIC alive (oscillator + TX test)
- Stage B: Pi-side software + drivers (11 sub-tests)
- Stage C: level converter
- Stage D: PIC register dump + RC7 GPIO read
- Stage E: software/integration
- Stage F: Pi kernel/driver layer

Plus a section for debugging at home without bench supply (uses Pi 5V rail or HW-131/MB102 + NodeMCU).

**Two NodeMCU tests** (Test 1: sniff Pi TX → no converter needed. Test 2: NodeMCU replaces Pi to verify PIC + converter → uses converter).

Current `MyProject.c` runs the Test 2 reception main (looks for BB-10-AA pattern, displays handshake count on LCD).

---

## What Works (verified)

- LCD 16x2 in 4-bit mode → text displays correctly
- Soil moisture ADC reads soil dryness 0–100 %
- ACS712 current sensor reads ~0 mA at idle
- DHT11 temp + humidity reads valid values
- HC-SR04 water level reads in cm
- Stepper motor moves between plant positions (verified with Vref ≈ 0.85 V)
- Limit switch homing works
- E-stop button toggle works
- Buzzer beeps
- Brown-out reset issue → solved by using bench PSU (not breadboard MB102 under transient load)

## What's Untested

- End-to-end Pi ↔ PIC comms (blocked by handshake failure)
- Full automatic irrigation cycle with real pump load
- ACS712 reading under load (haven't run pump yet)
- Production main long-run stability

---

## Notes for Whoever Picks This Up

- Caveman commits/PRs are fine but write real English in code comments + docs.
- Production main is gated by `#if 0` in `pic-firmware/APP/MyProject.c`. Change to `#if 1` once UART verified.
- The Test 2 reception main (currently active) detects the BB-10-AA pattern using a 3-state FSM and shows handshake count on LCD row 2.
- Stepper motor + pump need external 12 V supply for VMOT + pump load. Pi 5V GPIO won't cut it.
- Don't trust MB102/HW-131 under transient load — voltage sags below 4.5 V cause PIC brown-out resets.

Contact: Ibrahem Hanafy — s-ibrahim.hanafy@zewailcity.edu.eg
