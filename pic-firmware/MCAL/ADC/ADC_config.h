/*
 * ADC_config.h
 * Irrigation Project — ADC Configuration
 */

#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

/*
 * Result format:
 *   ADC_RIGHT_JUSTIFIED  → result in ADRESH:ADRESL bits [9:0] (use this)
 *   ADC_LEFT_JUSTIFIED   → result in ADRESH bits [9:2]
 */
#define ADC_RIGHT_JUSTIFIED   1
#define ADC_LEFT_JUSTIFIED    0
#define ADC_RESULT_FORMAT     ADC_RIGHT_JUSTIFIED

/* ADC clock: Fosc/32 (safe for 20 MHz) */
#define ADC_CLOCK_SELECT      ADC_CLK_FOSC_32

#endif /* ADC_CONFIG_H */
