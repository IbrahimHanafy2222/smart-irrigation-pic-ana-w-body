/*
 * config.h
 * Smart Irrigation — Central Configuration
 * Include FIRST in every .c file, before <xc.h>, so _XTAL_FREQ is
 * defined before any __delay_ms / __delay_us macro is encountered.
 *
 * Hardware: PIC16F877A @ 8 MHz external crystal
 * Compiler: XC8 v2.36 free mode
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ================================================================
   CRYSTAL / COMPILER
   ================================================================ */
#define _XTAL_FREQ  8000000UL   /* Required by __delay_ms/__delay_us */

/* ================================================================
   OPERATING MODES
   ================================================================ */
#define MODE_AUTO    0u
#define MODE_MANUAL  1u

/* ================================================================
   UART / COMMS PROTOCOL
   ================================================================ */
#define UART_SPBRG_VAL       51u    /* 9600 baud @ 8MHz, BRGH=1: (8MHz/153600)-1 */

#define UART_HDR_PI_TO_PIC   0xBBu  /* Header byte: Pi → PIC packets             */
#define UART_HDR_PIC_TO_PI   0xAAu  /* Header byte: PIC → Pi packets             */

/* Pi → PIC command bytes (3-byte packet: [0xBB][CMD][DATA]) */
#define UART_CMD_MODE        0x01u  /* DATA: 0x00=auto, 0x01=manual              */
#define UART_CMD_IRRIGATE    0x02u  /* DATA: 0x00–0x04 plant index               */
#define UART_CMD_ESTOP       0x03u  /* DATA: 0x01=stop, 0x00=release             */
#define UART_CMD_HANDSHAKE   0x10u  /* DATA: 0xAA — Pi signals ready             */

/* PIC → Pi type bytes (variable packet: [0xAA][TYPE][DATA...]) */
#define UART_TYPE_SENSORS    0x01u  /* 6 data bytes: soil,temp,hum,curr_h,curr_l,water */
#define UART_TYPE_AT_PLANT   0x02u  /* 1 data byte:  plant_index — "take image now"    */
#define UART_TYPE_STATUS     0x03u  /* 2 data bytes: mode, lockout                     */
#define UART_TYPE_HANDSHAKE  0x10u  /* 1 data byte:  0xBB — PIC handshake ACK          */

/* ================================================================
   ADC
   ================================================================ */
#define ADCON1_CONFIG   0x84u   /* ADFM=1 right-just; PCFG=0100: AN0-AN2 analog, RE1/RE2 digital */
#define ADC_CH_SOIL     0u      /* AN0 / RA0 — capacitive soil moisture sensor   */
#define ADC_CH_CURRENT  2u      /* AN2 / RA2 — ACS712-05B current sensor         */

/* Soil moisture sensor calibration (capacitive v1.2):
   Dry  ≈ 3.0 V → ADC raw ≈ 614
   Wet  ≈ 1.2 V → ADC raw ≈ 245
   Formula: pct = 100 - ((raw - WET_ADC) * 100 / (DRY_ADC - WET_ADC)), clamped 0-100 */
#define DRY_ADC         614u
#define WET_ADC         245u

/* ================================================================
   SAFETY THRESHOLDS
   ================================================================ */

/* Overwatering (soil moisture %) */
#define SOIL_WARN_PCT    80u    /* Warning:  amber LED + beep                    */
#define SOIL_ACTION_PCT  90u    /* Action:   red LED + pump OFF + lockout        */
#define SOIL_CLEAR_PCT   75u    /* Hysteresis clear level                        */
#define SOIL_SKIP_PCT    70u    /* Irrigation skip: soil already wet enough      */

/* Overheat (DHT11 temperature °C, integer resolution) */
#define TEMP_WARN_C      55u    /* Warning:  amber LED + beep + fan ON           */
#define TEMP_ACTION_C    70u    /* Action:   red LED + shutdown + lockout + fan  */
#define TEMP_CLEAR_C     50u    /* Hysteresis clear level (fan OFF when clear)   */

/* Overcurrent (ACS712-05B, milliamps) */
#define CURR_WARN_MA     3500u  /* Warning:  amber LED + beep                   */
#define CURR_ACTION_MA   4500u  /* Action:   red LED + pump+stepper OFF + lockout */
#define CURR_CLEAR_MA    3000u  /* Hysteresis clear level                        */

/* Dry-run / low water (HC-SR04, cm from top of tank) */
#define WATER_WARN_CM    5u     /* Warning:  amber LED + beep                   */
#define WATER_ACTION_CM  2u     /* Action:   red LED + pump OFF + lockout        */
#define WATER_CLEAR_CM   6u     /* Hysteresis clear level                        */
#define TANK_HEIGHT_CM   30u    /* Physical tank height — used in distance→level */

