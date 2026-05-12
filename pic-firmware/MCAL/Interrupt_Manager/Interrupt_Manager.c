/*
 * Interrupt_Manager.c
 * Irrigation Project — Central ISR Dispatcher
 * Compiler Agnostic (Uses INTERRUPT_ROUTINE Macro)
 */

#include "Interrupt_Manager_Interface.h"

#define UART_START_APP   0xCC
#define UART_END_APP     0xDD

extern volatile u8 g_tick_100ms;
extern u8 g_frame[4];
extern u8 g_frame_idx;
extern u8 g_frame_ready;

/* * INTERRUPT_ROUTINE dynamically expands to:
 * - void interrupt()                 (in MikroC)
 * - void __interrupt() ISR_Routine() (in XC8)
 */
INTERRUPT_ROUTINE
{
    /* Timer0 Interrupt Check */
    if (GET_BIT(INTCON, TMR0IF)) {

        CLR_BIT(INTCON, TMR0IF); /* Clear flag */
        g_tick_100ms = 1;
    }

    /* UART RX Interrupt Check */
    if (GET_BIT(PIR1, RCIF)) {
        u8 b = RCREG; /* Read the byte (auto-clears flag) */

        /* Frame assembly state machine logic */
        if(g_frame_idx == 0u) {
            if(b == UART_START_APP) {
                g_frame[0] = b;
                g_frame_idx = 1u;
            }
        }
        else if(g_frame_idx < 3u) {
            g_frame[g_frame_idx++] = b;
        }
        else {
            g_frame[3] = b;
            g_frame_idx = 0u;
            if(b == UART_END_APP) {
                g_frame_ready = 1u;
            }
        }
    }
}