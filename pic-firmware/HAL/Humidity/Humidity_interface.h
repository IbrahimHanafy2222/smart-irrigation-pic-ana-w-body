/*
 * Humidity_interface.h
 * Irrigation Project � DHT11/DHT22 Humidity Sensor Public API
 */

#ifndef HUMIDITY_INTERFACE_H
#define HUMIDITY_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

#define HUMIDITY_OK             0u
#define HUMIDITY_ERROR          1u
#define HUMIDITY_ERR_RESPONSE   2u  /* Sensor didn't pull LOW/HIGH during handshake */
#define HUMIDITY_ERR_BITREAD    3u  /* Timeout waiting for bit edge during data read */
#define HUMIDITY_ERR_CHECKSUM   4u  /* 40 bits received but checksum mismatch */

/*
 * Humidity_Init()
 * Configures the DHT pin as an input by default so it stays HIGH
 * via the internal or external pull-up resistor.
 */
void Humidity_Init(void);

/*
 * Humidity_Read(humidity, temperature)
 * Bit-bangs the 1-wire protocol to fetch the 40-bit DHT11 packet.
 * On success: fills *humidity (0-100 %) and *temperature (0-50 C),
 *             returns HUMIDITY_OK.
 * On timeout or checksum mismatch: returns HUMIDITY_ERROR (pointers unchanged).
 */
u8 Humidity_Read(u8 *humidity, u8 *temperature);

#endif /* HUMIDITY_INTERFACE_H */