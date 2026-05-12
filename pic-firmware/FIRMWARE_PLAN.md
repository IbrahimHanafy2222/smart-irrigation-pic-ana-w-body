# Plan: PIC16F877A Smart Irrigation Firmware — Auto + Manual Mode

## Context

Course project CIE-406. PIC16F877A runs local safety + actuation. Raspberry Pi runs CV model, Firebase, and receives app commands. Pi ↔ PIC talk over UART (RC6/RC7, already reserved). App has two modes — automatic (timer-driven) and manual (user picks plant). Everything below reflects 8MHz crystal, DHT11, soil + current sensors.

---

## System Architecture

```
[ App (mobile) ]
      │  Firebase (read/write mode + commands)
      ▼
[ Raspberry Pi 4 ]
  - CV model: takes image at each plant, outputs plant health status
  - Uploads image + status to Firebase
  - Receives app commands (mode switch, manual irrigate)
  - Talks to PIC over UART (9600 baud)
      │  UART: RC6=TX(PIC→Pi), RC7=RX(Pi→PIC)
      ▼
[ PIC16F877A ]
  - Sensor polling (DHT11, soil, current, ultrasonic)
  - Safety state machine (4 features, lockout)
  - Motor gantry control (home + move to plant)
  - Pump relay control
  - LCD + LEDs + buzzer + fan
  - Mode: AUTO or MANUAL
```

---

## Communication Protocol (UART 9600 baud, 8N1)

**Pi → PIC packets (3 bytes, fixed):**

```
[0xBB][CMD][DATA]

CMD 0x01  Mode switch      DATA: 0x00=auto, 0x01=manual
CMD 0x02  Irrigate plant   DATA: 0x00–0x04 (plant index)
CMD 0x03  App E-stop       DATA: 0x01=stop, 0x00=release
```

**PIC → Pi packets:**

```
[0xAA][TYPE][DATA...]

TYPE 0x01  Sensor update    [soil_pct, temp_c, humidity, curr_hi, curr_lo, water_cm]  6 data bytes
TYPE 0x02  At plant N       [plant_index]  1 byte — "take image now"
TYPE 0x03  System status    [mode, lockout]  2 bytes — sent on any state change
```

**Pi receives 0xAA 0x02 [N]** → captures image of plant N → runs CV model → uploads image + health status to Firebase → optionally sends health status back to PIC (future extension; not blocking PIC).

**Voltage warning:** Pi GPIO is 3.3V; PIC UART outputs 5V. Use a resistor divider or level shifter on PIC TX → Pi RX line. Pi RX → PIC RX: 3.3V is within PIC VIH spec (PIC16F877A VIH min = 0.25×VDD+0.8V = 2.05V @ 5V supply). 3.3V input to PIC RX: **OK**. PIC TX (5V) to Pi RX (3.3V max): **needs level shift or voltage divider**.

**USART config @ 8MHz, 9600 baud:**

- BRGH=1 (high speed): SPBRG = (8000000 / (16 × 9600)) − 1 = 51 → actual 9615 baud, error 0.16% ✓
- TXSTA: TXEN=1, BRGH=1, SYNC=0
- RCSTA: SPEN=1, CREN=1

---

## Pin Assignment (final, no conflicts)

```
RA0/AN0  → Soil Moisture (ADC Ch0, analog)
RA1/AN1  → UNUSED (analog, never read)
RA2/AN2  → ACS712 Current (ADC Ch2, analog)
RB0      → DHT11 DATA (bidirectional, managed by Humidity.c)
RB1      → HC-SR04 Echo (digital input)
RB2      → Buzzer via NPN (digital output)
RB3      → Limit Switch (digital input, pull-up, active LOW)
RB4      → E-Stop button (digital input, pull-up, active LOW)
RC0      → STEP_X (digital output)
RC1      → DIR_X (digital output)
RC2      → ENABLE_X (LOW=enabled, HIGH=disabled)
RC3      → HC-SR04 Trigger (digital output)
RC4      → Fan via NPN transistor (digital output, active HIGH)
RC5      → UNUSED (digital output, hold LOW)
RC6      → UART TX to Pi (PIC→Pi)
RC7      → UART RX from Pi (Pi→PIC)
RD0      → Pump relay (active LOW: LOW=ON, HIGH=OFF)
RD1      → Spare relay (keep HIGH)
RD2      → LCD RS
RD3      → LCD Enable
RD4–RD7  → LCD D4–D7 (4-bit mode)
RE1      → Yellow LED — Warning
RE2      → Red LED — Fault/Lockout
```