/* ================================================================
   IRRIGATION
   ================================================================ */
#define NUM_PLANTS            2u    /* Number of plant positions on gantry       */
#define PUMP_ON_TICKS         50u   /* Pump-on duration: 50 × 100ms = 5 s        */
#define SETTLE_MS_LOOP        3u    /* Post-move settle: 3 × __delay_ms(100)     */
#define HOMING_BACKOFF_STEPS  200u  /* Steps to back off after limit switch hit  */

/* Plant positions in microsteps from home (GT2 20T pulley, 1/16 microstepping):
   1 step = 0.0125 mm → 100 mm = 8000 steps; first plant at 50 mm = 4000 steps  */
/* Array defined in motor.c:
   const u32 plant_positions[NUM_PLANTS] = {4000, 12000, 20000, 28000, 36000};  */

/* ================================================================
   MAIN LOOP TIMING  (1 tick = __delay_ms(100) = 100 ms)
   ================================================================ */
#define SENSOR_PERIOD_TICKS   20u   /* Read sensors every 2 s  (20 × 100ms)     */
#define IRRIG_PERIOD_TICKS    3000u /* Irrigation every 5 min (3000 × 100ms)    */

/* ================================================================
   MOTOR (A4988, 1/16 microstepping, 8 MHz)
   ================================================================ */
#define STEP_DELAY_NORMAL_US  200   /* ~5000 steps/s — compile-time constant for __delay_us */
#define STEP_DELAY_HOMING_US  800   /* ~1250 steps/s homing (slower for limit switch safety) */

/* ================================================================
   HC-SR04 ULTRASONIC (RC3 trigger, RB1 echo)
   ================================================================ */
/* At 8 MHz each echo-count loop ≈ 2 µs (calibrate empirically).
   distance_cm = (count × ECHO_US_PER_LOOP) / 58
   Timeout at 25 ms: 25000 µs / 2 µs/loop = 12500 loops             */
#define ECHO_US_PER_LOOP    2u
#define ECHO_TIMEOUT_LOOPS  12500u

/* ================================================================
   PIN NUMBERS  (bit position within their respective PORT/TRIS)
   ================================================================ */

/* PORTA — analog inputs */
/* RA0 = soil (ADC_CH_SOIL), RA1 = unused, RA2 = current (ADC_CH_CURRENT) */

/* PORTB */
#define PIN_DHT11   0   /* RB0 — DHT11 data (bidirectional, Humidity.c manages) */
#define PIN_ECHO    1   /* RB1 — HC-SR04 echo input                             */
#define PIN_BUZZ    2   /* RB2 — buzzer via NPN transistor (active HIGH)        */
#define PIN_LIMIT   3   /* RB3 — limit switch (active LOW, pull-up)             */
#define PIN_ESTOP   4   /* RB4 — emergency stop button (active LOW, pull-up)    */
/* RB5-7 unused, available */

/* PORTC */
#define PIN_STEP    0   /* RC0 — A4988 STEP pulse                               */
#define PIN_DIR     1   /* RC1 — A4988 DIR (HIGH=away from home)                */
#define PIN_ENABLE  2   /* RC2 — A4988 ENABLE (LOW=enabled, HIGH=disabled)      */
#define PIN_TRIG    3   /* RC3 — HC-SR04 trigger output                         */
#define PIN_FAN     4   /* RC4 — cooling fan via NPN transistor (active HIGH)   */
/* RC5 unused */
/* RC6 = UART TX (PIC→Pi), RC7 = UART RX (Pi→PIC) — managed by USART driver   */

/* PORTD — shadow register required (LCD + relays share this port) */
#define PIN_PUMP    0   /* RD0 — pump relay   (ACTIVE LOW: LOW=ON, HIGH=OFF)    */
#define PIN_MOTOR   1   /* RD1 — motor relay REMOVED; kept to avoid compile errors in Button.c (writes to RD1 are harmless) */
#define PIN_RS      2   /* RD2 — LCD RS                                         */
#define PIN_LCDEN   3   /* RD3 — LCD Enable                                     */
/* RD4-RD7 = LCD D4-D7 (4-bit mode)                                             */

/* PORTE */
#define PIN_LED_YLW 1   /* RE1 — yellow/amber warning LED                       */
#define PIN_LED_RED 2   /* RE2 — red fault/lockout LED                          */

/* ================================================================
   FREED / AVAILABLE PINS (no keypad, LM35 removed)
   ================================================================ */
/* RA1/AN1 — was LM35, now unused (stays analog via ADCON1=0x04, never read)   */
/* RA3, RA4, RA5 — available digital outputs/inputs                            */
/* RB5, RB6, RB7 — available digital I/O                                       */
/* RC5             — available digital output                                   */
/* RE0             — available digital I/O                                      */

#endif /* CONFIG_H */
