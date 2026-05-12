/*
 * ADC.c
 * Irrigation Project � ADC Driver Implementation
 * Target: PIC16F877A @ 8 MHz | Compiler Agnostic
 */

#include "../../config.h"
#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"
#include "../MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

/* Notice: ADC___delay_us() wrapper has been completely removed! */

void ADC_Init(void)
{
    /* Step 1: Set AN0, AN1, AN2 as analog inputs in TRISA */
    SET_BIT(TRISA, ADC_CH_AN0);
    SET_BIT(TRISA, ADC_CH_AN1);
    SET_BIT(TRISA, ADC_CH_AN2);

    /* Step 2: Configure ADCON1 (Right justified, PCFG for analog pins) */
    ADCON1 = ADC_ADCON1_CONFIG;

    /* Step 3: Configure ADCON0 (Clock = Fosc/32) and Turn ON */
    ADCON0 &= 0b00111111;              /* Clear ADCS bits */
    ADCON0 |= (ADC_CLK_FOSC_32 << 6);  /* Set ADC clock   */
    SET_BIT(ADCON0, ADC_BIT_ADON);     /* Power ON ADC    */

    /* Step 4: Acquisition delay using our new generic delay */
    __delay_us(ADC_ACQ_DELAY_US);
}

u16 ADC_Read(u8 channel)
{
    u16 result = 0;

    /* 1. Select channel: write CHS2:CHS0 in ADCON0 */
    ADCON0 &= 0b11000111;
    ADCON0 |= (u8)((channel & 0x07) << 3);

    /* 2. Acquisition delay using generic service */
    __delay_us(ADC_ACQ_DELAY_US);

    /* 3. Start conversion */
    SET_BIT(ADCON0, ADC_BIT_GO);

    /* 4. Wait for conversion to complete */
    while(GET_BIT(ADCON0, ADC_BIT_GO));

    /* 5. Read 10-bit result */
    result  = (u16)ADRESH << 8;
    result |= (u16)ADRESL;
    result &= 0x03FF;

    return result;
}

u16 ADC_ReadAverage(u8 channel, u8 samples)
{
    u32 sum = 0;
    u8  i   = 0;

    if(samples == 0) return 0;

    for(i = 0; i < samples; i++)
    {
        sum += ADC_Read(channel);
    }
    return (u16)(sum / samples);
}

u8 ADC_SoilPct(u16 raw)
{
    u16 wet = WET_ADC;
    u16 dry = DRY_ADC;
    s16 pct;

    if(raw <= wet) return 100u;
    if(raw >= dry) return 0u;
    pct = (s16)(100 - (((s16)(raw - wet)) * 100 / (s16)(dry - wet)));
    if(pct < 0)   pct = 0;
    if(pct > 100) pct = 100;
    return (u8)pct;
}

u16 ADC_CurrentmA(u16 raw)
{
    s16 offset;
    s16 signed_raw = (s16)raw;

    offset = signed_raw - 512;
    if(offset < 0) offset = -offset;
    return (u16)((u16)offset * 26u);
}