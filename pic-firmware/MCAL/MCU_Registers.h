/*
 * MCU_Registers.h
 * PIC16F877A Register Map & Project Hardware Pinout
 * Compiler: MikroC Pro for PIC  OR  MPLAB XC8
 */

#ifndef MCU_REGISTERS_H
#define MCU_REGISTERS_H

#include "../SERVICES/STD_TYPES.h"
#include "../SERVICES/Delay.h"
#include <stddef.h>

#define F_OSC    8000000UL   /* 8 MHz */

/* =====================================================
   COMPILER SPLIT
   ===================================================== */
#if defined(__XC8) || defined(__XC)

    /* XC8: device header supplies all SFR / bit-field names */
    #ifndef _XTAL_FREQ
    #define _XTAL_FREQ  8000000UL
    #endif
    #include <xc.h>

    #define INTERRUPT_ROUTINE void __interrupt() ISR_Routine(void)

#else   /* MikroC Pro for PIC */

    #define INTERRUPT_ROUTINE void interrupt()

    /* PORT DATA REGISTERS */
    #define PORTA   (*((volatile u8*)0x05))
    #define PORTB   (*((volatile u8*)0x06))
    #define PORTC   (*((volatile u8*)0x07))
    #define PORTD   (*((volatile u8*)0x08))
    #define PORTE   (*((volatile u8*)0x09))

    /* TRIS REGISTERS */
    #define TRISA   (*((volatile u8*)0x85))
    #define TRISB   (*((volatile u8*)0x86))
    #define TRISC   (*((volatile u8*)0x87))
    #define TRISD   (*((volatile u8*)0x88))
    #define TRISE   (*((volatile u8*)0x89))

    /* INTCON & OPTION */
    #define INTCON      (*((volatile u8*)0x0B))
    #define GIE          7
    #define PEIE         6
    #define TMR0IE       5
    #define INTE         4
    #define RBIE         3
    #define TMR0IF       2
    #define INTF         1
    #define RBIF         0

    #define OPTION_REG  (*((volatile u8*)0x81))
    #define INTEDG       6
    #define T0CS         5
    #define T0SE         4
    #define PSA          3
    #define PS2          2
    #define PS1          1
    #define PS0          0

    /* TIMERS */
    #define TMR0    (*((volatile u8*)0x01))
    #define TMR1L   (*((volatile u8*)0x0E))
    #define TMR1H   (*((volatile u8*)0x0F))
    #define T1CON   (*((volatile u8*)0x10))
    #define T1CKPS1  5
    #define T1CKPS0  4
    #define T1OSCEN  3
    #define T1SYNC   2
    #define TMR1CS   1
    #define TMR1ON   0

    /* PIR1 / PIE1 */
    #define PIR1    (*((volatile u8*)0x0C))
    #define PIE1    (*((volatile u8*)0x8C))
    #define ADIF     6
    #define RCIF     5
    #define TXIF     4
    #define TMR1IF   0
    #define ADIE     6
    #define RCIE     5
    #define TXIE     4
    #define TMR1IE   0

    /* ADC */
    #define ADCON0  (*((volatile u8*)0x1F))
    #define ADCON1  (*((volatile u8*)0x9F))
    #define ADRESH  (*((volatile u8*)0x1E))
    #define ADRESL  (*((volatile u8*)0x9E))
    #define ADCS1    7
    #define ADCS0    6
    #define CHS2     5
    #define CHS1     4
    #define CHS0     3
    #define GO_DONE  2
    #define ADON     0
    #define ADFM     7

    /* USART */
    #define TXSTA   (*((volatile u8*)0x98))
    #define RCSTA   (*((volatile u8*)0x18))
    #define SPBRG   (*((volatile u8*)0x99))
    #define TXREG   (*((volatile u8*)0x19))
    #define RCREG   (*((volatile u8*)0x1A))
    #define TXEN     5
    #define SYNC     4
    #define BRGH     2
    #define TRMT     1
    #define SPEN     7
    #define CREN     4

#endif  /* compiler split */

/* =====================================================
   PROJECT HARDWARE MAPPING (shared by both compilers)
   ===================================================== */
#define LCD_CTRL_PORT    PORTD
#define LCD_CTRL_TRIS    TRISD
#define LCD_RS_PIN       2
#define LCD_RW_PIN       1
#define LCD_E_PIN        3

#define LCD_DATA_PORT    PORTD
#define LCD_DATA_TRIS    TRISD
#define LCD_D4_PIN       4
#define LCD_D5_PIN       5
#define LCD_D6_PIN       6
#define LCD_D7_PIN       7

#define HUMIDITY_DATA_PORT    PORTB
#define HUMIDITY_DATA_TRIS    TRISB
#define HUMIDITY_DATA_PIN     0

#define RELAY_PORT       PORTD
#define RELAY_TRIS       TRISD
#define RELAY_PIN        0

#define BUZZER_PORT      PORTB
#define BUZZER_TRIS      TRISB
#define BUZZER_PIN       2   /* RB2 */

#define LED_YELLOW_PORT  4   /* GPIO_PORTE */
#define LED_YELLOW_PIN   1   /* RE1 */

#define LED_RED_PORT     4   /* GPIO_PORTE */
#define LED_RED_PIN      2   /* RE2 */

#endif /* MCU_REGISTERS_H */
