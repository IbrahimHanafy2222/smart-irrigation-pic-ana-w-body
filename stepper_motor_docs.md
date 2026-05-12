# Stepper Motor Project Documentation

**Target:** PIC16F877A · **Driver:** A4988 · **Motor:** 17HS4401 (NEMA 17) · **Clock:** 8 MHz external crystal · **Compiler:** XC8

---

## Pin Connections

| PIC16F877A Pin | A4988 Pin | Notes |
|---|---|---|
| RC0 | STEP | Step pulse |
| RC1 | DIR | Direction |
| RC2 | ENABLE | Active LOW |
| 5V | VDD | Logic power |
| GND | GND | Common ground |

**Motor wiring (A4988 → 17HS4401):**

| A4988 Pin | Wire Color | Motor Terminal |
|---|---|---|
| 1B | Red | B+ |
| 1A | Black | A+ |
| 2A | Green | A− |
| 2B | Blue | B− |

**Microstepping:** MS1 = MS2 = MS3 = GND → Full step mode (200 steps/rev)

**Power:** VMOT = 12V external PSU · All GNDs connected together

---

## File 1 — `stepper_test.c`

### Purpose

One-time test: rotates the motor exactly **360°** (1 full revolution) then stops and disables the driver.

### Key Parameters

| Parameter | Value | Description |
|---|---|---|
| `STEPS_ONE_REV` | 200 | Steps for 1 full revolution (full step mode) |
| `STEP_DELAY_MS` | 5 ms | Delay between steps — controls speed |
| `CW` | 1 | Clockwise direction |
| `CCW` | 0 | Counter-clockwise direction |

### Execution Flow

```
Power ON
  │
  ▼
Setup pins as OUTPUT (RC0, RC1, RC2)
  │
  ▼
Initial state: STEP=0, DIR=CW, ENABLE=1 (disabled), ADCON1=0x06 (all digital)
  │
  ▼
Delay 100 ms (startup)
  │
  ▼
Enable driver: ENABLE = 0
Delay 5 ms
  │
  ▼
Set DIR = CW
Delay 5 µs (DIR settle time)
  │
  ▼
Loop 200 steps:
  STEP = 1 → delay 2 µs → STEP = 0 → delay 2 µs
  Wait 5 ms (inter-step delay)
  │
  ▼
Disable driver: ENABLE = 1
  │
  ▼
Halt forever: while(1)
```

### Step Pulse Timing

```
STEP pin:  _____|‾‾|___________________|‾‾|___
               2µs 2µs    ~5ms        2µs 2µs
```

Each step = 4 µs pulse + 5 ms delay ≈ **5.004 ms/step**
Total for 200 steps ≈ **~1 second** for one full revolution.

### Notes

- `ADCON1 = 0x06` sets PORTA/E pins as digital — important since RC pins share with analog functions on this PIC.
- The 100 ms startup delay allows the A4988 to power up and stabilize before the first pulse.
- After completion, `ENABLE = 1` releases motor holding torque — motor is free to spin.

---

## File 2 — `stepper_plates.c`

### Purpose

Production control program. Moves a belt-driven carriage between **4 plate positions**, waiting 20 seconds at each, then returns to the start and repeats forever.

### Mechanical Setup

| Parameter | Value |
|---|---|
| Pulley diameter | 20 mm |
| Pulley circumference | π × 20 = **62.83 mm** |
| Distance between plates | **50 mm** |
| Steps/mm | 200 ÷ 62.83 = **3.183 steps/mm** |
| Steps per plate | 50 × 3.183 = **159 steps** |

### Key Parameters

| Constant | Value | Description |
|---|---|---|
| `STEPS_PER_PLATE` | 159 | Steps to move 50 mm |
| `TOTAL_PLATES` | 4 | Number of plate positions |
| `STEP_DELAY_MS` | 5 ms | Inter-step delay (speed control) |
| `WAIT_SECONDS` | 20 s | Dwell time at each plate |

### Speed Reference

| `STEP_DELAY_MS` | Speed | Use case |
|---|---|---|
| 2 ms | Fast (~500 steps/sec) | Light loads |
| 5 ms | Medium | **Recommended for belt drive** |
| 10 ms | Slow | Heavy loads, less stalling risk |

### Behavior Cycle

```
[Plate 1] ──wait 20s──► move 159 steps CW ──► [Plate 2]
                                                    │
                                               wait 20s
                                                    │
                                                    ▼
[Plate 1] ◄── move 477 steps CCW ◄── [Plate 4] ◄── [Plate 3]
                                          │
                                     wait 20s
                                          │
                                     [Plate 4] ◄── move 159 steps CW ◄── [Plate 3]
                                                                               │
                                                                          wait 20s
```

**Return move:** 3 × 159 = **477 steps CCW** to go from Plate 4 back to Plate 1 in one motion.

### Function Reference

| Function | Description |
|---|---|
| `init()` | Sets pin directions, initial output states, all-digital mode |
| `motor_enable()` | ENABLE = 0, waits 5 ms for driver to stabilize |
| `motor_disable()` | ENABLE = 1, releases holding torque |
| `set_direction(dir)` | Sets DIR pin, waits 5 µs for signal to settle |
| `step_pulse()` | Sends one STEP pulse (HIGH 2 µs → LOW 2 µs) |
| `move_steps(steps, dir)` | Moves N steps in given direction with inter-step delay |
| `wait_seconds(seconds)` | Disables motor during wait, re-enables before next move |

### Power-Saving Behavior

During each 20-second wait, `wait_seconds()` **disables the driver** (`ENABLE = 1`) to reduce heat and power consumption, then re-enables it before the next move. This means the motor holds **no torque** while waiting — acceptable if the belt and load are self-locking or gravity-neutral.

> If the load can back-drive the belt during the wait, remove the `motor_disable()` / `motor_enable()` calls inside `wait_seconds()`.

---

## Common Configuration (Both Files)

### Configuration Bits

| Bit | Setting | Meaning |
|---|---|---|
| `FOSC` | HS | High-speed crystal oscillator |
| `WDTE` | OFF | Watchdog timer disabled |
| `PWRTE` | ON | Power-up timer enabled |
| `BOREN` | ON | Brown-out reset enabled |
| `LVP` | OFF | Low-voltage programming disabled |
| `CPD` | OFF | Data EEPROM code protect off |
| `WRT` | OFF | Flash write protect off |
| `CP` | OFF | Code protect off |

### A4988 Timing Requirements (met by both files)

| Requirement | Minimum | Code provides |
|---|---|---|
| STEP pulse width | 1 µs | 2 µs |
| DIR setup before STEP | 200 ns | 5 µs |
| Enable setup time | 1 µs | 5 ms |

---

## Testing Checklist

**Before first run:**

- [ ] All GNDs connected together (PIC, A4988, PSU)
- [ ] MS1, MS2, MS3 all tied to GND
- [ ] A4988 current limit set for 17HS4401 (rated 1.7 A — set Vref ≈ 0.85 V)
- [ ] 12V PSU connected to VMOT with a 100 µF decoupling capacitor
- [ ] Motor wires in correct order: Red→1B, Black→1A, Green→2A, Blue→2B
- [ ] Flash `stepper_test.c` first — verify motor completes exactly one revolution
- [ ] If direction is wrong: swap Red↔Blue **or** Black↔Green (not across coils)
- [ ] Once test passes, flash `stepper_plates.c` for the full plate cycle

---

*PIC16F877A · A4988 · 17HS4401 NEMA 17 · XC8 Compiler · 8 MHz HS Crystal*
