/*
 * Irrigation.c
 * Automatic and manual irrigation control.
 * Pump is active-LOW on RD0: LOW=ON, HIGH=OFF — always written via portd_shadow.
 * last_temp_c is updated every 2 s by main loop; used here to avoid 20 ms
 * DHT11 blocking reads inside the 100 ms pump safety loop.
 */

#include "../../config.h"
#include "Irrigation_interface.h"
#include "../Safety/Safety_interface.h"
#include "../Comms/Comms_interface.h"
#include "../../HAL/Motor/Motor_interface.h"
#include "../../HAL/Button/Button_interface.h"
#include "../../HAL/LCD/LCD_interface.h"
#include "../../HAL/Ultrasonic/Ultrasonic_interface.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "../../MCAL/MCU_Registers.h"
#include "../../SERVICES/BIT_MATH.h"
#include "../../SERVICES/STD_TYPES.h"

extern volatile u8 portd_shadow;
extern u8 last_temp_c;

static void pump_on(void)
{
    portd_shadow &= (u8)(~(u8)(1u << PIN_PUMP));
    PORTD = portd_shadow;
}

static void pump_off(void)
{
    portd_shadow |= (u8)(1u << PIN_PUMP);
    PORTD = portd_shadow;
}

static u8 estopped(void)
{
    return (u8)(Button_IsEstopped() || Comms_AppEstopActive());
}

/* Inner pump loop shared by both modes.
   Returns 1 if aborted (locked/estopped), 0 if completed normally. */
static u8 run_pump(u8 plant_index)
{
    u8  pump_ticks = 0u;
    u8  soil;
    u16 curr;
    u8  water;

    pump_on();
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Watering P");
    LCD_SendNumber((s16)plant_index);
    LCD_SendString_Const("      ");

    while(pump_ticks < PUMP_ON_TICKS)
    {
        __delay_ms(100);
        Button_Poll();
        Comms_Poll();

        if(estopped() || Safety_IsLocked()) {
            pump_off();
            return 1u;
        }

        soil  = ADC_SoilPct(ADC_Read(ADC_CH_SOIL));
        curr  = ADC_CurrentmA(ADC_Read(ADC_CH_CURRENT));
        water = Ultrasonic_GetWaterLevel();
        Safety_RunChecks(soil, last_temp_c, curr, water);

        if(Safety_IsLocked()) {
            pump_off();
            return 1u;
        }
        pump_ticks++;
    }

    pump_off();
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Done  P");
    LCD_SendNumber((s16)plant_index);
    LCD_SendString_Const("         ");
    return 0u;
}

void Irrigation_RunCycle(void)
{
    u8 i, j;
    u8 soil;

    /* Skip if soil already wet enough */
    soil = ADC_SoilPct(ADC_Read(ADC_CH_SOIL));
    if(soil >= SOIL_SKIP_PCT) {
        LCD_GoToRowCol(1u, 1u);
        LCD_SendString_Const("Soil OK-Skipping");
        return;
    }

    /* Home gantry */
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Homing...       ");
    Motor_Home();
    if(estopped() || Safety_IsLocked()) goto abort;

    /* Irrigate each plant */
    for(i = 0u; i < NUM_PLANTS; i++)
    {
        Motor_MoveTo(i);
        if(estopped() || Safety_IsLocked()) goto abort;

        /* Settle */
        for(j = 0u; j < SETTLE_MS_LOOP; j++) { __delay_ms(100); }

        Comms_SendAtPlant(i);

        if(run_pump(i)) goto abort;

        /* Short pause between plants */
        for(j = 0u; j < 5u; j++) { __delay_ms(100); }
    }

    Motor_Home();
    Motor_Disable();
    return;

abort:
    pump_off();
    Motor_Disable();
}

void Irrigation_RunSinglePlant(u8 plant_index)
{
    u8 j;

    if(plant_index >= NUM_PLANTS) return;

    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Manual: P");
    LCD_SendNumber((s16)plant_index);
    LCD_SendString_Const("       ");

    Motor_Home();
    if(estopped() || Safety_IsLocked()) goto abort;

    Motor_MoveTo(plant_index);
    if(estopped() || Safety_IsLocked()) goto abort;

    for(j = 0u; j < SETTLE_MS_LOOP; j++) { __delay_ms(100); }

    Comms_SendAtPlant(plant_index);

    if(run_pump(plant_index)) goto abort;

    Motor_Home();
    Motor_Disable();
    Comms_SendStatus(MODE_MANUAL, Safety_IsLocked());
    return;

abort:
    pump_off();
    Motor_Disable();
    Comms_SendStatus(MODE_MANUAL, Safety_IsLocked());
}
