/*
 * Temp.c
 * Irrigation Project — LM35 Implementation
 */

#include "Temp_interface.h"
#include "Temp_config.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "../../SERVICES/STD_TYPES.h"

void Temp_Init(void)
{
    /* ADC initialization handles the pin configuration */
}

s16 Temp_Read_C(void)
{
    u16 raw = 0;
    s16 temp_c = 0;

    raw = ADC_ReadAverage(TEMP_ADC_CHANNEL, TEMP_SAMPLES);
    
    /* 500u represents (VCC_MV / SENSITIVITY) -> 5000 / 10 = 500 */
    temp_c = (s16)(((u32)raw * 500u) / TEMP_ADC_RESOLUTION);

    return temp_c;
}