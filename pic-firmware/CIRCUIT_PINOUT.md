# Circuit & Pinout Reference
## PIC16F877A @ 8 MHz — Smart Irrigation Controller

---

## MCU Overview

| Property | Value |
|---|---|
| MCU | PIC16F877A |
| Clock | 8 MHz external crystal (HS mode) |
| Supply | 5 V |
| Compiler | MPLAB XC8 v2.36 |
| Config bits | FOSC=HS, WDTE=OFF, PWRTE=ON, BOREN=ON, **LVP=OFF** (frees RB3) |

---

## Complete Pin Assignment

| PIC Pin | Port/Pin | Signal | Direction | Notes |
|---|---|---|---|---|
| 2 | RA0 / AN0 | Soil moisture AO | Input (analog) | Capacitive sensor v1.2 |
| 3 | RA1 / AN1 | *(unused)* | Input (analog) | Configured analog, never read |
| 4 | RA2 / AN2 | ACS712-05B OUT | Input (analog) | Current sensor |
| 13 | OSC1 | 8 MHz crystal | — | |
| 14 | OSC2 | 8 MHz crystal | — | |
| 33 | RB0 | DHT11 DATA | Bidirectional | 4.7 kΩ pull-up to VCC required |
| 34 | RB1 | HC-SR04 ECHO | Input | |
| 35 | RB2 | Buzzer (via NPN) | Output | Active HIGH |
| 36 | RB3 | Limit switch | Input | Active LOW, PORTB pull-up |
| 37 | RB4 | E-Stop button | Input | Active LOW, PORTB pull-up |
| 15 | RC0 | A4988 STEP | Output | Step pulse |
| 16 | RC1 | A4988 DIR | Output | HIGH = away from home |
| 17 | RC2 | A4988 ENABLE | Output | LOW = enabled, HIGH = disabled |
| 18 | RC3 | HC-SR04 TRIG | Output | 10 µs pulse |
| 23 | RC4 | Fan (via NPN) | Output | Active HIGH |
| 25 | RC6 | UART TX → Pi | Output | PIC → Pi (9600 baud) |
| 26 | RC7 | UART RX ← Pi | Input | Pi → PIC |
| 19 | RD0 | Pump relay IN | Output | **Active LOW: LOW=ON, HIGH=OFF** |
| 20 | RD1 | Motor relay IN | Output | **Active LOW: LOW=ON, HIGH=OFF** |
| 21 | RD2 | LCD RS | Output | |
| 22 | RD3 | LCD Enable | Output | |
| 27 | RD4 | LCD D4 | Output | |
| 28 | RD5 | LCD D5 | Output | |
| 29 | RD6 | LCD D6 | Output | |
| 30 | RD7 | LCD D7 | Output | |
| 9 | RE1 | Yellow/Amber LED | Output | Warning indicator |
| 10 | RE2 | Red LED | Output | Fault/Lockout indicator |
| 11, 32 | VDD | +5 V | Power | |
| 12, 31 | VSS | GND | Power | |

> **PORTD shadow register:** PORTD is shared by the LCD (RD2–RD7) and pump/spare relays (RD0–RD1).
> All PORTD writes in firmware go through `portd_shadow` to prevent relay state corruption during LCD operations.

> **Voltage warning:** PIC UART TX outputs 5 V. Pi RX expects max 3.3 V. Use a resistor divider
> (1 kΩ + 2 kΩ) or level-shifter on the PIC TX → Pi RX line. Pi TX (3.3 V) → PIC RX is safe (PIC VIH ≈ 2.0 V @ 5 V supply).

---

## LCD — HD44780 16x2 (4-bit mode)

```
LCD Module          PIC16F877A
──────────          ──────────
VSS  (pin 1)  ───── GND
VDD  (pin 2)  ───── +5V
VO   (pin 3)  ───── 10 kΩ pot wiper (contrast, between VCC and GND)
RS   (pin 4)  ───── RD2
RW   (pin 5)  ───── GND  (always write — do NOT connect to PIC)
E    (pin 6)  ───── RD3
D0–D3 (7–10)  ───── NOT CONNECTED (4-bit mode)
D4   (pin 11) ───── RD4
D5   (pin 12) ───── RD5
D6   (pin 13) ───── RD6
D7   (pin 14) ───── RD7
A    (pin 15) ───── +5V via 33 Ω (backlight)
K    (pin 16) ───── GND
```

---

## DHT11 — Temperature & Humidity Sensor

```
DHT11               PIC16F877A
─────               ──────────
VCC  (pin 1)  ───── +5V
DATA (pin 2)  ───── RB0
               └─── 4.7 kΩ pull-up to +5V  ← REQUIRED
NC   (pin 3)  ───── (not connected)
GND  (pin 4)  ───── GND
```

**Error codes returned by `Humidity_Read()`:**

| Code | Meaning |
|---|---|
| 0 | OK |
| 2 | No sensor response (check wiring / pull-up) |
| 3 | Bit-read timeout |
| 4 | Checksum mismatch (noisy line) |

