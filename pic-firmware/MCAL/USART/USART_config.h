#ifndef UART_CONFIG_H
#define UART_CONFIG_H

/* CPU Frequency — must match _XTAL_FREQ in MCU_Registers.h */
#define FOSC 8000000UL

/* Baud Rate */
#define UART_BAUDRATE 9600

/* Speed Mode */
#define UART_HIGH_SPEED 1

/* SPBRG value for 9600 baud @ 8MHz, BRGH=1:
   SPBRG = (FOSC / (16 * BAUDRATE)) - 1 = (8000000 / 153600) - 1 = 51
   Actual baud = 9615, error = 0.16% */
#define UART_SPBRG_VAL 51u

#endif