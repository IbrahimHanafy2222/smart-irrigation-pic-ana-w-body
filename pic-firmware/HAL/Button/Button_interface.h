#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/*
 * Physical emergency-stop button on RB4 (active LOW, PORTB pull-up).
 * Button_Poll() must be called every 100 ms main-loop tick.
 * On press: sets internal estop flag, kills pump, disables stepper.
 * On second press: clears estop flag (toggle behaviour).
 */
void Button_Init(void);
void Button_Poll(void);
u8   Button_IsEstopped(void);

#endif /* BUTTON_INTERFACE_H */
