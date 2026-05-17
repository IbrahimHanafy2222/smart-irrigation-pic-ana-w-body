/*
 * Motor.c
 * A4988 stepper driver, 1/16 microstepping.
 * RC0=STEP, RC1=DIR (HIGH=away from home), RC2=ENABLE (LOW=on).
 * RB3=limit switch (active LOW) marks home position.
 * Motor relay (RD1) removed — single-channel relay board, pump only.
 *
 * Speed note: __delay_us() with a compile-time constant generates an
 * efficient NOP loop. The old variable-loop approach had ~5-10x overhead
 * in XC8 free mode, making 500 "µs" delays actually ~5 ms.
 * Two inline functions (normal / homing) with fixed delays fix this.
 */

#include "../../config.h"
#include "Motor_interface.h"
#include "../Button/Button_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

static s32 current_position = 0;

/* 2 plants, 10 cm apart.
   Plant 0:  4000 steps (~5 cm from home)
   Plant 1: 12000 steps (10 cm further)       */
static const u32 plant_positions[NUM_PLANTS] = {4000u, 12000u};

void Motor_Init(void)
{
    CLR_BIT(PORTC, PIN_STEP);
    CLR_BIT(PORTC, PIN_DIR);
    SET_BIT(PORTC, PIN_ENABLE);   /* A4988 disabled at startup */
    current_position = 0;
}

/* Compile-time constant delays — efficient NOP loops, no loop overhead.
   Tune STEP_DELAY_NORMAL_US / STEP_DELAY_HOMING_US in config.h.
   At 8 MHz: max reliable speed with NEMA17 + no load ≈ 200 µs/step.
   Add 100 µF + 100 nF caps on VMOT-GND for stability above 500 steps/s. */
static void step_once_normal(void)
{
    SET_BIT(PORTC, PIN_STEP);
    __delay_us(2);
    CLR_BIT(PORTC, PIN_STEP);
    __delay_us(STEP_DELAY_NORMAL_US);
}

static void step_once_homing(void)
{
    SET_BIT(PORTC, PIN_STEP);
    __delay_us(2);
    CLR_BIT(PORTC, PIN_STEP);
    __delay_us(STEP_DELAY_HOMING_US);
}

void Motor_Home(void)
{
    u32 backoff;
    u16 poll_cnt = 0u;

    CLR_BIT(PORTC, PIN_ENABLE);   /* enable A4988 */
    CLR_BIT(PORTC, PIN_DIR);      /* DIR LOW = toward home */

    while(GET_BIT(PORTB, PIN_LIMIT))
    {
        step_once_homing();
        poll_cnt++;
        if((poll_cnt & 0x31u) == 0u) {
            Button_Poll();
            if(Button_IsEstopped()) { Motor_Disable(); return; }
        }
    }

    /* Back off from limit switch */
    SET_BIT(PORTC, PIN_DIR);
    for(backoff = 0u; backoff < HOMING_BACKOFF_STEPS; backoff++) {
        step_once_homing();
    }

    current_position = 0;
    CLR_BIT(PORTC, PIN_DIR);
}

void Motor_MoveTo(u8 plant_index)
{
    s32 target, delta;
    u32 steps;
    u16 poll_cnt = 0u;

    if(plant_index >= NUM_PLANTS) return;

    target = (s32)plant_positions[plant_index];
    delta  = target - current_position;
    steps  = (delta < 0) ? (u32)(-delta) : (u32)delta;

    CLR_BIT(PORTC, PIN_ENABLE);

    if(delta > 0) SET_BIT(PORTC, PIN_DIR);
    else          CLR_BIT(PORTC, PIN_DIR);

    while(steps > 0u)
    {
        step_once_normal();
        steps--;
        current_position += (delta > 0) ? 1 : -1;
        poll_cnt++;
        if((poll_cnt % 50u) == 0u) {
            Button_Poll();
            if(Button_IsEstopped()) { Motor_Disable(); return; }
        }
    }
}

void Motor_Disable(void)
{
    SET_BIT(PORTC, PIN_ENABLE);   /* A4988 ENABLE HIGH = free shaft */
}
