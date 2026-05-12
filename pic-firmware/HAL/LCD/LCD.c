/*
 * LCD.c
 * HD44780 4-bit mode driver. Uses portd_shadow for ALL PORTD writes so that
 * pump relay (RD0) and spare relay (RD1) state is never corrupted by LCD ops.
 */

#include "../../config.h"
#include "LCD_interface.h"
#include "LCD_private.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

/* Defined in MyProject.c — every PORTD write must go through this. */
extern volatile u8 portd_shadow;

/* ============================================================
   MACROS
   ============================================================ */

/* Write high nibble of 'nibble' into RD4-RD7, then pulse E (RD3). */
#define LCD_SEND_NIBBLE(nibble) do { \
    portd_shadow = (portd_shadow & 0x0Fu) | ((u8)(nibble) & 0xF0u); \
    PORTD = portd_shadow; \
    portd_shadow |= (u8)(1u << LCD_E_PIN); \
    PORTD = portd_shadow; \
    __delay_us(LCD_ENABLE_PULSE_US); \
    portd_shadow &= (u8)(~(u8)(1u << LCD_E_PIN)); \
    PORTD = portd_shadow; \
    __delay_us(LCD_ENABLE_PULSE_US); \
} while(0)

/* Send full byte (2 nibbles). rs=1 for data, rs=0 for command. */
#define LCD_SEND_BYTE(byte, rs) do { \
    if ((rs) == 1) { portd_shadow |= (u8)(1u << LCD_RS_PIN); } \
    else           { portd_shadow &= (u8)(~(u8)(1u << LCD_RS_PIN)); } \
    PORTD = portd_shadow; \
    LCD_SEND_NIBBLE((u8)(byte) & 0xF0u); \
    LCD_SEND_NIBBLE((u8)((u8)(byte) << 4)); \
    __delay_ms(LCD_CMD_DELAY_MS); \
} while(0)

/* ============================================================
   LCD_Init
   ============================================================ */
void LCD_Init(void)
{
    CLR_BIT(LCD_CTRL_TRIS, LCD_RS_PIN);
    CLR_BIT(LCD_CTRL_TRIS, LCD_E_PIN);
    CLR_BIT(LCD_DATA_TRIS, LCD_D4_PIN);
    CLR_BIT(LCD_DATA_TRIS, LCD_D5_PIN);
    CLR_BIT(LCD_DATA_TRIS, LCD_D6_PIN);
    CLR_BIT(LCD_DATA_TRIS, LCD_D7_PIN);

    __delay_ms(LCD_INIT_DELAY_MS);

    portd_shadow &= (u8)(~(u8)(1u << LCD_RS_PIN));
    PORTD = portd_shadow;

    LCD_SEND_NIBBLE(0x30u);   __delay_ms(5);
    LCD_SEND_NIBBLE(0x30u);   __delay_ms(1);
    LCD_SEND_NIBBLE(0x30u);   __delay_ms(1);
    LCD_SEND_NIBBLE(0x20u);   __delay_ms(1);

    LCD_SendCommand(LCD_CMD_FUNCTION_SET_4BIT);
    LCD_SendCommand(0x08u);
    LCD_SendCommand(LCD_CMD_CLEAR_DISPLAY);
    __delay_ms(LCD_CLEAR_DELAY_MS);
    LCD_SendCommand(LCD_CMD_ENTRY_MODE);
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);
}

/* ============================================================
   LCD Functions
   ============================================================ */
void LCD_SendCommand(u8 cmd)
{
    LCD_SEND_BYTE(cmd, 0);
}

void LCD_SendChar(u8 ch)
{
    LCD_SEND_BYTE(ch, 1);
}

void LCD_SendString(char *str)
{
    u8 count = 0;
    while(*str && count < LCD_COL_MAX)
    {
        LCD_SendChar((u8)*str);
        str++; count++;
    }
}

void LCD_SendString_Const(const char *str)
{
    u8 count = 0;
    while(*str && count < LCD_COL_MAX)
    {
        LCD_SendChar((u8)*str);
        str++; count++;
    }
}

void LCD_SendNumber(s16 num)
{
    char buf[7];
    u8   i   = 0;
    u8   len = 0;
    char tmp[7];

    if(num < 0) {
        num = -num;
        LCD_SendChar('-');
    }
    if(num == 0) {
        LCD_SendChar('0');
        return;
    }
    while(num > 0) {
        tmp[i++] = (char)('0' + (u8)(num % 10));
        num /= 10;
    }
    len = i;
    for(i = 0; i < len; i++) {
        buf[i] = tmp[len - 1 - i];
    }
    buf[len] = '\0';
    LCD_SendString(buf);
}

void LCD_GoToRowCol(u8 row, u8 col)
{
    u8 address = 0;
    if(col  < 1)  col  = 1;
    if(col  > 16) col  = 16;
    if(row != 2)  row  = 1;

    if(row == 1) address = (u8)(LCD_CMD_SET_DDRAM + LCD_ROW1_OFFSET + (col - 1u));
    else         address = (u8)(LCD_CMD_SET_DDRAM + LCD_ROW2_OFFSET + (col - 1u));
    LCD_SendCommand(address);
}

void LCD_Clear(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR_DISPLAY);
    __delay_ms(LCD_CLEAR_DELAY_MS);
}

void LCD_DisplayStatus(u8 row, char *label, s16 value, char *unit)
{
    LCD_GoToRowCol(row, 1);
    LCD_SendString(label);
    LCD_SendChar(':');
    LCD_SendChar(' ');
    LCD_SendNumber(value);
    LCD_SendString(unit);
}
