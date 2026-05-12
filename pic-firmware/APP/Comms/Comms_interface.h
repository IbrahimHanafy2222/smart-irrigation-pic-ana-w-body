#ifndef COMMS_INTERFACE_H
#define COMMS_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/* Outbound (PIC → Pi) */
void Comms_SendSensors(u8 soil, u8 temp, u8 hum, u16 curr, u8 water);
void Comms_SendAtPlant(u8 plant_index);
void Comms_SendStatus(u8 mode, u8 lockout);

/* Inbound (Pi → PIC) — call every main-loop tick (non-blocking) */
void Comms_Poll(void);

/* Results of last parsed packet */
u8 Comms_GetMode(void);
u8 Comms_ManualCommandPending(void);
u8 Comms_GetManualPlant(void);
u8 Comms_AppEstopActive(void);
u8 Comms_HandshakeReceived(void);
void Comms_ClearManualCommand(void);
void Comms_SendHandshakeAck(void);

#endif /* COMMS_INTERFACE_H */
