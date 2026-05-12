# Smart Irrigation Controller — Session Progress

## Hardware
- PIC16F877A @ 8 MHz external crystal
- Compiler: MPLAB XC8 v3.10
- Pi ↔ PIC UART at 9600 baud 8N1 (RC6=TX, RC7=RX)

## Issues Fixed This Session

### 1. PIC Reset Loop (brown-out)
**Symptom:** LCD cycled between "Smart Irrigation Initializing" and "Waiting for Pi" indefinitely.  
**Root cause:** `BOREN = ON` in config bits. Power supply was too weak — voltage dipped and triggered brown-out reset every ~1.5 s.  
**Fix:** Replaced power supply with lab bench unit. Hardware fix: add 100 nF ceramic + 10–47 µF electrolytic near PIC VDD/VSS pins. Also verify MCLR (pin 1) has 10 kΩ pull-up to VDD.

### 2. UART Overrun (OERR lockup)
**Symptom:** Handshake never received even when Pi was sending.  
**Root cause:** If Pi sends bytes before PIC UART is ready, `RCSTA.OERR` (bit 1) sets and receiver stalls permanently.  
**Fix:** Added `UART_ClearOverrun()` in `MCAL/USART/USART.c` + declaration in `USART_interface.h`. Called at top of handshake wait loop in `APP/MyProject.c`.

```c
// USART.c — added
void UART_ClearOverrun(void)
{
    if(GET_BIT(RCSTA, 1u)) {   /* OERR = RCSTA<1> */
        CLR_BIT(RCSTA, CREN);
        SET_BIT(RCSTA, CREN);
    }
}
```

### 3. LCD Sign-Conversion Warning
**File:** `HAL/LCD/LCD.c:121`  
**Fix:** Cast `num % 10` to `(u8)` — safe because negative case is handled before the loop.

## Current Status
- Build: **clean** (warnings only, no errors)
- Program space: 64.3% | RAM: 27.4%
- Reset loop: **fixed** (power supply)
- UART overrun: **fixed**
- Handshake: **NOT yet working** — Pi has no script to send `[0xBB][0x10][0xAA]`

## Pending

### Pi Handshake Script (not written yet)
PIC waits for exactly: `0xBB 0x10 0xAA` on UART.  
PIC responds with: `0xAA 0x10 0xBB` (handshake ACK).  
Pi serial port must be 9600 baud 8N1 (`/dev/ttyS0` or `/dev/ttyAMA0`).

Quick test one-liner (from Pi terminal):
```bash
python3 -c "
import serial, time
s = serial.Serial('/dev/ttyS0', 9600, timeout=1)
time.sleep(0.5)
s.write(bytes([0xBB, 0x10, 0xAA]))
ack = s.read(3)
print('ack:', ack.hex())
"
```

Full Pi app (to be written): handshake + receive sensor packets + send mode/irrigate/estop commands.

## Protocol Reference

### Pi → PIC  `[0xBB][CMD][DATA]`
| CMD  | DATA | Meaning |
|------|------|---------|
| 0x01 | 0x00/0x01 | Set mode: auto/manual |
| 0x02 | 0x00–0x04 | Manual irrigate plant N |
| 0x03 | 0x00/0x01 | E-stop release/activate |
| 0x10 | 0xAA | Handshake |

### PIC → Pi  `[0xAA][TYPE][DATA...]`
| TYPE | DATA | Meaning |
|------|------|---------|
| 0x01 | soil, temp, hum, curr_h, curr_l, water | Sensor packet (6 bytes) |
| 0x02 | plant_index | At-plant notification |
| 0x03 | mode, lockout | Status update |
| 0x10 | 0xBB | Handshake ACK |
