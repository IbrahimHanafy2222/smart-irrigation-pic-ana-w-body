# UART Debug Plan — PIC16F877A ↔ Raspberry Pi 4

**Symptom:** Pi sends `[0xBB][0x10][0xAA]` continuously. PIC LCD RX counter never increments. Pi-side loopback (GPIO14→GPIO15) works fine.

This document is a **layered triage**. Run tests in order. Each test isolates one layer. STOP at the first test that fails — that's your root cause.

---

## Mental Model

```
[Pi GPIO14 TX 3.3V]
       │
       ▼
[Level converter LV/TXI]
       │  (BSS138 MOSFET + pull-ups)
       ▼
[Level converter HV/TXO 5V]
       │
       ▼
[PIC RC7/RX]
       │
       ▼
[USART module: SPEN, CREN, RCIF, RCREG]
       │
       ▼
[Application: UART_DataAvailable() reads PIR1.RCIF]
```

Failure can be at **any** stage. Loopback test proved Pi side TX works only **at the GPIO pin**. Everything downstream is unverified.

---

## Hypothesis Ranking

| # | Hypothesis | Likelihood | Why |
|---|---|---|---|
| 1 | Level converter miswired, dead channel, or unpowered | **HIGH** | Hand-wired modules, easy to flip TXI/TXO. BSS138 needs both LV and HV powered. |
| 2 | RC7 input voltage never reaches VIH (4.0 V) | **HIGH** | PIC16F877A Schmitt-trigger input VIH = 0.8×VDD = **4.0 V at VDD=5V**. If converter outputs only 3.3 V, line never reads HIGH. |
| 3 | PIC oscillator not running at 8 MHz | **MED** | Wrong baud rate. Detect by blinking LED at 1 Hz with `__delay_ms(500)` and timing it. |
| 4 | Pi serial console still bound to ttyAMA0 | **MED** | `console=serial0,115200` in cmdline.txt corrupts the line with kernel logs. |
| 5 | Common ground missing (Pi, converter, PIC) | **MED** | All three rails must share GND. Floating GND = garbage signal. |
| 6 | UART RX line floating (no pull-up at PIC end) | **LOW-MED** | RC7 has **no internal pull-up**. If converter HV side is high-impedance when idle, line floats. |
| 7 | Wrong UART direction on Pi GPIO | **LOW** | `pinctrl get 14 15` already confirmed TXD0/RXD0 alt mode. |
| 8 | PIC config bits or SPEN not actually set | **LOW** | Code looks correct. Could verify by reading RCSTA back to LCD. |
| 9 | Software bug in `UART_DataAvailable()` | **LOW** | Reads `PIR1.RCIF` — correct flag. Code is trivial. |

---

## Critical Datasheet Fact (often missed)

**PIC16F877A PORTC input thresholds (Schmitt-trigger):**

| Parameter | Min | Max | At VDD = 5 V |
|---|---|---|---|
| VIH (high) | 0.8 × VDD | — | **4.0 V** |
| VIL (low) | — | 0.2 × VDD | 1.0 V |

**Implication:** a 3.3 V signal going directly into RC7 is **below VIH**. PIC may read it as indeterminate. The level converter MUST raise the signal to ~5 V for reliable RX. If the converter is broken/miswired and only passes 3.3 V through, the PIC won't see the start bit.

This is the #1 cause to chase.

---

# Debug Sequence

## STAGE A — Verify PIC is alive and oscillator runs

### A.1 — Blink test (sanity)

Replace the UART test main with a 1 Hz LED blink. Use RE1 (yellow LED, already wired).

```c
void main(void)
{
    TRISE = 0x00;
    while(1) {
        SET_BIT(PORTE, 1u);
        __delay_ms(500);
        CLR_BIT(PORTE, 1u);
        __delay_ms(500);
    }
}
```

**Pass:** LED blinks at exactly 1 Hz (use stopwatch / phone). If 10 blinks take 10.0 s ±0.1 s → oscillator OK, baud rate will be correct.
**Fail:** Blinks faster → crystal is higher freq than 8 MHz. Slower → internal RC fallback (HS config not engaging, crystal not oscillating). Fix crystal/caps/PCB.

### A.2 — TX test (PIC → Pi)

Modify test main to **continuously transmit** `0x55` (alternating bit pattern = easiest to scope):

```c
void main(void)
{
    /* port setup as before */
    UART_Init();
    LCD_Init();
    LCD_GoToRowCol(1u, 1u);
    LCD_SendString_Const("TX 0x55 loop    ");

    while(1) {
        UART_Write(0x55);
        __delay_ms(100);
    }
}
```

On Pi side:

```bash
sudo systemctl stop serial-getty@ttyAMA0
stty -F /dev/ttyAMA0 9600 raw -echo
hexdump -C /dev/ttyAMA0
```

