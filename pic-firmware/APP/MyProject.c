/*
 * MyProject.c
 * Smart Irrigation Controller — PIC16F877A @ 8 MHz
 * Compiler: MPLAB XC8 v2.36
 *
 * config.h must come first so _XTAL_FREQ is defined before xc.h
 * (required by __delay_ms/__delay_us in every included header).
 */

#include "../config.h"
#include "../MCAL/MCU_Registers.h"

/* XC8 configuration bits for PIC16F877A */
#pragma config FOSC = HS /* High-Speed crystal */
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF  /* disabled for PSU debug — re-enable when on bench supply */
#pragma config LVP = OFF /* LVP=OFF frees RB3 for limit switch */
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include "Safety/Safety_interface.h"
#include "Comms/Comms_interface.h"
#include "Irrigation/Irrigation_interface.h"
#include "../HAL/LCD/LCD_interface.h"
#include "../HAL/Buzzer/Buzzer_interface.h"
#include "../HAL/Button/Button_interface.h"
#include "../HAL/Fan/Fan_interface.h"
#include "../HAL/Humidity/Humidity_interface.h"
#include "../HAL/Ultrasonic/Ultrasonic_interface.h"
#include "../HAL/Motor/Motor_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/USART/USART_interface.h"
#include "../SERVICES/STD_TYPES.h"
#include "../SERVICES/BIT_MATH.h"

/* ── Globals shared with LCD.c, Safety.c, Irrigation.c, Button.c ── */
volatile u8 portd_shadow = 0xFFu; /* RD0=pump OFF, RD1=spare OFF */
u8 last_temp_c = 25u;             /* Updated every 2 s; safe init value */

/* ── Interrupt_Manager.c externs (kept to satisfy linker) ── */
volatile u8 g_tick_100ms = 0u;
u8 g_frame[4] = {0u};
u8 g_frame_idx = 0u;
u8 g_frame_ready = 0u;

/* ── TEST 2 MAIN — NodeMCU → PIC handshake reception verifier ──
 * NodeMCU sends [0xBB][0x10][0xAA] every 200ms through level converter.
 * This main detects the 3-byte handshake pattern and displays it on LCD.
 *
 * LCD layout:
 *   Row 1: "Last:0xXX Cnt:NNN"   ← updates on every byte received
 *   Row 2: "HANDSHAKE x NN"      ← increments when full BB-10-AA seen
 *          OR "No handshake yet" ← before first complete pattern
 *
 * Pass:   LCD row 2 increments every ~200ms → PIC + converter work.
 * Fail:   Row 1 stays at "Cnt:0" → no bytes at all (converter or PIC dead).
 *         Row 1 increments but row 2 stays 0 → bytes received but garbage
 *                                              (baud mismatch / framing error).
 */
static void lcd_hex(u8 val)
{
    const char hex[] = "0123456789ABCDEF";
    LCD_SendString_Const("0x");
    LCD_SendChar((u8)hex[val >> 4]);
    LCD_SendChar((u8)hex[val & 0x0Fu]);
}

