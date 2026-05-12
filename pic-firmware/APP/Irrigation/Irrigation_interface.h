#ifndef IRRIGATION_INTERFACE_H
#define IRRIGATION_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/* Automatic mode: irrigates all 5 plants, skips if soil already wet. */
void Irrigation_RunCycle(void);

/* Manual mode: irrigates a single plant by index (no moisture skip check). */
void Irrigation_RunSinglePlant(u8 plant_index);

#endif /* IRRIGATION_INTERFACE_H */