**Pass:** Pi sees a stream of `55 55 55 ...`. PIC TX hardware works. UART module configured correctly. Problem is **RX-side only** (converter or RC7).
**Fail:** Pi sees nothing OR garbage. PIC UART module is broken: bad config, wrong oscillator, or RC6 pin issue.
**Garbage but consistent pattern:** baud mismatch. Oscillator wrong.

This is the **single most informative test**. Run this first after A.1.

---

## STAGE B — Pi-side verification (FULL)

Pi 4 has **two UART hardware blocks**:
- **PL011** (`ttyAMA0`) — full-featured, stable baud, recommended.
- **Mini UART** (`ttyS0`) — baud derived from VPU clock, drifts under CPU load. Avoid.

By default on Pi 4: PL011 is wired to Bluetooth, mini UART exposed on GPIO14/15. To swap them (PL011 → GPIO14/15), use `dtoverlay=disable-bt` in config.txt. After that, `ttyAMA0` is the right port.

### B.1 — Verify which UART is on GPIO14/15

```bash
ls -l /dev/serial*
```

Expected output (if disable-bt applied):
```
/dev/serial0 -> ttyAMA0
/dev/serial1 -> ttyAMA10   (or similar)
```

`serial0` always points to the UART on the GPIO header. Use `/dev/serial0` in your Python script — symlink survives kernel renames.

```bash
dmesg | grep -i uart
dmesg | grep -i tty
```

Look for lines like `uart-pl011 fe201000.serial: ttyAMA0 at MMIO`. If you see `serial0-0` mapped to `ttyS0`, the disable-bt overlay didn't load → check config.txt path (`/boot/firmware/config.txt` on Bookworm, `/boot/config.txt` on older Raspbian).

### B.2 — Verify config.txt

```bash
cat /boot/firmware/config.txt | grep -Ei 'uart|bt'
```

Required:
```
enable_uart=1
dtoverlay=disable-bt
```

Optional but useful: `init_uart_baud=9600` (sets default).

After edits → reboot. Soft reload (`dtoverlay` command) does NOT survive a real reboot.

### B.3 — Disable serial console

This is the **#1 Pi-side bug**. Linux dumps kernel boot messages + getty login prompt to the serial port if console is bound. Garbage stream corrupts your handshake bytes.

```bash
cat /boot/firmware/cmdline.txt
```

Look for any of these — remove them:
- `console=serial0,115200`
- `console=ttyAMA0,115200`
- `console=ttyS0,115200`

