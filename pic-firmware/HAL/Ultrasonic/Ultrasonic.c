/*
 * Ultrasonic.c
 * HC-SR04 driver. Trigger on RC3, Echo on RB1.
 * Returns water level = TANK_HEIGHT_CM - measured_distance_cm.
 * Median of 3 readings for noise rejection.
 */

#include "../../config.h"
#include "Ultrasonic_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

static u8 single_reading(void)
{
    u16 count = 0u;

    /* 10 µs trigger pulse on RC3 */
    SET_BIT(PORTC, PIN_TRIG);
    __delay_us(10);
    CLR_BIT(PORTC, PIN_TRIG);

    /* Wait for echo to go HIGH */
    count = 0u;
    while(!GET_BIT(PORTB, PIN_ECHO)) {
        __delay_us(ECHO_US_PER_LOOP);
        if(++count > ECHO_TIMEOUT_LOOPS) return 0xFFu;
    }

    /* Measure HIGH duration */
    count = 0u;
    while(GET_BIT(PORTB, PIN_ECHO)) {
        __delay_us(ECHO_US_PER_LOOP);
        if(++count > ECHO_TIMEOUT_LOOPS) return 0xFFu;
    }

    /* distance_cm = (count * ECHO_US_PER_LOOP) / 58 */
    return (u8)((u16)(count * ECHO_US_PER_LOOP) / 58u);
}

static u8 median3(u8 a, u8 b, u8 c)
{
    if((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

u8 Ultrasonic_GetWaterLevel(void)
{
    u8 d1, d2, d3, dist;

    d1 = single_reading(); __delay_ms(10);
    d2 = single_reading(); __delay_ms(10);
    d3 = single_reading();

    if(d1 == 0xFFu || d2 == 0xFFu || d3 == 0xFFu) return 0xFFu;

    dist = median3(d1, d2, d3);
    if(dist >= TANK_HEIGHT_CM) return 0u;
    return (u8)(TANK_HEIGHT_CM - dist);
}
