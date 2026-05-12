#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "../../SERVICES/STD_TYPES.h"

/* LED States */
#define LED_OFF    0
#define LED_ON     1
/* Warning LED: RE1 — uses GPIO_PORTE ID (4), not raw address */
#define LED_YELLOW_PORT    4      /* GPIO_PORTE */
#define LED_YELLOW_PIN     1      /* RE1 */

/* Fault/Lockout LED: RE2 */
#define LED_RED_PORT       4      /* GPIO_PORTE */
#define LED_RED_PIN        2      /* RE2 */

void LED_Init(u8 Port, u8 Pin);
void LED_On(u8 Port, u8 Pin);
void LED_Off(u8 Port, u8 Pin);
void LED_Toggle(u8 Port, u8 Pin);

#endif