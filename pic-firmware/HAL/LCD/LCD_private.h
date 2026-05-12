#ifndef LCD_PRIVATE_H
#define LCD_PRIVATE_H

#include "../../MCAL/MCU_Registers.h"

/* -- LCD Commands -- */
#define LCD_CMD_CLEAR_DISPLAY    0x01
#define LCD_CMD_RETURN_HOME      0x02
#define LCD_CMD_ENTRY_MODE       0x06
#define LCD_CMD_DISPLAY_ON       0x0C
#define LCD_CMD_FUNCTION_SET_4BIT 0x28
#define LCD_CMD_SET_CGRAM        0x40
#define LCD_CMD_SET_DDRAM        0x80

/* -- Row DDRAM offsets -- */
#define LCD_ROW1_OFFSET   0x00
#define LCD_ROW2_OFFSET   0x40

/* -- Timing -- */
#define LCD_INIT_DELAY_MS    20
#define LCD_CMD_DELAY_MS      2
#define LCD_CLEAR_DELAY_MS    2
#define LCD_ENABLE_PULSE_US  10

#endif /* LCD_PRIVATE_H */