---

## Files to Create (inside `pic-firmware/firmware/`)

```
firmware/
  config.h
  main.c
  adc.c        adc.h
  lcd.c        lcd.h
  ultrasonic.c ultrasonic.h
  button.c     button.h
  motor.c      motor.h
  safety.c     safety.h
  irrigation.c irrigation.h
  uart.c       uart.h
  comms.c      comms.h
  .vscode/tasks.json
  README.md
```

Compile command:

```bash
xc8-cc -mcpu=16F877A -O1 -o output.hex \
  main.c adc.c safety.c lcd.c button.c motor.c \
  irrigation.c ultrasonic.c uart.c comms.c
```

---

## What Each File Does

### `config.h`

All `#define`s. Included FIRST in every `.c` before `<xc.h>`.

```c
#define _XTAL_FREQ  8000000UL

// Mode flags
#define MODE_AUTO    0u
#define MODE_MANUAL  1u

// UART
#define UART_BAUD           9600u
#define UART_SPBRG_VAL      51u     // (8MHz / (16×9600)) - 1
#define UART_HDR_PI_TO_PIC  0xBBu
#define UART_HDR_PIC_TO_PI  0xAAu
#define UART_CMD_MODE       0x01u
#define UART_CMD_IRRIGATE   0x02u
#define UART_CMD_ESTOP      0x03u
#define UART_TYPE_SENSORS   0x01u
#define UART_TYPE_AT_PLANT  0x02u
#define UART_TYPE_STATUS    0x03u

// ADC
#define ADCON1_CONFIG    0x04u
#define ADC_CH_SOIL      0u
#define ADC_CH_CURRENT   2u
#define DRY_ADC          614u
#define WET_ADC          245u

// Safety thresholds
#define SOIL_WARN_PCT    80u
#define SOIL_ACTION_PCT  90u
#define SOIL_CLEAR_PCT   75u
#define SOIL_SKIP_PCT    70u
#define TEMP_WARN_C      55u
#define TEMP_ACTION_C    70u
#define TEMP_CLEAR_C     50u
#define CURR_WARN_MA     3500u
#define CURR_ACTION_MA   4500u
#define CURR_CLEAR_MA    3000u
#define WATER_WARN_CM    5u
#define WATER_ACTION_CM  2u
#define WATER_CLEAR_CM   6u
#define TANK_HEIGHT_CM   30u

// Irrigation
#define NUM_PLANTS            5u
#define PUMP_ON_TICKS         50u   // 50×100ms = 5s
#define SETTLE_LOOPS          3u    // 3×__delay_ms(100) = 300ms
#define HOMING_BACKOFF_STEPS  200u

// Timing (1 tick = 100ms)
#define SENSOR_PERIOD_TICKS   20u   // 2s
#define IRRIG_PERIOD_TICKS    1800u // 3min

// Motor
#define STEP_DELAY_NORMAL_MS  2u    // ~500 steps/s
#define STEP_DELAY_HOMING_MS  8u    // ~125 steps/s

// HC-SR04
#define ECHO_US_PER_LOOP    2u
#define ECHO_TIMEOUT_LOOPS  12500u

// Plant positions (microsteps from home)
// {4000, 12000, 20000, 28000, 36000}  (10cm spacing, 50mm first offset)
#define NUM_PLANT_POSITIONS 5u

// Pins
#define PIN_TRIG    3   // RC3 - HC-SR04 trigger
#define PIN_ECHO    1   // RB1 - HC-SR04 echo
#define PIN_BUZZ    2   // RB2 - buzzer
#define PIN_LIMIT   3   // RB3 - limit switch
#define PIN_ESTOP   4   // RB4 - emergency stop
#define PIN_STEP    0   // RC0 - stepper STEP
#define PIN_DIR     1   // RC1 - stepper DIR
#define PIN_ENABLE  2   // RC2 - stepper ENABLE
#define PIN_FAN     4   // RC4 - cooling fan
#define PIN_PUMP    0   // RD0 - pump relay (active LOW)
#define PIN_SPARE   1   // RD1 - spare relay (keep HIGH)
#define PIN_RS      2   // RD2 - LCD RS
#define PIN_LCDEN   3   // RD3 - LCD Enable
#define PIN_LED_YLW 1   // RE1 - yellow/amber LED
#define PIN_LED_RED 2   // RE2 - red LED
```

