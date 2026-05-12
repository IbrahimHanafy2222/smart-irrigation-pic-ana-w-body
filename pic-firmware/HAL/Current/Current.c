/*
 * Current.c
 * Irrigation Project — ACS712 Implementation
 */

#include "Current_interface.h"
#include "Current_config.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "../../SERVICES/STD_TYPES.h"

void Current_Init(void)
{
    /* ADC initialization handles the pin configuration */
}

s16 Current_Read_mA(void)
{
    u16 raw     = 0;
    u32 vout_mv = 0;
    s32 vdelta  = 0;
    s16 current = 0;

    raw = ADC_ReadAverage(CURRENT_ADC_CHANNEL, CURRENT_SAMPLES);

    vout_mv = ((u32)raw * CURRENT_VCC_MV) / CURRENT_ADC_RESOLUTION;
    vdelta = (s32)vout_mv - (s32)CURRENT_OFFSET_MV;

    current = (s16)((vdelta * 1000) / (s32)CURRENT_SENS_MV_A);

    return current;
}