---

## HC-SR04 — Ultrasonic Distance Sensor (water level)

```
HC-SR04             PIC16F877A
───────             ──────────
VCC           ───── +5V
TRIG          ───── RC3  (10 µs pulse)
ECHO          ───── RB1  (measure HIGH duration)
GND           ───── GND
```

**Firmware constants:**

| Parameter | Value |
|---|---|
| Tank height | 30 cm |
| Warn level | water_cm ≤ 5 cm |
| Action level | water_cm ≤ 2 cm |
| Clear level | water_cm ≥ 6 cm |
| Loop time per count | 2 µs |
| Timeout | 12500 loops (25 ms) |

Water level = `TANK_HEIGHT_CM − measured_distance_cm`. Median of 3 readings used.

---

## Soil Moisture Sensor (capacitive v1.2)

```
Sensor Module       PIC16F877A
─────────────       ──────────
VCC           ───── +5V
GND           ───── GND
AO (analog)   ───── RA0 / AN0
DO (digital)  ───── NOT CONNECTED
```

**Calibration (in `config.h`):**

| State | ADC Raw | % |
|---|---|---|
| Dry (air) | 614 | 0 % |
| Wet (water) | 245 | 100 % |

Formula: `soil_pct = 100 − ((raw − 245) × 100 / (614 − 245))`, clamped 0–100.

---

## ACS712-05B — Current Sensor (±5 A)

```
ACS712 Module       PIC16F877A / Load
──────────────      ─────────────────
VCC           ───── +5V
GND           ───── GND
OUT           ───── RA2 / AN2
IP+  }
IP-  }        ───── In series with pump motor
```

**Firmware constants:**

| Parameter | Value |
|---|---|
| Zero-current ADC | 512 (2.5 V at 5 V supply) |
| Sensitivity | 185 mV/A → ×26 counts/A |
| Warn threshold | 3500 mA |
| Action threshold | 4500 mA |
| Clear threshold | 3000 mA |

Formula: `curr_mA = abs(raw − 512) × 26`

---

## Dual-Channel Relay Module (Active LOW)

```
Relay Module        PIC16F877A          Load
────────────        ──────────          ────
VCC           ───── +5V
GND           ───── GND
IN1           ───── RD0               Pump (LOW=ON, HIGH=OFF)
IN2           ───── RD1               Motor / A4988 VMOT (LOW=ON, HIGH=OFF)
```

> Both relays start HIGH (OFF) at power-up via `portd_shadow = 0xFF`. Safe start.
> Motor relay is switched ON by Motor.c before any move and OFF by `Motor_Disable()`.
> Safety lockout and e-stop both cut motor relay immediately via `portd_shadow`.

---

## Buzzer

```
RB2 ──[1 kΩ]── NPN Base (2N2222)
               Collector ── Buzzer+ ── +5V
               Emitter   ── GND
               Buzzer−   ── GND
```

Active HIGH on RB2. Patterns: 1 beep = warning, 2 beeps = lockout event.

---

## Stepper Motor — A4988 Driver + 17HS4401 NEMA 17

### A4988 ↔ PIC connections

```
A4988 Pin           PIC16F877A / Hardware
─────────           ─────────────────────
STEP          ───── RC0          (step pulse output)
DIR           ───── RC1          (HIGH = away from home / CW)
ENABLE        ───── RC2          (LOW = enabled, HIGH = disabled)
MS1           ───── +5V          (hardwired — 1/16 microstepping)
MS2           ───── +5V          (hardwired — 1/16 microstepping)
MS3           ───── +5V          (hardwired — 1/16 microstepping)
RESET         ───── +5V          (hardwired — never reset)
SLEEP         ───── +5V          (hardwired — never sleep)
VDD (logic)   ───── +5V
VMOT          ───── 12 V PSU     (100 µF decoupling cap required)
GND           ───── GND          (common with PIC and PSU)
```

**Microstepping table (MS1/MS2/MS3 all HIGH = 1/16):**

| MS1 | MS2 | MS3 | Mode | Steps/rev |
|-----|-----|-----|------|-----------|
| L   | L   | L   | Full step | 200 |
| H   | L   | L   | Half step | 400 |
| L   | H   | L   | 1/4 step  | 800 |
| H   | H   | L   | 1/8 step  | 1600 |
| **H** | **H** | **H** | **1/16 step** | **3200** ← firmware |

### 17HS4401 motor coil wiring

```
A4988 Pin     Wire colour    Motor terminal
──────────    ───────────    ──────────────
1B            Red            Coil A+
1A            Black          Coil A−
2A            Green          Coil B+
2B            Blue           Coil B−
```

> If motor spins wrong direction: swap Red ↔ Black **or** Green ↔ Blue (one coil only, not across coils).

**Motor specs (17HS4401):** 1.7 A rated · Set A4988 Vref ≈ 0.85 V for current limit.

**Limit switch (home position):**
```
RB3 ──── Limit switch ──── GND   (active LOW, PORTB pull-up enabled)
```

