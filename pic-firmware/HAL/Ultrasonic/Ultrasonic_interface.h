#ifndef ULTRASONIC_INTERFACE_H
#define ULTRASONIC_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/*
 * Ultrasonic_GetWaterLevel()
 *   Returns water level in cm (median of 3 pulses).
 *   Returns 0xFF on timeout or sensor error.
 *   Trigger: RC3 (PIN_TRIG).  Echo: RB1 (PIN_ECHO).
 */
u8 Ultrasonic_GetWaterLevel(void);

#endif /* ULTRASONIC_INTERFACE_H */
