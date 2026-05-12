/*
 * Current_config.h
 * Irrigation Project — ACS712-05B Calibration Constants
 */

#ifndef CURRENT_CONFIG_H
#define CURRENT_CONFIG_H

#include "../../MCAL/ADC/ADC_interface.h"

#define CURRENT_ADC_CHANNEL         ADC_CHANNEL_CURRENT
#define CURRENT_SAMPLES             8u

#define CURRENT_VCC_MV              5000u
#define CURRENT_ADC_RESOLUTION      1024u
#define CURRENT_OFFSET_MV           2500u
#define CURRENT_SENS_MV_A           66u

#endif /* CURRENT_CONFIG_H */