Keep `console=tty1` (that's the HDMI console — different thing).

Also disable getty:
```bash
sudo systemctl stop  serial-getty@ttyAMA0.service
sudo systemctl stop  serial-getty@ttyS0.service
sudo systemctl disable serial-getty@ttyAMA0.service
sudo systemctl disable serial-getty@ttyS0.service
sudo systemctl status serial-getty@ttyAMA0.service   # must show "disabled; inactive"
sudo reboot
```

The easy way: `sudo raspi-config` → **Interface Options → Serial Port** → "login shell over serial?" **No** → "serial port hardware enabled?" **Yes** → reboot.

### B.4 — Check who owns the port

```bash
sudo fuser -v /dev/ttyAMA0
sudo lsof /dev/ttyAMA0
```

Empty output = port free ✓. If anything is listed (getty, ModemManager, gpsd, anything), kill it. Common offenders:
```bash
sudo systemctl stop ModemManager
sudo systemctl disable ModemManager
sudo systemctl stop hciuart       # if BT not disabled, hciuart hogs ttyAMA0
```

### B.5 — Check user permissions

```bash
groups
```

User must be in `dialout` group to access `/dev/ttyAMA0` without sudo. If missing:
```bash
sudo usermod -a -G dialout $USER
# log out and back in
```

Without this, `pyserial` opens the port but writes silently fail (or throw permission errors).

### B.6 — Verify pin alt function

```bash
pinctrl get 14 15
```

Expected:
```
14: a0    pu | hi // GPIO14 = TXD0
15: a0    pu | lo // GPIO15 = RXD0
```

`a0` = alt function 0 = UART0 on Pi 4 (PL011 when disable-bt applied). If you see `ip` (input) or `op` (output), the UART driver didn't claim the pin → overlay misconfigured.

For mini UART (ttyS0), alt function is `a5`. If you see `a5` on GPIO14 and you wanted PL011, fix overlay.

### B.7 — Confirm baud rate on the wire

Pi's actual baud may not match what you ask for. Test by writing a known pattern:

```bash
sudo systemctl stop serial-getty@ttyAMA0
stty -F /dev/ttyAMA0 9600 raw -echo -echoe -echok -echoctl -echoke
echo -en '\x55\x55\x55\x55' > /dev/ttyAMA0
```

Scope GPIO14: should see square wave at 4800 Hz (0x55 = 01010101, each bit at 9600 → toggle every 104 µs → effective square wave 4800 Hz). No scope? Use a USB-TTL adapter (cheap CH340/CP2102) on another computer and `hexdump -C /dev/ttyUSB0`. Should see `55 55 55 55`.

If you see `55` at 9600 baud → Pi TX works end-to-end. Proceed to converter tests.
If you see different bytes / nothing / wrong baud → Pi-side problem, NOT the PIC.

### B.8 — Loopback at Pi GPIO

Already done by user. Pi loopback (GPIO14 → GPIO15 jumper) works. Confirms Pi UART block alive and Python script writes properly.

Caveat: loopback proves only the Pi's **own kernel UART path**. Does NOT prove signal exits the pad with sufficient drive strength or correct voltage. Use B.7 (external receiver) for that.

### B.9 — Confirm Pi's TX voltage levels under load

```bash
# while handshake.py is running:
sudo cat /sys/kernel/debug/gpio | grep 14
```

Won't tell you the analog voltage but confirms direction is OUT. Combined with multimeter on GPIO14 idle (3.28 V seen) and brief dips during TX → physical signal exists at the Pi pad.

### B.10 — Pyserial sanity

```bash
python3 -c "import serial; s=serial.Serial('/dev/ttyAMA0',9600,timeout=1); s.write(b'\\xbb\\x10\\xaa'); print('sent'); s.close()"
```

If this raises an exception, port is wrong / permissions wrong / kernel module missing.

### B.11 — Kernel module loaded?

```bash
lsmod | grep -i uart
lsmod | grep -i serial
```

PL011 is built-in to the kernel on RPi OS (not a module), so probably no output. That's OK. If you previously installed a custom serial driver, unload it.

---

## STAGE C — Level converter verification (HIGHEST PRIORITY)

### C.1 — Verify rails

With converter powered (Pi running, PIC powered):

| Measurement | Expected | Action if wrong |
|---|---|---|
| LV pin to GND | **3.3 V ± 0.1** | LV not connected to Pi 3.3 V rail. Fix wiring. |
| HV pin to GND | **5.0 V ± 0.1** | HV not connected to PIC 5 V rail. Fix wiring. |
| GND (all three: Pi, converter, PIC) | continuity | **Critical.** All grounds MUST be common. |

### C.2 — Verify idle levels

UART idle is HIGH. With **nothing transmitting**:

| Pin | Expected | Wrong → |
|---|---|---|
| TXI / channel 1 LV side (Pi TX input) | 3.3 V | Pi TX not connected / wrong GPIO |
| TXO / channel 1 HV side (PIC RX output) | **5.0 V** | Converter dead, mis-powered, or miswired. ROOT CAUSE LIKELY. |

If TXO sits at 3.3 V → HV pin not actually at 5 V, OR the MOSFET is dead, OR the pull-up to HV is missing.
If TXO sits at 0 V → converter shorted or output stuck.
If TXO sits at 5 V → good, proceed to C.3.

### C.3 — Verify the converter switches

With Pi NOT transmitting:

1. Connect TXI to GND with a jumper.
2. Measure TXO. Should drop from 5 V to near 0 V.
3. Remove jumper. TXO should snap back to 5 V.

**Fail:** converter is dead. Replace it. (Or swap channels — try the other channel if 2-channel module.)

### C.4 — Direct converter wiring sanity

Common BSS138 module pin layout (verify yours matches):

```
LV ─┐                 ┌─ HV
GND ┤                 ├─ GND
A1  ┤  3.3 V ↔ 5 V    ├─ B1
A2  ┤  3.3 V ↔ 5 V    ├─ B2
A3  ┤                 ├─ B3
A4  ┤                 ├─ B4
```

Each `An ↔ Bn` is a **bidirectional channel**. There is no fixed direction. Connect:

- A1 → Pi GPIO14 (TX)
- B1 → PIC RC7 (RX)
- A2 → Pi GPIO15 (RX)
- B2 → PIC RC6 (TX)
- LV → Pi 3.3 V
- HV → PIC 5 V
- All GND together

If your module labels pins `TXI/RXI/TXO/RXO`, it is a **directional** converter (not BSS138). Likely:

- TXI = TX input (low side, from Pi TX)
- TXO = TX output (high side, to PIC RX)
- RXI = RX input (high side, from PIC TX)
- RXO = RX output (low side, to Pi RX)

In that case: Pi GPIO14 → TXI, TXO → RC7, RC6 → RXI, RXO → Pi GPIO15.

**Swap test:** if RX direction broken, try the OTHER channel (A2/B2 or RXI/RXO) just to rule out a single dead MOSFET.

### C.5 — Bypass converter as a diagnostic ONLY

**Warning:** PIC VIH=4.0V means Pi 3.3V is below spec. May not work, but worth a 30-second test:

1. Disconnect TXO from RC7.
2. Connect Pi GPIO14 **directly** to RC7 through a 470 Ω series resistor (current limit if shorts).
3. Add a 4.7 kΩ pull-up from RC7 to **+5 V** (this lifts the idle level and "pulls up" the 3.3 V HIGH closer to 5 V via overshoot — crude but sometimes enough).
4. Run handshake.py. Watch LCD.

**Pass:** PIC receives bytes. Converter is definitely the problem — replace it.
**Fail:** problem is downstream of RC7 (PIC UART config or oscillator).

---

## STAGE D — PIC-side verification

### D.1 — Read back UART registers to LCD

Add to UART test main, after `UART_Init()`:

```c
LCD_GoToRowCol(1u, 1u);
LCD_SendString_Const("TX:");
lcd_hex(TXSTA);
LCD_SendString_Const(" RX:");
lcd_hex(RCSTA);
LCD_GoToRowCol(2u, 1u);
LCD_SendString_Const("BRG:");
LCD_SendNumber((s16)SPBRG);
LCD_SendString_Const(" TC:");
lcd_hex(TRISC);
while(1) { /* halt */ }
```

**Expected:**
- `TXSTA = 0x24` (TXEN=1, BRGH=1, SYNC=0)
- `RCSTA = 0x90` (SPEN=1, CREN=1)
- `SPBRG = 51`
- `TRISC = 0x80` (RC7=in, RC6=out)

**If RCSTA bit 7 (SPEN) is 0** → USART not actually enabled. Pin still GPIO.
**If TRISC bit 7 is 0** → RC7 driven as output, can't receive.

### D.2 — Try `TRISC<6> = 1`

Microchip's **mid-range reference manual** (DS33023) section 18.1 states: *"TRISC<7:6> must be set in order to configure pins as USART."* Many tutorials say RC6 should be output (TRISC<6>=0). In practice both often work, but set both to 1 as a sanity check:

```c
TRISC = 0b11000000u;   /* RC6 AND RC7 = 1 — let USART module take over */
```

Re-flash. If this fixes RX → datasheet really does require both bits set.

### D.3 — Manual RC7 read (bypass USART)

Disable USART and read RC7 as GPIO to verify the signal physically arrives:

```c
void main(void)
{
    TRISC = 0b10000000u;
    LCD_Init();
    /* USART deliberately NOT initialized */
    CLR_BIT(RCSTA, 7u);   /* SPEN = 0, RC7 = pure GPIO input */

    while(1) {
        LCD_GoToRowCol(1u, 1u);
        if(GET_BIT(PORTC, 7u))
            LCD_SendString_Const("RC7 = HIGH      ");
        else
            LCD_SendString_Const("RC7 = LOW       ");
        __delay_ms(50);
    }
}
```

Without Pi transmitting: LCD should show `RC7 = HIGH` (UART idle = HIGH).
While Pi transmits at 9600: LCD will flicker between HIGH/LOW (rapid toggling, you'll see brief LOWs).

**Stays HIGH always (Pi running):** converter output stuck — no signal reaching RC7.
**Stays LOW always (Pi idle):** converter output is broken (stuck low). PIC will never see start bit ANYWAY because line is never HIGH = no idle = no edge detection.
**Stays at indeterminate voltage:** measure RC7 with multimeter. <3 V = below VIH. Need real 5 V on line.

This is the **smoking gun** test. If RC7 doesn't toggle physically, USART can't possibly receive.

### D.4 — Send 0x00 from Pi (all-zeros stress)

`0x00` byte = start bit + 8 zero bits + stop bit = 9 consecutive LOW bits + 1 HIGH bit. Easiest to detect with a slow multimeter showing voltage dip.

On Pi:
```bash
while true; do printf '\x00' > /dev/ttyAMA0; sleep 0.05; done
```

Measure RC7 voltage with multimeter. Should average lower than idle. If RC7 stays at solid 5 V → line is not toggling.

---

## STAGE E — Software & integration

### E.1 — Test `UART_DataAvailable()` against a known-toggling signal

Touch RC7 manually to GND (via 1 kΩ resistor for safety) while UART test main is running. The framing will be garbage but `RCIF` should set after enough edges. If counter increments at all under any condition → software path is fine.

### E.2 — Add OERR detection

OERR (overrun) lights up if RCREG isn't read fast enough. The test main reads RCREG every loop iteration. Should never overrun. But if a previous run left OERR set and CREN wasn't toggled, future bytes are silently dropped.

Already handled by `UART_ClearOverrun()`. Call it once before the main loop just to be safe:

```c
UART_Init();
UART_ClearOverrun();
LCD_Init();
```

### E.3 — Watchdog timer

WDTE=OFF in config bits. WDT is not the culprit. (Re-verify by reading the .hex header in MPLAB X or running `xc8 -mlist`.)

---

## Quick-fire 10-Minute Test Order

If you have only 10 minutes, do these in order:

1. **Multimeter on RC7 idle** — must read 5.0 V. (30 sec)
2. **Multimeter on HV pin** — must read 5.0 V. (30 sec)
3. **Continuity Pi-GND ↔ PIC-GND** — must beep. (30 sec)
4. **Run TX test (Stage A.2)** — confirms PIC UART module + oscillator. (2 min)
5. **Read RCSTA/TXSTA/SPBRG back to LCD (D.1)** — verify register state. (2 min)
6. **RC7 GPIO read test (D.3)** — confirms signal physically arrives. (2 min)
7. **Try `TRISC = 0xC0` (D.2)** — set both UART bits. (1 min)
8. **Swap converter channels** — try A2/B2 instead of A1/B1. (1 min)

If steps 1–3 pass and step 6 fails → **converter is dead. Replace it.**
If step 6 passes but RCIF never sets → **PIC software/config issue.** Re-examine.

---

## Replacement Options if Converter is Dead

| Option | Pros | Cons |
|---|---|---|
| New BSS138 module | Drop-in | Same failure mode possible |
| 74HC4050 (one-direction step-up) | Rock solid for Pi→PIC | Need 2 chips for bidir, or pair with divider |
| Resistor divider (1k+2k) for PIC→Pi | Trivial | One direction only — pair with 74HC4050 for other |
| 4-channel TXS0108E module | More robust, faster | Overkill for 9600 baud |
| **Quick hack:** 4.7 kΩ pull-up from RC7 to +5 V + Pi GPIO14 direct via 1 kΩ | Works at 9600 baud usually | Out of spec; not reliable long term |

For PIC TX (5 V) → Pi RX (3.3 V): never connect directly. Voltage divider (e.g., 1k + 2k) or level shifter required to avoid frying Pi GPIO15.

---

## STAGE F — Drivers & Kernel Layer

Short answer to "does this have to do with drivers?": **on Pi side yes, on PIC side no**.

### PIC side — no drivers

PIC firmware is bare-metal. `USART.c` directly writes hardware registers (TXSTA, RCSTA, SPBRG, RCREG). There is no OS, no driver, no abstraction layer. If registers are right and pin levels are right, hardware will receive. Nothing to "install" or "update" on PIC.

XC8 compiler version (v2.36) doesn't matter for UART — the registers are documented in the silicon. Worst case a compiler bug could miscompile your init, but `UART_Init()` is trivial and the listing file can be checked.

### Pi side — yes, multiple driver layers matter

Stack (top to bottom):

```
[Python pyserial]
       │
[Linux tty layer (n_tty line discipline)]
       │
[serial_core driver]
       │
[amba-pl011 driver (PL011)]  OR  [8250_bcm2835aux (mini UART)]
       │
[BCM2711 UART peripheral block]
       │
[GPIO pinmux — pinctrl-bcm2835]
       │
[Physical pad]
```

Things that can go wrong at each layer:

| Layer | Failure mode | How to detect |
|---|---|---|
| pyserial | Wrong port, wrong baud, no flush | `python3 -m serial.tools.miniterm /dev/ttyAMA0 9600` interactive test |
| tty line discipline | Cooked mode mangling bytes (CR/LF translation, XON/XOFF) | `stty -F /dev/ttyAMA0 -a` — must show `raw`, no `icrnl`, no `ixon` |
| serial_core | Buffer flooding, blocked writes | `dmesg \| tail -50` after run |
| pl011/8250 | Wrong baud divisor, FIFO config | check `dmesg \| grep -i pl011`, look for baud reported on probe |
| BCM peripheral | Clocks not enabled (rare) | `vcgencmd measure_clock uart` should be non-zero |
| pinmux | Wrong alt function on GPIO14/15 | `pinctrl get 14 15` — must show `a0` for PL011 |
| device tree | Overlay didn't load → wrong UART exposed | `ls -l /dev/serial*`, `dmesg \| grep -i overlay` |

### Critical Pi configuration items

| Item | Where | Required value |
|---|---|---|
| `enable_uart=1` | `/boot/firmware/config.txt` | Forces UART clock on |
| `dtoverlay=disable-bt` | `/boot/firmware/config.txt` | Moves PL011 from BT to GPIO14/15 |
| `core_freq=250` (only if mini UART) | `/boot/firmware/config.txt` | Locks VPU clock so mini UART baud is stable |
| Serial console removed | `/boot/firmware/cmdline.txt` | No `console=serial0,...` |
| `serial-getty@ttyAMA0` disabled | systemd | Stop login prompt on UART |
| User in `dialout` group | `/etc/group` | Permission to open port |
| `hciuart` stopped | systemd | If disable-bt active, this must not run |

### Line discipline gotcha

By default the tty driver translates `\n` ↔ `\r\n`, intercepts Ctrl-C, etc. `pyserial` opens in raw mode by default, but if you ever wrote to `/dev/ttyAMA0` with `echo` or `cat`, the line discipline mangled bytes. Always:

```bash
stty -F /dev/ttyAMA0 9600 raw -echo
```

before piping data with shell tools.

### Bluetooth driver gotcha (Pi 4 specific)

If `disable-bt` is NOT in config.txt:
- PL011 (ttyAMA0) is **bound to Bluetooth modem** by `hciuart` service.
- mini UART (ttyS0) is on GPIO14/15.
- `serial0` symlink points to `ttyS0`.

If `disable-bt` IS in config.txt:
- PL011 (ttyAMA0) is on GPIO14/15.
- Bluetooth is off.
- `serial0` symlink points to `ttyAMA0`.
- `hciuart` service should NOT be running (will fail to start, harmless).

Verify which case you're in **before** debugging anything else. Mixed expectations here cause hours of confusion.

### Quick driver sanity sweep

```bash
# What does the kernel think serial0 is?
ls -l /dev/serial0

# What baud did the driver settle on?
stty -F /dev/serial0

# Is anyone else using it?
sudo fuser -v /dev/serial0

# Pin function on GPIO14/15?
pinctrl get 14 15

# Boot-time UART messages?
dmesg | grep -iE 'uart|tty|serial' | head -30

# Bluetooth driver still grabbing it?
sudo systemctl status hciuart
```

If all of those check out → drivers are fine, problem is at the wire/converter/PIC.

---

## Files to Review

- `pic-firmware/MCAL/USART/USART.c` — UART driver (looks correct)
- `pic-firmware/APP/MyProject.c` — current test main (verify TRISC, ADCON1)
- `pic-firmware/config.h` — confirm `ADCON1_CONFIG = 0x06` and `_XTAL_FREQ = 8000000`
- `pi-script/handshake.py` — confirm PORT, BAUD correct

---

## Once UART is Verified

1. Re-enable production main (`#if 1` in MyProject.c).
2. Run `handshake.py` — should see ACK in <1 s.
3. PIC LCD should pass "Waiting for Pi" → "PI Ready" → "Homing..." → "Auto Mode OK".
4. Move on to sensor data parsing on Pi side.

---

## Debugging Without University Bench Supply

**Available outside lab:**
- Pi 4 (5 V rail on GPIO header — pin 2/4)
- Breadboard PSU module (likely MB102 — rated ~700 mA but sags under transient load)
- Full multimeter
- NodeMCU (ESP8266, 3.3 V logic)

**Not available:**
- 12 V supply for A4988 VMOT
- High-current bench rail (>1 A)
- USB-TTL adapter

### Power Strategy

| Subsystem | Current draw | Power source at home |
|---|---|---|
| PIC + LCD + sensors | ~80 mA | **Pi 5V rail (pin 2 or 4)** ✓ |
| Buzzer (active) | ~30 mA | Pi 5V ✓ |
| LEDs (RE1, RE2) | ~20 mA each | Pi 5V ✓ |
| Level converter | ~5 mA | Pi 5V ✓ |
| HC-SR04 ultrasonic | ~15 mA | Pi 5V ✓ |
| DHT11 | ~2 mA | Pi 5V ✓ |
| Soil sensor | ~5 mA | Pi 5V ✓ |
| Relay module coils | ~70 mA each (on) | **MB102 OK if one at a time**, marginal for both |
| Pump (via relay) | 500 mA – 2 A | **NO — needs bench PSU** |
| A4988 VMOT | 1.7 A peak | **NO — needs 12 V bench PSU** |
| Stepper motor 17HS4401 | 1.7 A | **NO — needs A4988 + 12 V** |

**Recommended home wiring:**

```
Pi 5V (pin 2) ──┬── PIC VDD (pins 11, 32)
                ├── LCD VDD
                ├── Level converter HV
                ├── DHT11, HC-SR04, soil sensor VCC
                └── (skip relay coils, A4988, pump)

Pi 3.3V (pin 1) ── Level converter LV

Pi GND (pin 6) ──┬── PIC VSS (pins 12, 31)
                 ├── All sensor GNDs
                 ├── Level converter GND
                 └── (CRITICAL: common GND everywhere)
```

Pi 5V rail can supply ~600 mA after Pi self-consumption — enough for PIC + LCD + sensors. NOT enough for relays + motor.

### What You CAN Debug at Home

| Task | Doable? | How |
|---|---|---|
| Stage A (PIC alive, oscillator) | ✓ | Pi 5V to PIC, LED blink test |
| Stage A.2 (PIC → Pi TX test) | ✓ | Same wiring, run `hexdump` on Pi |
| Stage B (Pi UART config, drivers) | ✓ | Pi alone, all software-side |
| Stage C (level converter) | ✓ | Multimeter on rails + idle voltages |
| Stage D (PIC register dump to LCD) | ✓ | PIC + LCD + Pi 5V |
| Stage D.3 (RC7 manual GPIO read) | ✓ | PIC + LCD + Pi TX through converter |
| Stage F (driver/kernel sweep) | ✓ | Pi software only |
| Full handshake end-to-end | ✓ | Once UART working, no other power needed |
| Production main with sensors | ✓ | Skip motor cycle; sensors + comms only |

### What You CANNOT Debug at Home

| Task | Why blocked | Workaround |
|---|---|---|
| Motor homing | 12 V + A4988 needs bench PSU | Stub `Motor_Home()` to return immediately. Comment out call in main. |
| Pump activation | Pump = 12 V + high current | Stub `Irrigation_RunCycle()` to just toggle relay pin briefly (no real load) |
| ACS712 current measurement | No real pump load | Read raw ADC, will read ~512 (zero current). Verify formula in firmware separately. |
| Stress test relay switching | Coil current adds up | Test one relay at a time |

### NodeMCU Tests

**SoftwareSerial** = library that bit-bangs UART on any GPIO pin in software. Used because NodeMCU's hardware UART is tied to USB-serial bridge (debug output) — can't share.

Power NodeMCU via micro-USB to laptop. That's also your debug window (Arduino Serial Monitor @ 115200).

Safe pins for UART use: **D5 (GPIO14), D6 (GPIO12), D7 (GPIO13)**. Avoid D3/D4/D8 (boot-strapping pins).

---

#### Test 1 — Pi → NodeMCU sniffer (verify Pi transmits)

**No level converter needed.** Both sides 3.3 V.

Wiring:
```
Pi GPIO14 (pin 8) ─── NodeMCU D5
Pi GND    (pin 6) ─── NodeMCU GND
```

Sketch:
```cpp
#include <SoftwareSerial.h>
SoftwareSerial sniff(D5, D6);

void setup() {
  Serial.begin(115200);
  sniff.begin(9600);
  Serial.println("Sniffer ready");
}
void loop() {
  while (sniff.available()) {
    uint8_t b = sniff.read();
    Serial.printf("RX: 0x%02X\n", b);
  }
}
```

Run `python3 handshake.py` on Pi. Read Serial Monitor.

| Output | Meaning |
|---|---|
| `RX: 0xBB / 0x10 / 0xAA` repeating every 200 ms | Pi TX works. Move to Test 2. |
| Nothing | Pi not transmitting. Debug Pi (cmdline.txt console, getty, wrong port). |
| Garbage bytes | Baud mismatch. Wrong UART (mini vs PL011), missing `enable_uart=1`. |
| Only some bytes show | Pi flaky — getty stealing chars, driver buffering. |

---

#### Test 2 — NodeMCU → PIC master (verify PIC + converter)

**Level converter needed.** NodeMCU 3.3 V can't drive PIC RC7 directly (VIH = 4 V).

Wiring:
```
NodeMCU D6  ─── Converter A1/TXI (LV side)
NodeMCU 3V3 ─── Converter LV
NodeMCU GND ─── Converter GND ─── PIC GND
Pi 5V       ─── Converter HV
Pi 5V       ─── PIC VDD
Converter B1/TXO (HV side) ─── PIC RC7
```

Sketch:
```cpp
#include <SoftwareSerial.h>
SoftwareSerial picPort(D5, D6);

void setup() {
  Serial.begin(115200);
  picPort.begin(9600);
}
void loop() {
  picPort.write(0xBB);
  picPort.write(0x10);
  picPort.write(0xAA);
  Serial.println("TX: BB 10 AA");
  delay(200);
}
```

PIC running UART test main (LCD shows `Last:0xXX / Count:N`).

| LCD shows | Meaning |
|---|---|
| Count increments, Last cycles BB/10/AA | PIC + converter work. Pi was the problem. |
| Count stays 0 | Converter dead or PIC RX broken. Multimeter on RC7 — should sit at 5 V idle. If 0 V or 3.3 V → converter. If 5 V but no toggle → PIC. |
| Count increments, garbage values | Baud mismatch. Check PIC oscillator (Stage A.1). |

### Breadboard PSU (MB102) Caveats

MB102 module specs: ~700 mA combined across 3.3 V + 5 V rails. Real-world: **droops badly under transient loads**. Symptoms:
- Voltage sags from 5.0 V to 4.2 V when relay clicks → PIC brown-out reset (you saw this earlier)
- Capacitor discharge through onboard linear regulator can't keep up
- LED indicator stays on but rail is collapsing

**Mitigations if you must use MB102:**
- Add 1000 µF electrolytic across the 5 V output, close to PIC
- Add 100 nF ceramic at PIC VDD/VSS
- Disable BOREN config bit (BOREN=OFF) — masks symptom, not a real fix
- Don't try to power relays + motor from it

**Better:** for UART debugging, **use Pi 5V rail**. Pi has a proper switching regulator that holds 5.0 V under 600 mA load. No sag.

### Pi 4 GPIO Pinout (40-pin header)

View: looking at top of Pi 4 board, USB ports on right, GPIO header along right edge. Pin 1 = top-left of header (closest to SD card slot).

```
        ┌──────┬──────┐
3V3 PWR │  1   2  │ 5V PWR
GPIO2   │  3   4  │ 5V PWR
GPIO3   │  5   6  │ GND
GPIO4   │  7   8  │ GPIO14 (TXD0)  ← Pi → PIC via converter
GND     │  9  10  │ GPIO15 (RXD0)  ← Pi ← PIC via converter
GPIO17  │ 11  12  │ GPIO18
GPIO27  │ 13  14  │ GND
GPIO22  │ 15  16  │ GPIO23
3V3 PWR │ 17  18  │ GPIO24
GPIO10  │ 19  20  │ GND
GPIO9   │ 21  22  │ GPIO25
GPIO11  │ 23  24  │ GPIO8
GND     │ 25  26  │ GPIO7
GPIO0   │ 27  28  │ GPIO1
GPIO5   │ 29  30  │ GND
GPIO6   │ 31  32  │ GPIO12
GPIO13  │ 33  34  │ GND
GPIO19  │ 35  36  │ GPIO16
GPIO26  │ 37  38  │ GPIO20
GND     │ 39  40  │ GPIO21
        └──────┴──────┘
```

**Pins you need for this project:**

| Pi pin # | GPIO | Function | Connects to |
|---|---|---|---|
| 1 | — | 3V3 PWR | Level converter LV |
| 2 | — | 5V PWR | PIC VDD, LCD, sensors |
| 4 | — | 5V PWR | (alt source) |
| 6 | — | GND | Common ground rail |
| 8 | GPIO14 | TXD0 (UART TX) | Converter A1 / TXI |
| 9 | — | GND | (alt ground) |
| 10 | GPIO15 | RXD0 (UART RX) | Converter A2 / RXO |

Other GND pins: 14, 20, 25, 30, 34, 39. Any of them works. More ground connections = better.

### NodeMCU (ESP8266) Pinout

NodeMCU board labels (D0-D8) **do not match** ESP8266 GPIO numbers. Always use the GPIO number in code, the D label on the board.

```
                 ┌──────────────────┐
              A0 │ A0           D0  │ GPIO16
              G  │ GND          D1  │ GPIO5      ← I2C SCL (safe)
              VU │ VIN(5V)      D2  │ GPIO4      ← I2C SDA (safe)
                 │              D3  │ GPIO0  ⚠   ← boot pin, avoid
                 │              D4  │ GPIO2  ⚠   ← boot pin + LED
                 │              3V3 │ 3.3V out
                 │              GND │
                 │              D5  │ GPIO14     ← SAFE — use for UART
                 │              D6  │ GPIO12     ← SAFE — use for UART
                 │              D7  │ GPIO13     ← SAFE — use for UART
                 │              D8  │ GPIO15 ⚠   ← boot pin, avoid
                 │              RX  │ GPIO3      ← UART0 RX (USB shared)
                 │              TX  │ GPIO1      ← UART0 TX (USB shared)
                 │              GND │
                 │              3V3 │
                 └──────────────────┘
```

**Pins for sniffer / master substitute:**

| NodeMCU label | GPIO | Use for |
|---|---|---|
| D5 | GPIO14 | SoftwareSerial RX (sniff Pi TX) |
| D6 | GPIO12 | SoftwareSerial TX (send to PIC RX) |
| D7 | GPIO13 | spare (alternative pin) |
| GND | — | Common with Pi + PIC |
| VIN | — | 5 V input from USB or external |
| 3V3 | — | 3.3 V output (don't back-feed) |

**Power NodeMCU via its micro-USB cable** — simplest, gives you Serial Monitor at the same time.

### Should You Run the Script on the Pi?

**Yes — that's the design.** `handshake.py` runs on Pi. Pi is the master. PIC is the slave responder.

Workflow:

1. **Wire everything per project plan** (Pi ↔ converter ↔ PIC).
2. **Power PIC** (from Pi 5V or other supply).
3. **Power Pi**, boot to login.
4. **On Pi**, in terminal:
   ```bash
   cd ~/pi-script
   python3 handshake.py
   ```
5. **Watch PIC LCD** simultaneously. RX counter (test main) or "PI Ready!" message (production main).

Pi will retry every 200 ms for 30 seconds then give up. If PIC receives and responds, script prints `[OK] Handshake complete`.

**Debug order:**

1. Run `handshake.py` on Pi → baseline failure.
2. Test 1 (NodeMCU sniffer) → confirms Pi actually transmits.
3. Test 2 (NodeMCU master) → isolates converter/PIC if Pi transmits fine.

### Questions to Confirm Before Starting

- Can you spare a Pi GPIO pair (5 V + GND) to power PIC? **(Disconnect MB102 entirely — running both supplies in parallel creates ground loops.)**
- Is the level converter currently powered from Pi or MB102? Move it to Pi 3.3 V/5 V rails for consistency.
- Do you have a USB-C charger ≥ 2 A for Pi? Pi will under-volt with weak chargers and the GPIO 5 V rail droops.
