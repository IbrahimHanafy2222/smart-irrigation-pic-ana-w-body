/*
 * Interrupt_Manager_Interface.h
 * Irrigation Project — ISR Dispatcher Header
 */

#ifndef INTERRUPT_MANAGER_INTERFACE_H
#define INTERRUPT_MANAGER_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"
#include "../../SERVICES/BIT_MATH.h"
#include "Interrupt_Manager_Private.h"
#include "Interrupt_Manager_Config.h"
#include "../MCU_Registers.h"

/* ISR handlers defined in their respective driver modules */
extern void TIMER0_ISR(void);
extern void UART_ISR(void);

#endif /* INTERRUPT_MANAGER_INTERFACE_H */