---

### `uart.c / uart.h`

Hardware USART driver. Non-blocking RX (poll PIR1.RCIF), blocking TX.

```c
void UART_Init(void);
void UART_SendByte(u8 byte);
u8   UART_DataAvailable(void);   // returns 1 if RCREG has data
u8   UART_ReadByte(void);        // read RCREG (call only if DataAvailable)
void UART_SendBuffer(const u8 *buf, u8 len);
```

Init: TXSTA = 0x24 (TXEN=1, BRGH=1, SYNC=0), RCSTA = 0x90 (SPEN=1, CREN=1), SPBRG = UART_SPBRG_VAL.
No interrupts — RX polled via UART_DataAvailable() in main loop.

---

### `comms.c / comms.h`

Packet parser + sender. Sits on top of uart.c.

```c
// Outbound (PIC → Pi)
void Comms_SendSensors(u8 soil, u8 temp, u8 hum, u16 curr, u8 water);
void Comms_SendAtPlant(u8 plant_index);      // triggers Pi image capture
void Comms_SendStatus(u8 mode, u8 lockout);

// Inbound (Pi → PIC) — call every main loop tick
void Comms_Poll(void);

// Results of last parsed command (read after Comms_Poll)
u8   Comms_GetMode(void);
u8   Comms_ManualCommandPending(void);
u8   Comms_GetManualPlant(void);
u8   Comms_AppEstopActive(void);
void Comms_ClearManualCommand(void);
```

**Packet parsing state machine (inside Comms_Poll):**

```
state 0: wait for 0xBB header byte
state 1: read CMD byte
state 2: read DATA byte → dispatch command, go to state 0
```

Non-blocking: each call reads at most 1 byte. 3-byte packet assembles over ≤3 ticks (≤300ms).

---

### `adc.c / adc.h`

```c
void ADC_Init(void);
u16  ADC_Read(u8 channel);
u8   ADC_SoilPct(u16 raw);    // 100 - ((raw-WET)*100/(DRY-WET)), clamped 0-100
u16  ADC_CurrentmA(u16 raw);  // abs((s16)raw - 512) * 26
```

ADCON1=0x04, clock Fosc/32 (TAD=4µs @ 8MHz ✓), 20µs acquisition delay per read.

---

### `lcd.c / lcd.h`

HD44780 4-bit. Shadow register `portd_shadow` (extern, defined in main.c) used for ALL PORTD writes to preserve relay pin states on RD0/RD1.

```c
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(u8 row, u8 col);
void LCD_WriteChar(u8 c);
void LCD_WriteString(const char *str);
void LCD_WriteNumber(s16 num);
```

Every nibble write:

```c
portd_shadow = (portd_shadow & 0x0F) | (nibble & 0xF0);
PORTD = portd_shadow;
```

---

### `ultrasonic.c / ultrasonic.h`

RC3=trigger, RB1=echo. Median of 3 readings. Hard timeout — never hangs.

```c
u8 Ultrasonic_GetWaterLevel(void);  // returns cm; 0xFF on timeout/error
```

distance_cm = (echo_loop_count × ECHO_US_PER_LOOP) / 58
water_cm = TANK_HEIGHT_CM − distance_cm

---

### `button.c / button.h`

RB4 e-stop, 300ms debounce, falling-edge toggle. First press = lockout. Second press = resume.

```c
void Button_Init(void);
void Button_Poll(void);
u8   Button_IsEstopped(void);
```

---

### `motor.c / motor.h`

A4988 stepper. Step pulse = `__delay_us(2)` on/off. Normal=`__delay_ms(2)` per step, homing=`__delay_ms(8)` per step.

```c
void Motor_Home(void);           // drive toward RB3, back off 200 steps, position=0
void Motor_MoveTo(u8 plant_idx); // move to plant_positions[idx]
void Motor_Disable(void);        // RC2=HIGH
```

`static s32 current_position = 0;` — tracks microstep position globally.
Button polled every 50 steps inside move loops.

Plant positions array (microsteps): `{4000, 12000, 20000, 28000, 36000}`

---

### `safety.c / safety.h`

