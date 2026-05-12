#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

#include "USART_private.h"
#include "USART_config.h"
#include "../MCU_Registers.h"
#include "../../SERVICES/STD_TYPES.h"
#include "../../SERVICES/BIT_MATH.h"

/* --- Initialization --- */

/* Combined TX+RX init, no interrupts (polling mode).
   Use this for the irrigation controller comms layer. */
void UART_Init(void);

/* Legacy separate inits (interrupt-driven RX) — kept for compatibility */
void UART_RX_Init(void);
void UART_TX_Init(void);

/* --- Data Operations --- */
void UART_Write(u8 Data);           /* Blocking TX — waits for TRMT    */
void UART_Write_ISR(u8 Data);       /* Same as UART_Write, ISR-safe     */
u8   UART_Read(void);               /* Blocking RX — waits for RCIF     */
u8   UART_DataAvailable(void);      /* Non-blocking: 1 if byte waiting  */
void UART_WriteString(char *str);

/* --- Status --- */
u8 UART_TX_Empty(void);

/* Clear UART overrun error (RCSTA.OERR): toggle CREN low→high.
   Call when OERR is set; receiver stalls until cleared. */
void UART_ClearOverrun(void);

/* --- Interrupt-driven support (used with UART_RX_Init only) --- */
void UART_SetCallback(void (*Callback)(u8));
void UART_ISR(void);

#endif