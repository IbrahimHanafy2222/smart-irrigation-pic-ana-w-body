/*
 * Soil.c
 * Irrigation Project — Soil Sensor Implementation
 */

#include "Soil_interface.h"
#include "Soil_config.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "../../SERVICES/STD_TYPES.h"

void Soil_Init(void)
{
    /* ADC initialization handles the analog pin configuration (TRISA) */
}

u8 Soil_Read_Pct(void)
{
    u16 raw = ADC_ReadAverage(SOIL_ADC_CHANNEL, SOIL_SAMPLES);
    u8  pct = 0;

    /* Clamp values to prevent underflow/overflow if sensor goes out of bounds */
    if(raw >= SOIL_DRY_ADC) return 0u;
    if(raw <= SOIL_WET_ADC) return 100u;

    /* Linear map from the ADC range to 0-100% */
    pct = (u8)(((u32)(SOIL_DRY_ADC - raw) * 100u) / 
               (SOIL_DRY_ADC - SOIL_WET_ADC));

    return pct;
}