```c
void Safety_Init(void);
void Safety_RunChecks(u8 soil_pct, u8 temp_c, u16 curr_mA, u8 water_cm);
u8   Safety_IsLocked(void);
```

4 checks, each with exactly 2 levels. Temperature check also controls fan on RC4.

**Temperature (fan controlled here):**

```
temp >= TEMP_ACTION_C (70°C):
    Fan ON (RC4=1), red LED on, yellow LED off
    buzzer continuous, pump OFF, stepper OFF
    safety_lockout = 1
    LCD line 2: "LOCKOUT! Overheat"

temp >= TEMP_WARN_C (55°C):
    Fan ON (RC4=1), yellow LED on
    buzzer single beep
    LCD line 2: "Warn: High Temp"

temp < TEMP_WARN_C (warning → normal):
    Fan OFF (RC4=0), yellow LED off

temp < TEMP_CLEAR_C (50°C) while locked AND all other sensors clear:
    Fan OFF (RC4=0), lockout releases
```

**Dry-run (water level):**

```
water < WATER_ACTION_CM (2cm): red LED, pump OFF, lockout
water < WATER_WARN_CM  (5cm): yellow LED, beep
water > WATER_CLEAR_CM (6cm): clears
```

**Overwatering (soil):**

```
soil > SOIL_ACTION_PCT (90%): red LED, pump OFF, lockout
soil > SOIL_WARN_PCT   (80%): yellow LED, beep
soil < SOIL_CLEAR_PCT  (75%): clears
```

**Overcurrent:**

```
curr > CURR_ACTION_MA (4500mA): red LED, pump+stepper OFF, lockout
curr > CURR_WARN_MA   (3500mA): yellow LED, beep
curr < CURR_CLEAR_MA  (3000mA): clears
```

**Lockout clear condition:** ALL four sensors simultaneously below their clear thresholds.

---

### `irrigation.c / irrigation.h`

```c
void Irrigation_RunCycle(void);        // auto mode: all plants, soil skip check first
void Irrigation_RunSinglePlant(u8 i);  // manual mode: one plant, no skip check
```

**`Irrigation_RunCycle()` flow:**

```
1. Read soil. If >= SOIL_SKIP_PCT (70%): LCD "Soil OK-Skip", send status, return.

2. LCD "Homing...", Motor_Home()
   if estopped || app-estopped: return

3. for i = 0..NUM_PLANTS-1:
   a. Motor_MoveTo(i)
      if estopped || locked: goto abort
   b. Settle: __delay_ms(100) × 3
   c. Comms_SendAtPlant(i)      ← Pi captures image, runs CV, uploads to Firebase
   d. Pump ON via portd_shadow
   e. LCD "Watering P[i]"
   f. pump_ticks = 0
      while pump_ticks < PUMP_ON_TICKS (50):
          __delay_ms(100)
          Button_Poll()
          Comms_Poll()           ← catch app e-stop mid-pump
          soil = ADC_SoilPct(ADC_Read(0))
          curr = ADC_CurrentmA(ADC_Read(2))
          water = Ultrasonic_GetWaterLevel()
          Safety_RunChecks(soil, last_temp_c, curr, water)
          if locked || estopped: goto abort_pump
          pump_ticks++
   g. Pump OFF, LCD "Done P[i]", __delay_ms(100)×5

4. Motor_Home(), Motor_Disable()
   Comms_SendSensors(...)    ← final report to Pi

abort_pump: Pump OFF immediately
abort:      Motor_Disable(), return
```

Note: `last_temp_c` is a module-level extern updated by main loop every 2s. DHT11 not called inside pump loop (takes 20ms — too slow for 100ms iterations).

**`Irrigation_RunSinglePlant(u8 i)` flow (manual mode):**

```
1. LCD "Manual: P[i]"
2. Motor_Home()
3. Motor_MoveTo(i)
   if estopped || locked: goto abort
4. Settle (3×__delay_ms(100))
5. Comms_SendAtPlant(i)     ← Pi takes image in manual mode too
6. Pump ON
7. Same inner safety+poll loop as above
8. Pump OFF, Motor_Home(), Motor_Disable()
9. Comms_SendStatus(MODE_MANUAL, Safety_IsLocked())
```

---

### `main.c` — Detailed

**Config bits:**

```c
#pragma config FOSC=HS, WDTE=OFF, PWRTE=ON, BOREN=ON
#pragma config LVP=OFF    // CRITICAL: frees RB3 for limit switch input
#pragma config CPD=OFF, WRT=OFF, CP=OFF
```

