/*
 * Buzzer_interface.h
 * Irrigation Project — Buzzer Driver Public API
 *
 * Used for: Warning-level alerts (Level 1 of each safety feature)
 */

#ifndef BUZZER_INTERFACE_H
#define BUZZER_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/*
 * Buzzer_Init()     — Set buzzer pin as output, start silent.
 * Buzzer_On()       — Turn buzzer ON continuously.
 * Buzzer_Off()      — Turn buzzer OFF.
 * Buzzer_Beep(ms)   — Beep for ms milliseconds, then go silent.
 * Buzzer_BeepN(n, ms_on, ms_off)
 *                   — Beep n times: on for ms_on, off for ms_off.
 *                     Used for warning patterns (e.g. 3 short beeps).
 */
void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Beep(u16 duration_ms);
void Buzzer_BeepN(u8 count, u16 ms_on, u16 ms_off);

#endif /* BUZZER_INTERFACE_H */
