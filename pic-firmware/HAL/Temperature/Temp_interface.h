/*
 * Temp_interface.h
 * Irrigation Project — LM35 Temperature Sensor Public API
 */

#ifndef TEMP_INTERFACE_H
#define TEMP_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

void Temp_Init(void);
s16  Temp_Read_C(void);

#endif /* TEMP_INTERFACE_H */