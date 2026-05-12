/*
 * Interrupt_Manager_Config.h
 * Irrigation Project — ISR Enable Flags
 */

#ifndef INTERRUPT_MANAGER_CONFIG_H
#define INTERRUPT_MANAGER_CONFIG_H

#define INT_MGR_TIMER0_EN    1   /* Timer0 — 100ms main tick       */
#define INT_MGR_EXT_INT_EN   0   /* External INT — not used        */
#define INT_MGR_UART_RX_EN   1   /* UART RX — ESP8266 commands     */

#endif /* INTERRUPT_MANAGER_CONFIG_H */