**Plant positions (microsteps from home, 1/16 step, GT2 20T pulley):**

| Plant | Steps | Distance |
|---|---|---|
| 0 | 4 000 | 50 mm |
| 1 | 12 000 | 150 mm |
| 2 | 20 000 | 250 mm |
| 3 | 28 000 | 350 mm |
| 4 | 36 000 | 450 mm |

---

## Cooling Fan

```
RC4 ──[1 kΩ]── NPN Base
               Collector ── Fan+ ── +5V (or 12V with appropriate NPN)
               Emitter   ── GND
```

Active HIGH on RC4. Turns ON at temp ≥ 55 °C, turns OFF when temp < 50 °C and all other sensors clear.

---

## LEDs

```
RE1 ──[330 Ω]── Yellow/Amber LED anode ── cathode ── GND   (Warning)
RE2 ──[330 Ω]── Red LED anode          ── cathode ── GND   (Fault/Lockout)
```

---

## E-Stop Button

```
RB4 ──── Normally-open button ──── GND   (active LOW, PORTB pull-up)
```

Press = toggle e-stop. First press: pump OFF, stepper disabled, LCD "E-STOP ACTIVE". Second press: releases.

---

## UART — Pi ↔ PIC Communication

```
Pi (3.3 V)          UART Level Shifter Module       PIC16F877A (5 V)
──────────          ─────────────────────────       ────────────────
3.3 V       ─────── LV                  HV ──────── 5 V
GND         ─────── GND                GND ──────── GND
GPIO14 (TX) ─────── TXI  →  TXO ───────────────── RC7 (RX)
GPIO15 (RX) ─────── RXO  ←  RXI ───────────────── RC6 (TX)
```

> Module is a dedicated bi-directional UART level shifter (2-channel, BSS138-based).
> LV must read 3.3 V, HV must read 5 V — verify with multimeter before powering up.
> TXI/RXO = Pi (low) side. TXO/RXI = PIC (high) side. Do NOT swap.

**Protocol: 9600 baud, 8N1, polling mode (no interrupts)**

| Direction | Format | Meaning |
|---|---|---|
| Pi → PIC | `[0xBB][CMD][DATA]` | 3-byte fixed packet |
| PIC → Pi | `[0xAA][TYPE][DATA...]` | Variable length |

**Pi → PIC commands:**

| CMD | DATA | Action |
|---|---|---|
| 0x01 | 0x00/0x01 | Switch mode (auto/manual) |
| 0x02 | 0–4 | Irrigate plant N (manual) |
| 0x03 | 0x01/0x00 | App e-stop set/release |

**PIC → Pi packets:**

| TYPE | Data bytes | Content |
|---|---|---|
| 0x01 | 6 | soil%, temp°C, hum%, curr_hi, curr_lo, water_cm |
| 0x02 | 1 | plant_index — "take image now" |
| 0x03 | 2 | mode, lockout flag |

---

## Crystal Oscillator

```
8 MHz crystal
  Pin 1 ───── OSC1 (PIC pin 13) ──┬── 22 pF cap ── GND
  Pin 2 ───── OSC2 (PIC pin 14) ──┴── 22 pF cap ── GND
```

---

## Power Supply Decoupling

Place **100 nF ceramic** between VDD and VSS close to PIC (pins 11/12 and 32/31). Add **10 µF electrolytic** in parallel. Separate decoupling cap for A4988 VMOT (100 µF, close to driver).

---

## Safety Thresholds Summary

| Feature | Warn | Action | Clear |
|---|---|---|---|
| Soil overwater | ≥ 80 % | ≥ 90 % | ≤ 75 % |
| Overcurrent | ≥ 3500 mA | ≥ 4500 mA | ≤ 3000 mA |
| Overheat | ≥ 55 °C | ≥ 70 °C | ≤ 50 °C |
| Low water | ≤ 5 cm | ≤ 2 cm | ≥ 6 cm |

Lockout clears only when **all 4** sensors are simultaneously below their clear thresholds.

---

## ADC Channel Summary

| Channel | Pin | Sensor | Formula |
|---|---|---|---|
| AN0 | RA0 | Soil moisture | `100 − ((raw−245)×100/369)` |
| AN1 | RA1 | *(unused)* | — |
| AN2 | RA2 | ACS712 current | `abs(raw−512) × 26` |

ADC clock: Fosc/32 → Tad = 4 µs @ 8 MHz. VREF = VDD/VSS.

---

## LCD Display Pages (main loop, cycling every 2 s)

| Page | Row 1 | Row 2 |
|---|---|---|
| 0 | `Soil:XX%` | Status / fault |
| 1 | `Temp:XXC` | Status / fault |
| 2 | `Hum :XX%` | Status / fault |
| 3 | `Curr:XXXXmA` | Status / fault |
| 4 | `Watr:XXcm` | Status / fault |

Row 2 shows: `E-STOP ACTIVE`, `LOCKOUT!`, `Manual Mode`, or `Auto Mode OK`.
