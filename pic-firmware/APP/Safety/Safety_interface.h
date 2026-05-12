#ifndef SAFETY_INTERFACE_H
#define SAFETY_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

void Safety_Init(void);

/*
 * Safety_RunChecks(soil_pct, temp_c, curr_mA, water_cm)
 *   Evaluate all 4 safety features with pre-read sensor values.
 *   Controls LEDs, buzzer, fan, lockout flag, and LCD line 2.
 *   Call every 2 s from main loop.
 */
void Safety_RunChecks(u8 soil_pct, u8 temp_c, u16 curr_mA, u8 water_cm);

u8 Safety_IsLocked(void);

#endif /* SAFETY_INTERFACE_H */
