#include "Relay_interface.h"
#include "Relay_private.h"
#include "../../SERVICES/BIT_MATH.h"

void Relay_Init(void)
{
    CLR_BIT(RELAY_TRIS, RELAY_PIN); /* Set as OUTPUT (0) */
    CLR_BIT(RELAY_PORT, RELAY_PIN); /* Start INACTIVE (0) */
}

void Relay_On(void)
{
    SET_BIT(RELAY_PORT, RELAY_PIN);
}

void Relay_Off(void)
{
    CLR_BIT(RELAY_PORT, RELAY_PIN);
}

u8 Relay_GetState(void)
{
    return (u8)GET_BIT(RELAY_PORT, RELAY_PIN);
}