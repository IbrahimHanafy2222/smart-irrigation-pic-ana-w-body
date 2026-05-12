/*
 * Temp_config.h
 * Irrigation Project — LM35 Calibration Constants
 */

#ifndef TEMP_CONFIG_H
#define TEMP_CONFIG_H

#include "../../MCAL/ADC/ADC_interface.h"

#define TEMP_ADC_CHANNEL            ADC_CHANNEL_TEMP
#define TEMP_SAMPLES                4u

#define TEMP_VCC_MV                 5000u
#define TEMP_ADC_RESOLUTION         1024u
#define TEMP_SENSITIVITY_MV_PER_C   10u

#endif /* TEMP_CONFIG_H */