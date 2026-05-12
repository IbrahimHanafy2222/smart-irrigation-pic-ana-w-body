/*
 * Relay_private.h
 * Irrigation Project — Relay Module Internal Pin Map
 */

#ifndef RELAY_PRIVATE_H
#define RELAY_PRIVATE_H

#include "../../MCAL/GPIO/GPIO_interface.h"
#include "../../MCAL/MCU_Registers.h"

/* -- Logic levels -- */
#define RELAY_ACTIVE      HIGH    /* HIGH = relay energized (pump ON)  */
#define RELAY_INACTIVE    LOW     /* LOW  = relay de-energized (pump OFF) */

#endif /* RELAY_PRIVATE_H */