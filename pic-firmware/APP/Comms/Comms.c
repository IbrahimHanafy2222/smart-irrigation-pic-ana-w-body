/*
 * Comms.c
 * Pi ↔ PIC packet layer over UART (9600 baud polling mode).
 *
 * Pi → PIC: fixed 3-byte packet [0xBB][CMD][DATA]
 * PIC → Pi: variable packet [0xAA][TYPE][DATA...]
 *
 * Comms_Poll() reads at most 1 byte per call (non-blocking).
 * A complete 3-byte packet assembles across ≤ 3 loop ticks (≤ 300 ms).
 */

#include "../../config.h"
#include "Comms_interface.h"
#include "../../MCAL/USART/USART_interface.h"
#include "../../SERVICES/STD_TYPES.h"

static u8 parse_state  = 0u;   /* 0=await HDR, 1=await CMD, 2=await DATA */
static u8 parse_cmd    = 0u;

static u8 current_mode        = MODE_AUTO;
static u8 manual_cmd_pending  = 0u;
static u8 manual_plant        = 0u;
static u8 app_estop           = 0u;
static u8 handshake_received  = 0u;

/* ── Outbound ── */

void Comms_SendSensors(u8 soil, u8 temp, u8 hum, u16 curr, u8 water)
{
    UART_Write(UART_HDR_PIC_TO_PI);
    UART_Write(UART_TYPE_SENSORS);
    UART_Write(soil);
    UART_Write(temp);
    UART_Write(hum);
    UART_Write((u8)(curr >> 8));
    UART_Write((u8)(curr & 0xFFu));
    UART_Write(water);
}

void Comms_SendAtPlant(u8 plant_index)
{
    UART_Write(UART_HDR_PIC_TO_PI);
    UART_Write(UART_TYPE_AT_PLANT);
    UART_Write(plant_index);
}

void Comms_SendStatus(u8 mode, u8 lockout)
{
    UART_Write(UART_HDR_PIC_TO_PI);
    UART_Write(UART_TYPE_STATUS);
    UART_Write(mode);
    UART_Write(lockout);
}

/* ── Inbound parser (non-blocking, ≤1 byte per call) ── */

void Comms_Poll(void)
{
    u8 byte;

    if(!UART_DataAvailable()) return;
    byte = UART_Read();

    switch(parse_state) {
        case 0u:
            if(byte == UART_HDR_PI_TO_PIC) parse_state = 1u;
            break;
        case 1u:
            parse_cmd   = byte;
            parse_state = 2u;
            break;
        case 2u:
            switch(parse_cmd) {
                case UART_CMD_MODE:
                    current_mode = (byte == MODE_MANUAL) ? MODE_MANUAL : MODE_AUTO;
                    break;
                case UART_CMD_IRRIGATE:
                    if(byte < 5u) {
                        manual_plant       = byte;
                        manual_cmd_pending = 1u;
                    }
                    break;
                case UART_CMD_ESTOP:
                    app_estop = (byte != 0u) ? 1u : 0u;
                    break;
                case UART_CMD_HANDSHAKE:
                    if(byte == 0xAAu) { handshake_received = 1u; }
                    break;
                default: break;
            }
            parse_state = 0u;
            break;
        default:
            parse_state = 0u;
            break;
    }
}

/* ── Accessors ── */

u8 Comms_GetMode(void)              { return current_mode; }
u8 Comms_ManualCommandPending(void) { return manual_cmd_pending; }
u8 Comms_GetManualPlant(void)       { return manual_plant; }
u8 Comms_AppEstopActive(void)       { return app_estop; }
u8 Comms_HandshakeReceived(void)    { return handshake_received; }
void Comms_ClearManualCommand(void) { manual_cmd_pending = 0u; }

void Comms_SendHandshakeAck(void)
{
    UART_Write(UART_HDR_PIC_TO_PI);
    UART_Write(UART_TYPE_HANDSHAKE);
    UART_Write(0xBBu);
}