void main(void)
{
    u8  rx_byte    = 0u;
    u16 rx_count   = 0u;
    u8  hs_state   = 0u;     /* handshake FSM: 0=idle, 1=saw BB, 2=saw BB+10 */
    u16 hs_count   = 0u;     /* full BB-10-AA sequences seen */
    u8  i          = 0u;

    ADCON1 = ADCON1_CONFIG;
    TRISA  = 0x07u;
    TRISB  = 0b00011011u;
    TRISC  = 0b10000000u;    /* RC7 = RX input, RC6 = TX output */
    TRISD  = 0x00u;
    TRISE  = 0x00u;

    /* PIC alive check: blink RE1 (yellow LED) 3x before LCD init.
     * If LED blinks → PIC is running → problem is LCD contrast or wiring.
     * If LED does NOT blink → power/BOREN/crystal issue. */
    for(i = 0u; i < 3u; i++)
    {
        SET_BIT(PORTE, 1u);
        __delay_ms(200);
        CLR_BIT(PORTE, 1u);
        __delay_ms(200);
    }

    CLR_BIT(OPTION_REG, 7u); /* Enable PORTB pull-ups */
    PORTD  = portd_shadow;
    SET_BIT(PORTC, 6u);      /* UART TX idle HIGH */

    LCD_Init();
    UART_Init();

    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("BiDir Test      ");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const("Waiting for Pi  ");

    /* ── Bidirectional test ──────────────────────────────────────────
     * Pi sends  [0xBB][0x10][0xAA] every few seconds.
     * PIC replies [0xAA][0x10][0xBB] immediately on receipt.
     * LCD row 1: RX count (handshakes received from Pi)
     *     row 2: TX count (ACKs sent back to Pi)
     *
     * Tight inner drain loop reads all pending bytes before any LCD
     * update — prevents FIFO overrun when all 3 bytes arrive together.
     * ─────────────────────────────────────────────────────────────── */
    while(1)
    {
        /* Drain every waiting byte before touching the LCD */
        while(UART_DataAvailable())
        {
            rx_byte = UART_Read();

            switch(hs_state)
            {
                case 0u:
                    if(rx_byte == 0xBBu) hs_state = 1u;
                    break;
                case 1u:
                    if(rx_byte == 0x10u)      hs_state = 2u;
                    else if(rx_byte == 0xBBu) hs_state = 1u;
                    else                      hs_state = 0u;
                    break;
                case 2u:
                    if(rx_byte == 0xAAu)
                    {
                        UART_Write(0xAAu);
                        UART_Write(0x10u);
                        UART_Write(0xBBu);
                        if(rx_count < 0xFFFFu) rx_count++;
                        if(hs_count < 0xFFFFu) hs_count++;
                    }
                    hs_state = 0u;
                    break;
                default:
                    hs_state = 0u;
                    break;
            }
        }
        UART_ClearOverrun();

        /* LCD update once per 50 ms outer tick */
        LCD_GoToRowCol(1u, 1u);
        LCD_SendString_Const("RX:");
        LCD_SendNumber((s16)rx_count);
        LCD_SendString_Const(" TX:");
        LCD_SendNumber((s16)hs_count);
        LCD_SendString_Const("    ");

        LCD_GoToRowCol(2u, 1u);
        if(rx_count == 0u)
            LCD_SendString_Const("Waiting for Pi  ");
        else
            LCD_SendString_Const("Link OK         ");

        __delay_ms(50);
    }
}

