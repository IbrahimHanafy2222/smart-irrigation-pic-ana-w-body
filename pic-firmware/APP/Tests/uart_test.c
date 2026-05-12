/*
 * uart_test.c
 * Standalone UART RX verification — PIC16F877A @ 8 MHz
 *
 * Waits for any byte on RC7 (9600 baud) and displays it on LCD.
 * Row 1: last received byte in hex
 * Row 2: total bytes received
 *
 * Compile SEPARATELY — not part of main irrigation build.
 * xc8-cc -mcpu=16F877A ... uart_test.c LCD.c USART.c Delay.c -o uart_test.hex
 */

#include "../../config.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../MCAL/USART/USART_interface.h"
#include "../../HAL/LCD/LCD_interface.h"
#include "../../SERVICES/STD_TYPES.h"
#include "../../SERVICES/BIT_MATH.h"

#pragma config FOSC  = HS
#pragma config WDTE  = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP   = OFF
#pragma config CPD   = OFF
#pragma config WRT   = OFF
#pragma config CP    = OFF

volatile u8 portd_shadow = 0xFFu;

static void lcd_hex(u8 val)
{
    const char hex[] = "0123456789ABCDEF";
    LCD_SendString_Const("0x");
    LCD_SendChar((u8)hex[val >> 4]);
    LCD_SendChar((u8)hex[val & 0x0Fu]);
}

void main(void)
{
    u8  rx_byte  = 0u;
    u16 rx_count = 0u;

    ADCON1 = 0x06u;
    TRISA  = 0x07u;
    TRISB  = 0b00011011u;
    TRISC  = 0b10000000u;   /* RC7 = RX input */
    TRISD  = 0x00u;
    TRISE  = 0x00u;

    CLR_BIT(OPTION_REG, 7u);
    PORTD = portd_shadow;
    SET_BIT(PORTC, 6u);     /* UART TX idle HIGH */

    UART_Init();
    LCD_Init();

    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("UART Test Ready ");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const("Waiting bytes...");

    while(1)
    {
        if(UART_DataAvailable())
        {
            rx_byte = UART_Read();
            rx_count++;

            LCD_GoToRowCol(1u, 1u);
            LCD_SendString_Const("Last: ");
            lcd_hex(rx_byte);
            LCD_SendString_Const("      ");

            LCD_GoToRowCol(2u, 1u);
            LCD_SendString_Const("Count:");
            LCD_SendNumber((s16)rx_count);
            LCD_SendString_Const("          ");
        }
    }
}
