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
    /* Direct register writes avoid the XC8 vs MikroC bit-name conflict.
       Under XC8, BRGH/TXEN/SPEN/CREN resolve to TXSTAbits.xxx (bit-field
       members, value 0/1) not integer positions; SET_BIT would shift by 0
       or 1 and set the wrong bit.  Write the known-good values directly. */
    TXSTA = 0x24u;          /* bit5=TXEN, bit2=BRGH, SYNC=0 (async)    */
    SPBRG = UART_SPBRG_VAL; /* 9600 baud @ 8 MHz: 51 → 9615 baud       */
    RCSTA = 0x90u;          /* bit7=SPEN (port on), bit4=CREN (rx on)   */
    /* Interrupts deliberately NOT enabled — RX polled via
       UART_DataAvailable() every main-loop tick.                      */
}

/* =================================
   RX Initialization
================================= */

void UART_RX_Init(void)
{
    TXSTA = 0x20u;          /* BRGH=0, SYNC=0, TXEN=1 (TX on for idle HIGH) */
    SPBRG = 51u;            /* 9600 baud @ 8 MHz                             */
    RCSTA = 0x90u;          /* SPEN=1, CREN=1                                */
    SET_BIT(PIE1,   5u);    /* RCIE  = PIE1<5>                               */
    SET_BIT(INTCON, 6u);    /* PEIE  = INTCON<6>                             */
    SET_BIT(INTCON, 7u);    /* GIE   = INTCON<7>                             */
}

/* =================================
   TX Initialization
================================= */

void UART_TX_Init(void)
{
    TXSTA = 0x24u;          /* BRGH=1, SYNC=0, TXEN=1 */
    SPBRG = 51u;
    RCSTA = 0x80u;          /* SPEN=1, CREN=0 (TX only) */
}

/* =================================
   Send Byte
================================= */

void UART_Write(u8 Data)
{
    while(!GET_BIT(TXSTA, 1u));   /* TRMT = TXSTA<1> */
    TXREG = Data;
}

void UART_Write_ISR(u8 Data)
{
    while(!GET_BIT(TXSTA, 1u));   /* TRMT = TXSTA<1> */
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
    return (u8)GET_BIT(PIR1, 5u);   /* RCIF = PIR1<5> */
}

/* =================================
   Receive Byte (Blocking)
   Waits until data arrives.
   Prefer UART_DataAvailable() +
   UART_Read() in polling contexts.
================================= */

u8 UART_Read(void)
{
    while(!GET_BIT(PIR1, 5u));   /* RCIF = PIR1<5> */
    return RCREG;
}

/* =================================
   TX Buffer Status
================================= */

u8 UART_TX_Empty(void)
{
    return GET_BIT(TXSTA, 1u);   /* TRMT = TXSTA<1> */
}

void UART_ClearOverrun(void)
{
    if(GET_BIT(RCSTA, 1u)) {   /* OERR = RCSTA<1> */
        CLR_BIT(RCSTA, 4u);   /* CREN = RCSTA<4> */
        SET_BIT(RCSTA, 4u);
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