/*
 * LCD_interface.h
 * Irrigation Project — LCD HD44780 (16x2, 4-bit) Public API
 */

#ifndef LCD_INTERFACE_H
#define LCD_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/* ── Row / Column constants ── */
#define LCD_ROW_1    1u
#define LCD_ROW_2    2u
#define LCD_COL_MAX  16u

/*
 * LCD_Init()
 *   Power-up initialization sequence for HD44780 in 4-bit mode.
 *   Must be called once before any other LCD function.
 */
void LCD_Init(void);

/*
 * LCD_SendCommand(cmd)
 *   Send a raw HD44780 command byte (RS=0).
 */
void LCD_SendCommand(u8 cmd);


void LCD_SendString_Const(const char *str);

/*
 * LCD_SendChar(ch)
 *   Send one character to the current cursor position (RS=1).
 */
void LCD_SendChar(u8 ch);

/*
 * LCD_SendString(str)
 *   Send a null-terminated string to the LCD.
 *   Automatically stops at column 16 to prevent overflow.
 */
void LCD_SendString(char *str);

/*
 * LCD_SendNumber(num)
 *   Display a signed 32-bit integer at the current cursor position.
 */
void LCD_SendNumber(s16 num);

/*
 * LCD_GoToRowCol(row, col)
 *   Move cursor to row (1 or 2) and column (1–16).
 */
void LCD_GoToRowCol(u8 row, u8 col);

/*
 * LCD_Clear()
 *   Clear display and return cursor to home (0,0).
 */
void LCD_Clear(void);

/*
 * LCD_DisplayStatus(row, label, value, unit)
 *   Utility: prints "label: valueunit" on the specified row.
 *   Example: LCD_DisplayStatus(1, "Temp", 35, "C") → "Temp: 35C      "
 */
void LCD_DisplayStatus(u8 row, char *label, s16 value, char *unit);

#endif /* LCD_INTERFACE_H */