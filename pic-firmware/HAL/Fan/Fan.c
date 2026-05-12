/*
 * Fan.c
 * Cooling fan on RC4 via NPN transistor (active HIGH).
 */

#include "../../config.h"
#include "Fan_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"

void Fan_Init(void)
{
    CLR_BIT(TRISC, PIN_FAN);
    CLR_BIT(PORTC, PIN_FAN);
}

void Fan_On(void)  { SET_BIT(PORTC, PIN_FAN); }
void Fan_Off(void) { CLR_BIT(PORTC, PIN_FAN); }
