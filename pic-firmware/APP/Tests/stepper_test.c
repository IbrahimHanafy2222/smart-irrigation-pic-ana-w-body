/*
 * stepper_test.c
 * Standalone A4988 stepper motor test — PIC16F877A @ 8 MHz
 *
 * Rotates motor exactly ONE full revolution CW then stops.
 * Hardware: MS1=MS2=MS3=HIGH (hardwired) → 1/16 microstep → 3200 steps/rev.
 *
 * Compile separately — do NOT include in main irrigation build.
 * XC8: xc8-cc -mcpu=16F877A stepper_test.c -o stepper_test.hex
 *
 * Wiring:
 *   RC0 → A4988 STEP
 *   RC1 → A4988 DIR   (HIGH = CW / away from home)
 *   RC2 → A4988 ENABLE (LOW = enabled)
 *   MS1/MS2/MS3 → all HIGH (hardwired, 1/16 microstepping)
 *   RESET/SLEEP → +5V (hardwired)
 *   VMOT → 12V, VDD → 5V, GND common
 */

#include "../../config.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

#pragma config FOSC  = HS
#pragma config WDTE  = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP   = OFF
#pragma config CPD   = OFF
#pragma config WRT   = OFF
#pragma config CP    = OFF

/* 1/16 microstepping: 200 full steps × 16 = 3200 microsteps per revolution */
#define STEPS_ONE_REV   3200u
#define STEP_DELAY_MS      2u   /* 2 ms/step → ~500 steps/s → ~6.4 s/rev   */

static void step_pulse(void)
{
    u8 i;
    SET_BIT(PORTC, PIN_STEP);
    __delay_us(2);
    CLR_BIT(PORTC, PIN_STEP);
    __delay_us(2);
    for(i = 0u; i < STEP_DELAY_MS; i++) { __delay_ms(1); }
}

void main(void)
{
    u16 s;

    /* All PORTA/E pins digital */
    ADCON1 = 0x06u;

    /* Motor pins → output; RC7 = UART RX kept input (safe even unused) */
    TRISC = 0b10000000u;

    /* Safe initial state: STEP=0, DIR=0, ENABLE=1 (disabled) */
    CLR_BIT(PORTC, PIN_STEP);
    CLR_BIT(PORTC, PIN_DIR);
    SET_BIT(PORTC, PIN_ENABLE);

    /* 100 ms startup — let A4988 power up */
    __delay_ms(100);

    /* Enable driver */
    CLR_BIT(PORTC, PIN_ENABLE);
    __delay_ms(5);

    /* CW direction — away from home */
    SET_BIT(PORTC, PIN_DIR);
    __delay_us(5);

    /* One full revolution */
    for(s = 0u; s < STEPS_ONE_REV; s++)
    {
        step_pulse();
    }

    /* Disable driver — release holding torque */
    SET_BIT(PORTC, PIN_ENABLE);

    /* Halt */
    while(1) {}
}
