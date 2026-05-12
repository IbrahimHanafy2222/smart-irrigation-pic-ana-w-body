/*
 * Button.c
 * Physical e-stop on RB4 (active LOW, PORTB pull-up).
 * Poll every 100 ms. Debounce = 3 consecutive LOW reads (300 ms).
 * Toggle: first press = ESTOP ON (kills pump + stepper); second press = ESTOP OFF.
 */

#include "../../config.h"
#include "Button_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

extern volatile u8 portd_shadow;

static u8 estop_active    = 0u;
static u8 debounce_count  = 0u;
static u8 last_state      = 1u;   /* 1 = released (pull-up HIGH) */

void Button_Init(void)
{
    SET_BIT(TRISB, PIN_ESTOP);   /* RB4 input */
    estop_active   = 0u;
    debounce_count = 0u;
    last_state     = 1u;
}

void Button_Poll(void)
{
    u8 current = (u8)GET_BIT(PORTB, PIN_ESTOP);   /* 1=released, 0=pressed */

    if(current == 0u) {
        if(debounce_count < 3u) debounce_count++;
    } else {
        debounce_count = 0u;
    }

    /* Falling edge after 300 ms debounce */
    if(debounce_count == 3u && last_state == 1u) {
        estop_active ^= 1u;
        if(estop_active) {
            /* Immediately safe: pump OFF, motor relay OFF, stepper DISABLED */
            portd_shadow |= (u8)(1u << PIN_PUMP);
            portd_shadow |= (u8)(1u << PIN_MOTOR);
            PORTD = portd_shadow;
            SET_BIT(PORTC, PIN_ENABLE);
        }
        last_state = 0u;
    }

    if(current == 1u) last_state = 1u;
}

u8 Button_IsEstopped(void) { return estop_active; }
