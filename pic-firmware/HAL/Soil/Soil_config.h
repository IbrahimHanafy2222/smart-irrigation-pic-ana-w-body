/*
 * Soil_config.h
 * Irrigation Project — Soil Sensor Calibration Constants
 */

#ifndef SOIL_CONFIG_H
#define SOIL_CONFIG_H

#include "../../MCAL/ADC/ADC_interface.h"

/* Channel and Sampling Configuration */
#define SOIL_ADC_CHANNEL    ADC_CHANNEL_SOIL
#define SOIL_SAMPLES        4u

/* Calibration Thresholds */
#define SOIL_DRY_ADC        820u    /* ADC reading in completely dry air/soil */
#define SOIL_WET_ADC        350u    /* ADC reading when fully submerged/saturated */

#endif /* SOIL_CONFIG_H */