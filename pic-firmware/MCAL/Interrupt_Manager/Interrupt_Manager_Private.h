/*
 * Interrupt_Manager_Private.h
 * Irrigation Project — Interrupt Flag Bit Aliases
 */

#ifndef INTERRUPT_MANAGER_PRIVATE_H
#define INTERRUPT_MANAGER_PRIVATE_H

/* INTCON bit aliases for Interrupt_Manager.c */
#define T0IE_BIT    5   /* Timer0 Interrupt Enable  */
#define T0IF_BIT    2   /* Timer0 Interrupt Flag    */
#define INTE_BIT    4   /* External INT Enable      */
#define INTF_BIT    1   /* External INT Flag        */

/* PIR1 bit aliases */
#define RCIF_BIT    5   /* UART RX Interrupt Flag   */

#endif /* INTERRUPT_MANAGER_PRIVATE_H */
