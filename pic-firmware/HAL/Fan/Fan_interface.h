#ifndef FAN_INTERFACE_H
#define FAN_INTERFACE_H

/*
 * Cooling fan via NPN transistor on RC4 (active HIGH).
 * Controlled by Safety.c: ON at temp >= TEMP_WARN_C, OFF when temp clears.
 */
void Fan_Init(void);
void Fan_On(void);
void Fan_Off(void);

#endif /* FAN_INTERFACE_H */
