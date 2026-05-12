/*
 * Timer0.c
 * Irrigation Project — Timer0 Driver
 * Target: PIC16F877A | MikroC Pro for PIC
 *
 * Config: Internal clock, Prescaler = 256 (PS2:PS0 = 111)
 *
 * At 8 MHz (matching MikroC project settings):
 *   Tick period = (4 / 8MHz) * 256 = 128µs
 *   Ticks/sec   = 7,812
 *   For 100ms   = 781 ticks → overflows=3, remainder=13, jump=243
 */

#include "../../SERVICES/BIT_MATH.h"
#include "Timer0_interface.h"
#include "../MCU_Registers.h"

static void (*TMR0_Callback)(void) = 0;

static u32 overflows_needed  = 0;
static u8  jump_value        = 0;
static u32 current_overflows = 0;

/* ─── TMR0_Init ─── */
void TMR0_Init(void)
{
    CLR_BIT(OPTION_REG, T0CS);   /* Internal clock source     */
    CLR_BIT(OPTION_REG, PSA);    /* Prescaler assigned to TMR0*/
    SET_BIT(OPTION_REG, PS2);    /* PS2:PS0 = 111 → div 256   */
    SET_BIT(OPTION_REG, PS1);
    SET_BIT(OPTION_REG, PS0);
    CLR_BIT(INTCON, TMR0IF);     /* Clear overflow flag        */
}

/* ─── Enable / Disable / Reset ─── */
void TMR0_Enable(void)  { SET_BIT(INTCON, TMR0IE); }
void TMR0_Disable(void) { CLR_BIT(INTCON, TMR0IE); }
void TMR0_Reset(void)   { TMR0 = 0; }

/* ─── SetInterval_s: whole seconds ─── */
void TMR0_SetInterval_s(u8 seconds)
{
    /*
     * At 8MHz, prescaler=256: 7812 ticks/sec
     */
    u32 total_ticks  = (u32)seconds * 7812UL;
    u8  remainder    = (u8)(total_ticks % 256u);
    overflows_needed = total_ticks / 256u;
    jump_value       = (u8)(256u - remainder);
    current_overflows = 0;
}

/* ─── SetInterval_ms: milliseconds (use this for 100ms tick) ─── */
void TMR0_SetInterval_ms(u16 milliseconds)
{
    /*
     * At 8MHz, prescaler=256: 7.812 ticks/ms
     * ticks = (ms * 7812) / 1000
     * For 100ms: ticks=781, overflows=3, remainder=13, jump=243
     */
    u32 total_ticks  = ((u32)milliseconds * 7812UL) / 1000UL;
    u8  remainder    = (u8)(total_ticks % 256u);
    overflows_needed = total_ticks / 256u;
    jump_value       = (u8)(256u - remainder);
    current_overflows = 0;
}

/* ─── SetCallback ─── */
void TMR0_SetCallback(void (*ptr)(void))
{
    if(ptr != 0)
    {
        TMR0_Callback = ptr;
    }
}

/* ─── ISR ─── */
void TIMER0_ISR(void)
{
    current_overflows++;

    if(current_overflows == overflows_needed)
    {
        TMR0 = jump_value;
    }
    else if(current_overflows > overflows_needed)
    {
        current_overflows = 0;
        if(TMR0_Callback != NULL)
        {
            TMR0_Callback();
        }
    }

    CLR_BIT(INTCON, TMR0IF);
}
