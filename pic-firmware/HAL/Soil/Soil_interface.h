/*
 * Soil_interface.h
 * Irrigation Project — Analog Soil Sensor Public API
 */

#ifndef SOIL_INTERFACE_H
#define SOIL_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

void Soil_Init(void);
u8   Soil_Read_Pct(void);

#endif /* SOIL_INTERFACE_H */