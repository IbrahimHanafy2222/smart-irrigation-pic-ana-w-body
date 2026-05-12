#include "USART_interface.h"

/* =================================
   Global Pointer To Callback
================================= */

static void (*UART_Callback)(u8) = 0;

/* =================================
   Combined Init — Polling Mode
   TX + RX enabled, NO interrupts.
   Call this instead of UART_RX_Init
   + UART_TX_Init when using the
   irrigation controller (comms.c).
================================= */

void UART_Init(void)
{
    SET_BIT(TXSTA, BRGH);        /* High-speed mode                  */
    CLR_BIT(TXSTA, SYNC);        /* Asynchronous                     */
    SET_BIT(TXSTA, TXEN);        /* Enable TX                        */

    SPBRG = UART_SPBRG_VAL;      /* 9600 baud @ 8MHz (51)            */

    SET_BIT(RCSTA, SPEN);        /* Enable serial port (RC6/RC7)     */
    SET_BIT(RCSTA, CREN);        /* Continuous receive               */

    /* Interrupts deliberately NOT enabled — RX is polled via
       UART_DataAvailable() in the main loop every 100 ms tick.     */
}

/* =================================
   RX Initialization
================================= */

void UART_RX_Init(void)
{

    SET_BIT(TXSTA , BRGH);      // High Speed Mode

    SPBRG = 51;                 // 9600 Baud @ 8MHz: (8000000/(16*9600))-1 = 51 → 9615 baud (0.16% error)

    CLR_BIT(TXSTA , SYNC);      // Asynchronous Mode

    SET_BIT(RCSTA , SPEN);      // Enable Serial Port

    SET_BIT(RCSTA , CREN);      // Continuous Receive

    SET_BIT(PIE1 , RCIE);       // Enable UART RX Interrupt

    SET_BIT(INTCON , PEIE);     // Peripheral Interrupt Enable
    SET_BIT(INTCON , GIE);      // Global Interrupt Enable
}

/* =================================
   TX Initialization
================================= */

void UART_TX_Init(void)
{

    SET_BIT(TXSTA , BRGH);      // High Speed

    SPBRG = 51;                 // 9600 Baud @ 8MHz: (8000000/(16*9600))-1 = 51 → 9615 baud (0.16% error)

    CLR_BIT(TXSTA , SYNC);      // Asynchronous Mode

    SET_BIT(RCSTA , SPEN);      // Enable Serial Port

    SET_BIT(TXSTA , TXEN);      // Enable Transmission
}

/* =================================
   Send Byte
================================= */

void UART_Write(u8 Data)
{

    while(!GET_BIT(TXSTA , TRMT));   // Wait until TX empty

    TXREG = Data;
}

void UART_Write_ISR(u8 Data) {
    while(!GET_BIT(TXSTA , TRMT));   // Wait until TX empty

    TXREG = Data;
}

/* =================================
   Non-Blocking RX Check
   Returns 1 if a byte is waiting
   in RCREG, 0 if nothing received.
   Use before UART_Read() to avoid
   blocking the main loop.
================================= */

u8 UART_DataAvailable(void)
{
    return (u8)GET_BIT(PIR1, RCIF);
}

/* =================================
   Receive Byte (Blocking)
   Waits until data arrives.
   Prefer UART_DataAvailable() +
   UART_Read() in polling contexts.
================================= */

u8 UART_Read(void)
{
    while(!GET_BIT(PIR1, RCIF));     /* Wait for data */
    return RCREG;
}

/* =================================
   TX Buffer Status
================================= */

u8 UART_TX_Empty(void)
{

    return GET_BIT(TXSTA , TRMT);
}

void UART_ClearOverrun(void)
{
    if(GET_BIT(RCSTA, 1u)) {   /* OERR = RCSTA<1> */
        CLR_BIT(RCSTA, CREN);
        SET_BIT(RCSTA, CREN);
    }
}

/* =================================
   Callback Setter
================================= */

void UART_SetCallback(void (*Callback)(u8))
{

    if(Callback != 0)
    {
        UART_Callback = Callback;
    }

}

void UART_ISR(void)
{

    u8 UART_data = RCREG;   //
    if(UART_Callback != 0)
    {
        UART_Callback(UART_data);   //
    }

}

void UART_WriteString(char *str)
{
    while(*str)
    {
        UART_Write(*str++);
    }
}