**Init sequence:**

```c
ADCON1 = 0x04;             // RA0,RA1,RA2 analog; rest digital
TRISA  = 0x07;             // RA0,RA1,RA2 input; RA3-5 output (unused)
TRISB  = 0b00011010;       // RB1=in(echo), RB3=in(limit), RB4=in(estop)
                           // RB0 left as input; DHT11 driver manages it
                           // RB2=out(buzzer), RB5-7=out(unused)
TRISC  = 0b10000000;       // RC7=in(UART RX); RC0-RC6=out
TRISD  = 0x00;             // all output (LCD + relays)
TRISE  = 0x00;             // RE1,RE2 output (LEDs)
OPTION_REG &= 0x7F;        // bit7=0: enable PORTB pull-ups (RB3, RB4)

portd_shadow = 0xFF;       // RD0=HIGH(pump off), RD1=HIGH(spare off)
PORTD = portd_shadow;
PORTCbits.RC2 = 1;         // stepper disabled
PORTCbits.RC3 = 0;         // HC-SR04 trigger idle LOW
PORTCbits.RC4 = 0;         // fan off
PORTCbits.RC6 = 1;         // UART TX idle HIGH
PORTBbits.RB2 = 0;         // buzzer off
PORTEbits.RE1 = 0;         // yellow LED off
PORTEbits.RE2 = 0;         // red LED off

ADC_Init();
UART_Init();
LCD_Init();
Safety_Init();
Button_Init();

// Splash screen
LCD_SetCursor(1,1); LCD_WriteString("Smart Irrigation");
LCD_SetCursor(2,1); LCD_WriteString("Initializing... ");
__delay_ms(100); __delay_ms(100); /* ×15 total = 1.5s */
LCD_Clear();
Comms_SendStatus(MODE_AUTO, 0);   // announce ready to Pi
```

**Main loop (~100ms per tick, pure polling):**

```c
u16 tick        = 0;
u8  lcd_page    = 0;
u8  current_mode = MODE_AUTO;
u8  soil_pct    = 0;
u8  temp_c      = 25;   // last known; updated every 2s
u8  humidity    = 50;   // last known; updated every 2s
u16 curr_mA     = 0;
u8  water_cm    = 20;

while(1) {
    __delay_ms(100);
    tick++;

    // ── STEP 1: Physical e-stop (every tick, highest priority) ──
    Button_Poll();

    // ── STEP 2: UART commands from Pi/app (every tick, non-blocking) ──
    Comms_Poll();

    u8 new_mode = Comms_GetMode();
    if (new_mode != current_mode) {
        current_mode = new_mode;
        Comms_SendStatus(current_mode, Safety_IsLocked());
    }

    if (current_mode == MODE_MANUAL && Comms_ManualCommandPending()) {
        u8 plant = Comms_GetManualPlant();
        Comms_ClearManualCommand();
        if (!Safety_IsLocked() && !Button_IsEstopped() && !Comms_AppEstopActive()) {
            Irrigation_RunSinglePlant(plant);
        }
    }

    // ── STEP 3: Sensor read + safety (every 2s = every 20 ticks) ──
    if (tick % SENSOR_PERIOD_TICKS == 0) {
        soil_pct = ADC_SoilPct(ADC_Read(ADC_CH_SOIL));
        curr_mA  = ADC_CurrentmA(ADC_Read(ADC_CH_CURRENT));
        water_cm = Ultrasonic_GetWaterLevel();
        Humidity_Read(&humidity, &temp_c);  // ~20ms blocking; keeps last value on error

        Safety_RunChecks(soil_pct, temp_c, curr_mA, water_cm);
        Comms_SendSensors(soil_pct, temp_c, humidity, curr_mA, water_cm);

        // LCD line 1: rotate through 5 sensor pages
        LCD_SetCursor(1,1);
        switch(lcd_page % 5) {
            case 0: LCD_WriteString("Soil:"); LCD_WriteNumber(soil_pct); LCD_WriteString("%   "); break;
            case 1: LCD_WriteString("Temp:"); LCD_WriteNumber(temp_c);   LCD_WriteString("C   "); break;
            case 2: LCD_WriteString("Hum: "); LCD_WriteNumber(humidity); LCD_WriteString("%   "); break;
            case 3: LCD_WriteString("Curr:"); LCD_WriteNumber((s16)curr_mA); LCD_WriteString("mA "); break;
            case 4: LCD_WriteString("Watr:"); LCD_WriteNumber(water_cm); LCD_WriteString("cm  "); break;
        }
        lcd_page++;

        // LCD line 2: system status
        LCD_SetCursor(2,1);
        if      (Button_IsEstopped() || Comms_AppEstopActive())
                    LCD_WriteString("E-STOP ACTIVE   ");
        else if (Safety_IsLocked())
                    LCD_WriteString("LOCKOUT!        ");
        else if (current_mode == MODE_MANUAL)
                    LCD_WriteString("Manual Mode     ");
        else        LCD_WriteString("Auto Mode OK    ");
    }

    // ── STEP 4: Auto irrigation (every 3min = 1800 ticks, auto mode only) ──
    if (current_mode == MODE_AUTO && tick >= IRRIG_PERIOD_TICKS) {
        tick = 0;
        if (!Safety_IsLocked() && !Button_IsEstopped() && !Comms_AppEstopActive()) {
            Irrigation_RunCycle();
        }
    }

    if (current_mode == MODE_MANUAL && tick >= IRRIG_PERIOD_TICKS) {
        tick = 0;   // prevent overflow in manual mode
    }
}
```

