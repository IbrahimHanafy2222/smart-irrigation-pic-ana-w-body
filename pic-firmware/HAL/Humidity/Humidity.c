/*
 * Humidity.c
 * Irrigation Project � DHT11/DHT22 Bit-Banging Driver
 * Target: PIC16F877A @ 8 MHz | MikroC Pro for PIC
 */

#include "Humidity_interface.h"
#include "Humidity_private.h"
#include "Humidity_config.h"
#include "../../SERVICES/BIT_MATH.h"

void Humidity_Init(void)
{
    /* Start with the pin as input to let pull-up keep it HIGH */
    SET_BIT(HUMIDITY_DATA_TRIS, HUMIDITY_DATA_PIN);
}

u8 Humidity_Read(u8 *humidity, u8 *temperature)
{
    /* FIXED: 'data' is a reserved keyword in MikroC. Renamed to 'dht_data' */
    u8 dht_data[5] = {0, 0, 0, 0, 0};
    u8 i = 0, j = 0;
    u16 timeout = 0;

    /* 1. MCU sends START signal */
    CLR_BIT(HUMIDITY_DATA_TRIS, HUMIDITY_DATA_PIN); /* Pin to Output */
    CLR_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN); /* Pull LOW */
    __delay_ms(18);                                   /* Min 18ms for DHT11 */

    SET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN); /* Release HIGH */
    __delay_us(30);                                   /* Wait 20-40us */

    SET_BIT(HUMIDITY_DATA_TRIS, HUMIDITY_DATA_PIN); /* Pin to Input */

    /* 2. Sensor Response */
    /* Wait for Sensor to pull LOW */
    timeout = 0;
    while(GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
    {
        __delay_us(2);
        if(++timeout > 200) return HUMIDITY_ERR_RESPONSE;
    }

    /* Wait for Sensor to pull HIGH */
    timeout = 0;
    while(!GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
    {
        __delay_us(2);
        if(++timeout > 200) return HUMIDITY_ERR_RESPONSE;
    }

    /* Wait for Sensor to pull LOW again (Start of bit transmission) */
    timeout = 0;
    while(GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
    {
        __delay_us(2);
        if(++timeout > 200) return HUMIDITY_ERR_RESPONSE;
    }

    /* 3. Read 40 bits of data */
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            /* Wait for bit start (LOW -> HIGH) */
            timeout = 0;
            while(!GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
            {
                __delay_us(2);
                if(++timeout > 200) return HUMIDITY_ERR_BITREAD;
            }

            /* * Measure HIGH duration:
             * DHT11 '0' = ~28us HIGH
             * DHT11 '1' = ~70us HIGH
             * We wait 35us. If it's still HIGH, it must be a '1'.
             */
            __delay_us(35);

            if(GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
            {
                dht_data[i] |= (1 << (7 - j)); /* Set the bit */

                /* Wait for pin to go LOW again to finish the bit */
                timeout = 0;
                while(GET_BIT(HUMIDITY_DATA_PORT, HUMIDITY_DATA_PIN))
                {
                    __delay_us(2);
                    if(++timeout > 200) return HUMIDITY_ERR_BITREAD;
                }
            }
        }
    }

    /* 4. Verify Checksum */
    if((u8)(dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) != dht_data[4])
    {
        return HUMIDITY_ERR_CHECKSUM;
    }

    /* 5. Fill output pointers — dht_data[0]=hum_int, dht_data[2]=temp_int */
    *humidity    = dht_data[0];
    *temperature = dht_data[2];

    return HUMIDITY_OK;
}