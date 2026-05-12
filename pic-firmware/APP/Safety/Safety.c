/*
 * Safety.c
 * 4-feature safety monitor: dry-run, overwater, overcurrent, overheat.
 * Lockout sets on ANY action-level event; clears only when ALL 4 sensors
 * simultaneously fall below their hysteresis thresholds.
 * Temperature also drives the cooling fan on RC4.
 */

#include "../../config.h"
#include "Safety_interface.h"
#include "../Comms/Comms_interface.h"
#include "../../HAL/LED/LED_interface.h"
#include "../../HAL/LCD/LCD_interface.h"
#include "../../HAL/Buzzer/Buzzer_interface.h"
#include "../../HAL/Fan/Fan_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../MCAL/GPIO/GPIO_interface.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

extern volatile u8 portd_shadow;

static u8 safety_lockout = 0u;

/* ── Per-sensor warn flags (not lockout, just state memory) ── */
static u8 water_warn   = 0u;
static u8 soil_warn    = 0u;
static u8 curr_warn    = 0u;
static u8 temp_warn    = 0u;

static void led_yellow_on(void)  { SetPinValue(LED_YELLOW_PORT, LED_YELLOW_PIN, 1u); }
static void led_yellow_off(void) { SetPinValue(LED_YELLOW_PORT, LED_YELLOW_PIN, 0u); }
static void led_red_on(void)     { SetPinValue(LED_RED_PORT,    LED_RED_PIN,    1u); }
static void led_red_off(void)    { SetPinValue(LED_RED_PORT,    LED_RED_PIN,    0u); }

static void pump_off(void)
{
    portd_shadow |= (u8)(1u << PIN_PUMP);
    PORTD = portd_shadow;
}

static void motor_off(void)
{
    SET_BIT(PORTC, PIN_ENABLE);           /* A4988 ENABLE HIGH = disabled */
    portd_shadow |= (u8)(1u << PIN_MOTOR);
    PORTD = portd_shadow;
}

void Safety_Init(void)
{
    safety_lockout = 0u;
    water_warn = soil_warn = curr_warn = temp_warn = 0u;
    led_yellow_off();
    led_red_off();
    Fan_Off();
}

void Safety_RunChecks(u8 soil_pct, u8 temp_c, u16 curr_mA, u8 water_cm)
{
    u8 any_action = 0u;

    /* ── 1. Dry-run (water level too low) ── */
    if(water_cm != 0xFFu) {
        if(water_cm <= WATER_ACTION_CM) {
            any_action = 1u;
            pump_off();
            water_warn = 2u;
            LCD_GoToRowCol(2u, 1u);
            LCD_SendString_Const("LOCKOUT!LowWater");
        } else if(water_cm <= WATER_WARN_CM) {
            if(water_warn < 1u) Buzzer_BeepN(1u, 100u, 0u);
            water_warn = 1u;
            LCD_GoToRowCol(2u, 1u);
            LCD_SendString_Const("Warn: Low Water ");
        } else if(water_cm >= WATER_CLEAR_CM) {
            water_warn = 0u;
        }
    }

    /* ── 2. Overwatering ── */
    if(soil_pct >= SOIL_ACTION_PCT) {
        any_action = 1u;
        pump_off();
        soil_warn = 2u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("LOCKOUT!Overwter");
    } else if(soil_pct >= SOIL_WARN_PCT) {
        if(soil_warn < 1u) Buzzer_BeepN(1u, 100u, 0u);
        soil_warn = 1u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("Warn: Soil Wet  ");
    } else if(soil_pct <= SOIL_CLEAR_PCT) {
        soil_warn = 0u;
    }

    /* ── 3. Overcurrent ── */
    if(curr_mA >= CURR_ACTION_MA) {
        any_action = 1u;
        pump_off();
        motor_off();
        curr_warn = 2u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("LOCKOUT!OverCurr");
    } else if(curr_mA >= CURR_WARN_MA) {
        if(curr_warn < 1u) Buzzer_BeepN(1u, 200u, 0u);
        curr_warn = 1u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("Warn: High Curr ");
    } else if(curr_mA <= CURR_CLEAR_MA) {
        curr_warn = 0u;
    }

    /* ── 4. Overheat (also controls fan) ── */
    if(temp_c >= TEMP_ACTION_C) {
        any_action = 1u;
        pump_off();
        motor_off();
        Fan_On();
        temp_warn = 2u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("LOCKOUT!Overheat");
    } else if(temp_c >= TEMP_WARN_C) {
        Fan_On();
        if(temp_warn < 1u) Buzzer_BeepN(1u, 150u, 0u);
        temp_warn = 1u;
        LCD_GoToRowCol(2u, 1u);
        LCD_SendString_Const("Warn: High Temp ");
    } else if(temp_c <= TEMP_CLEAR_C) {
        temp_warn = 0u;
    }

    /* ── Lockout state machine ── */
    if(any_action) {
        safety_lockout = 1u;
        led_red_on();
        led_yellow_off();
        Buzzer_BeepN(2u, 300u, 100u);
    } else if(safety_lockout) {
        /* Clear only when ALL 4 sensors simultaneously below clear thresholds */
        if(water_warn == 0u && soil_warn == 0u && curr_warn == 0u && temp_warn == 0u) {
            safety_lockout = 0u;
            Fan_Off();
            led_red_off();
        }
    } else {
        /* Warn-level: yellow LED if any warn active */
        if(water_warn || soil_warn || curr_warn || temp_warn) {
            led_yellow_on();
        } else {
            led_yellow_off();
            Fan_Off();
        }
    }
}

u8 Safety_IsLocked(void) { return safety_lockout; }
