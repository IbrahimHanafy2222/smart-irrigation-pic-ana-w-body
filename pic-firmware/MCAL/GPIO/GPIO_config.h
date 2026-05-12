/*
 * GPIO_config.h
 * Smart Irrigation — Pin Direction & Initial Value Config
 *
 * PORTA:  RA0=Soil(AN0 IN), RA1=unused(AN1 IN), RA2=Current(AN2 IN) — analog inputs
 * PORTB:  RB0=DHT11(bi-dir,IN), RB1=Echo(IN), RB2=Buzzer(OUT)
 *         RB3=LimitSW(IN pull-up), RB4=EStop(IN pull-up)
 * PORTC:  RC0=STEP(OUT), RC1=DIR(OUT), RC2=ENABLE(OUT), RC3=TRIG(OUT),
 *         RC4=Fan(OUT), RC6=UART_TX(OUT), RC7=UART_RX(IN)
 * PORTD:  RD0=Pump relay(OUT), RD1=Spare relay(OUT), RD2=LCD_RS(OUT),
 *         RD3=LCD_E(OUT), RD4-7=LCD D4-D7(OUT)  — all OUTPUT
 * PORTE:  RE0=unused(IN), RE1=LED_YLW(OUT), RE2=LED_RED(OUT)
 */

#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#define GPIO_PORTA_DIR       0x07   /* RA0-RA2 analog inputs; RA3-5 output */

/* RB0=IN, RB1=IN, RB2=OUT, RB3=IN, RB4=IN; others OUT */
#define GPIO_PORTB_DIR       0b00011011   /* 0x1B */

/* RC7=IN(RX); all others output */
#define GPIO_PORTC_DIR       0b10000000   /* 0x80 */

#define GPIO_PORTD_DIR       0x00         /* All outputs */

/* RE0=IN(unused), RE1=OUT, RE2=OUT */
#define GPIO_PORTE_DIR       0b00000001   /* 0x01 */

/* Initial values: relays OFF (active-LOW → HIGH), fan/stepper/LEDs off */
#define GPIO_PORTA_INIT_VAL  0x00
#define GPIO_PORTB_INIT_VAL  0x00
#define GPIO_PORTC_INIT_VAL  0x04   /* RC2=1: stepper DISABLED at startup */
#define GPIO_PORTD_INIT_VAL  0xFF   /* RD0/RD1 HIGH = relays OFF */
#define GPIO_PORTE_INIT_VAL  0x00   /* LEDs off */

#endif /* GPIO_CONFIG_H */
