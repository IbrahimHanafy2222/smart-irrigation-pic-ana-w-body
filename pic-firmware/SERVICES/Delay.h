/*
 * Delay.h
 * XC8: provides _XTAL_FREQ + xc.h so __delay_ms / __delay_us work everywhere.
 * Use __delay_ms(x) and __delay_us(x) directly in all drivers.
 */

#ifndef DELAY_H
#define DELAY_H

#if defined(__XC8) || defined(__XC)
    #ifndef _XTAL_FREQ
        #define _XTAL_FREQ  8000000UL
    #endif
    #include <xc.h>
#endif

#endif /* DELAY_H */
