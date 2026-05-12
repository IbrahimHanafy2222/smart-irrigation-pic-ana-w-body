/*
 * Current_interface.h
 * Irrigation Project — ACS712 Current Sensor Public API
 */

#ifndef CURRENT_INTERFACE_H
#define CURRENT_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

void Current_Init(void);
s16  Current_Read_mA(void);

#endif /* CURRENT_INTERFACE_H */