**Loop timing summary:**

| Step                 | Frequency   | Reason                              |
| -------------------- | ----------- | ----------------------------------- |
| Button_Poll()        | Every 100ms | Physical e-stop must be fast        |
| Comms_Poll()         | Every 100ms | 1 byte/tick; full packet in ≤300ms  |
| Sensor read + safety | Every 2s    | DHT11 min 1s interval; sensors slow |
| Auto irrigation      | Every 3min  | Course requirement                  |

**Mode behavior:**

- **AUTO:** timer fires every 3min → soil check → irrigate all plants with image capture at each
- **MANUAL:** timer disabled; each app command irrigates exactly one plant with image capture
- **Both:** safety checks always run; image capture always happens when motor reaches a plant

---

## Data Flow — Manual Mode Example

```
App: user taps "Irrigate Plant 2" in manual mode
  → Firebase updated
  → Pi reads Firebase → sends UART: 0xBB 0x02 0x02
  → PIC Comms_Poll() receives packet over ≤3 ticks
  → ManualCommandPending=1, plant=2
  → Irrigation_RunSinglePlant(2) called
      Motor_Home() → Motor_MoveTo(2)
      Comms_SendAtPlant(2) → UART: 0xAA 0x02 0x02
  → Pi receives signal → camera captures plant 2 image
  → CV model runs → "Healthy" or "Sick"
  → Pi uploads image + status to Firebase
  → App shows image + health status
      PIC: pump ON 5s → pump OFF
      Motor_Home() → Motor_Disable()
      Comms_SendStatus(MANUAL, 0) → Pi → Firebase → app
```

---

## Existing Files to Reuse

| File                      | How                                                |
| ------------------------- | -------------------------------------------------- |
| `HAL/Humidity/Humidity.c` | Call `Humidity_Read()` directly from main.c        |
| `HAL/LCD/LCD.c`           | Copy logic into `firmware/lcd.c`, add portd_shadow |
| `MCAL/ADC/ADC.c`          | Copy channel-select + read into `firmware/adc.c`   |

## Existing Files to Flag (keep, don't delete)

| File                         | Issue                                                                |
| ---------------------------- | -------------------------------------------------------------------- |
| `HAL/Relay/Relay.c`          | Active HIGH; new firmware uses active LOW via portd_shadow           |
| `APP/MyProject.c`            | Sensor test demo; superseded by new main.c                           |
| `MCAL/MCU_Registers.h`       | BUZZER mapped to RD1 (conflict with spare relay); LED on RC1 (wrong) |
| `APP/Safety/Safety_config.h` | Missing dry-run check; thresholds differ from spec                   |

---

## Verification

```bash
cd pic-firmware/firmware
"C:\Program Files\Microchip\xc8\v2.36\bin\xc8-cc.exe" -mcpu=16F877A -O1 -o output.hex \
  main.c adc.c safety.c lcd.c button.c motor.c irrigation.c ultrasonic.c uart.c comms.c
```

Expected: 0 errors, advisory 2049 only (C90 library notice — normal for XC8 free mode).
