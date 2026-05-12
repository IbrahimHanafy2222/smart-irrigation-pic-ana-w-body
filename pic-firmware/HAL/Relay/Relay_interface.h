/*
 * Relay_interface.h
 * Irrigation Project — Relay (Water Pump) Driver Public API
 */

#ifndef RELAY_INTERFACE_H
#define RELAY_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/* Relay state constants */
#define RELAY_ON    1u
#define RELAY_OFF   0u

/*
 * Relay_Init()
 *   Configure relay control pin as output, initial state = OFF.
 *   Call once at startup.
 */
void Relay_Init(void);

/*
 * Relay_On()
 *   Energize relay → Pump ON.
 */
void Relay_On(void);

/*
 * Relay_Off()
 *   De-energize relay → Pump OFF.
 */
void Relay_Off(void);

/*
 * Relay_GetState() → u8
 *   Returns current relay state: RELAY_ON or RELAY_OFF.
 *   Reads actual pin state — not a stored variable.
 */
u8 Relay_GetState(void);

#endif /* RELAY_INTERFACE_H */