#if 0  /* PRODUCTION MAIN — change to #if 1 when UART verified */
void main(void)
{
    u16 tick = 0u;
    u8 lcd_page = 0u;
    u8 current_mode = MODE_AUTO;
    u8 new_mode;

    u8 soil_pct = 0u;
    u8 humidity = 50u;
    u8 water_cm = 20u;
    u16 curr_mA = 0u;

    /* ── Port configuration ── */
    ADCON1 = ADCON1_CONFIG; /* AN0-AN2 analog; RE1/RE2 digital */

    TRISA = 0x07u; /* RA0-RA2 input (analog); RA3-5 output */
    /* RB0=DHT11(in), RB1=echo(in), RB2=buzz(out), RB3=limit(in), RB4=estop(in) */
    TRISB = 0b00011011u;
    /* RC7=UART RX(in); all others output */
    TRISC = 0b10000000u;
    TRISD = 0x00u; /* All outputs (LCD + relays) */
    TRISE = 0x00u; /* RE1/RE2 LED outputs */

    /* Enable PORTB internal pull-ups (for RB3 limit switch, RB4 e-stop) */
    CLR_BIT(OPTION_REG, 7u);

    /* ── Initial output state ── */
    PORTD = portd_shadow;     /* Pump OFF (RD0=1), motor relay OFF (RD1=1) */
    CLR_BIT(PORTC, PIN_TRIG); /* HC-SR04 trigger idle LOW */
    CLR_BIT(PORTC, PIN_FAN);  /* Fan off */
    SET_BIT(PORTC, 6u);       /* UART TX idle HIGH */
    CLR_BIT(PORTB, PIN_BUZZ); /* Buzzer off */
    CLR_BIT(PORTE, PIN_LED_YLW);
    CLR_BIT(PORTE, PIN_LED_RED);

    /* ── Driver init ── */
    ADC_Init();
    UART_Init();
    LCD_Init();
    Safety_Init();
    Button_Init();
    Fan_Init();
    Humidity_Init();
    Buzzer_Init();
    Motor_Init();

    /* Splash screen */
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Smart Irrigation");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const(" Initializing...");
    {
        u8 s;
        for (s = 0u; s < 15u; s++)
        {
            __delay_ms(100);
        }
    }

    /* Wait for Pi handshake [0xBB][0x10][0xAA] before starting sensors */
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Waiting for Pi  ");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const("                ");
    {
        u8 rx_dbg = 0u;
        while (!Comms_HandshakeReceived())
        {
            UART_ClearOverrun();
            if (UART_DataAvailable())
            {
                rx_dbg++;
                LCD_GoToRowCol(2u, 1u);
                LCD_SendString_Const("RX:");
                LCD_SendNumber((s16)rx_dbg);
                LCD_SendString_Const("            ");
            }
            __delay_ms(100);
            Comms_Poll();
        }
    }
    Comms_SendHandshakeAck();

    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("PI Ready!       ");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const("Starting...     ");
    {
        u8 s;
        for (s = 0u; s < 15u; s++)
        {
            __delay_ms(100);
        }
    }
    LCD_Clear();

    /* Home gantry at startup — verifies motor + limit switch before first cycle */
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Homing...       ");
    LCD_GoToRowCol(2u, 1u);
    LCD_SendString_Const("                ");
    Motor_Home();
    Motor_Disable();

    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("Home OK         ");
    {
        u8 s;
        for (s = 0u; s < 10u; s++)
        {
            __delay_ms(100);
        }
    }
    LCD_Clear();

    Comms_SendStatus(MODE_AUTO, 0u);

    /* ══════════════════════════════════════
       MAIN LOOP  (1 tick = 100 ms)
       ══════════════════════════════════════ */
    while (1)
    {
        __delay_ms(100);
        tick++;

        /* ── 1. Hardware e-stop (every tick) ── */
        Button_Poll();

        /* ── 2. Pi commands (every tick, non-blocking) ── */
        Comms_Poll();

        /* Mode switch from app */
        new_mode = Comms_GetMode();
        if (new_mode != current_mode)
        {
            current_mode = new_mode;
            Comms_SendStatus(current_mode, Safety_IsLocked());
            LCD_GoToRowCol(2u, 1u);
            if (current_mode == MODE_AUTO)
                LCD_SendString_Const("Auto Mode OK    ");
            else
                LCD_SendString_Const("Manual Mode     ");
        }

        /* Manual irrigate command */
        if (current_mode == MODE_MANUAL && Comms_ManualCommandPending())
        {
            u8 plant = Comms_GetManualPlant();
            Comms_ClearManualCommand();
            if (!Safety_IsLocked() && !Button_IsEstopped() && !Comms_AppEstopActive())
            {
                Irrigation_RunSinglePlant(plant);
            }
        }

        /* ── 3. Sensor read + safety (every 2 s) ── */
        if ((tick % SENSOR_PERIOD_TICKS) == 0u)
        {
            soil_pct = ADC_SoilPct(ADC_Read(ADC_CH_SOIL));
            curr_mA = ADC_CurrentmA(ADC_Read(ADC_CH_CURRENT));
            water_cm = Ultrasonic_GetWaterLevel();
            Humidity_Read(&humidity, &last_temp_c);

            Safety_RunChecks(soil_pct, last_temp_c, curr_mA, water_cm);
            Comms_SendSensors(soil_pct, last_temp_c, humidity, curr_mA, water_cm);

            /* LCD line 1: rotate sensor pages */
            LCD_GoToRowCol(1u, 1u);
            switch (lcd_page % 5u)
            {
            case 0u:
                LCD_SendString_Const("Soil:");
                LCD_SendNumber((s16)soil_pct);
                LCD_SendString_Const("%          ");
                break;
            case 1u:
                LCD_SendString_Const("Temp:");
                LCD_SendNumber((s16)last_temp_c);
                LCD_SendString_Const("C          ");
                break;
            case 2u:
                LCD_SendString_Const("Hum :");
                LCD_SendNumber((s16)humidity);
                LCD_SendString_Const("%          ");
                break;
            case 3u:
                LCD_SendString_Const("Curr:");
                LCD_SendNumber((s16)curr_mA);
                LCD_SendString_Const("mA     ");
                break;
            case 4u:
                LCD_SendString_Const("Watr:");
                LCD_SendNumber((s16)water_cm);
                LCD_SendString_Const("cm         ");
                break;
            default:
                break;
            }
            lcd_page++;

            /* LCD line 2: overall status */
            if (!Safety_IsLocked())
            {
                LCD_GoToRowCol(2u, 1u);
                if (Button_IsEstopped() || Comms_AppEstopActive())
                    LCD_SendString_Const("E-STOP ACTIVE   ");
                else if (current_mode == MODE_MANUAL)
                    LCD_SendString_Const("Manual Mode     ");
                else
                    LCD_SendString_Const("Auto Mode OK    ");
            }
        }

        /* ── 4. Automatic irrigation cycle (every 3 min) ── */
        if (current_mode == MODE_AUTO && tick >= IRRIG_PERIOD_TICKS)
        {
            tick = 0u;
            if (!Safety_IsLocked() && !Button_IsEstopped() && !Comms_AppEstopActive())
            {
                Irrigation_RunCycle();
            }
        }

        /* Prevent stale trigger when switching back from manual */
        if (current_mode == MODE_MANUAL && tick >= IRRIG_PERIOD_TICKS)
        {
            tick = 0u;
        }
    }
}
/* ── END PRODUCTION MAIN ── */
#endif
