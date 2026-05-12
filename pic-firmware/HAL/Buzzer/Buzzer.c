/*
 * Buzzer.c
 * Active buzzer via NPN transistor on RB2 (active HIGH).
 * Variable-duration delays use a 1ms loop — XC8 free mode requires
 * compile-time-constant arguments to __delay_ms.
 */

#include "../../config.h"
#include "Buzzer_interface.h"
#include "Buzzer_private.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

void Buzzer_Init(void)
{
    CLR_BIT(BUZZER_TRIS, BUZZER_PIN);
    CLR_BIT(BUZZER_PORT, BUZZER_PIN);
}

void Buzzer_On(void)  { SET_BIT(BUZZER_PORT, BUZZER_PIN); }
void Buzzer_Off(void) { CLR_BIT(BUZZER_PORT, BUZZER_PIN); }

void Buzzer_Beep(u16 duration_ms)
{
    u16 i;
    Buzzer_On();
    for(i = 0u; i < duration_ms; i++) { __delay_ms(1); }
    Buzzer_Off();
}

void Buzzer_BeepN(u8 count, u16 ms_on, u16 ms_off)
{
    u8  n;
    u16 i;
    for(n = 0u; n < count; n++)
    {
        Buzzer_On();
        for(i = 0u; i < ms_on;  i++) { __delay_ms(1); }
        Buzzer_Off();
        if(n < (u8)(count - 1u)) {
            for(i = 0u; i < ms_off; i++) { __delay_ms(1); }
        }
    }
}
