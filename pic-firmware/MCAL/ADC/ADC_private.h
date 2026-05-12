/*
 * ADC_private.h
 * PIC16F877A ADC Internal Register Bit Definitions
 */

#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

/* ── Channel selection (CHS2:CHS0 in ADCON0) ── */
#define ADC_CH_AN0   0   /* RA0 — Soil Moisture   */
#define ADC_CH_AN1   1   /* RA1 — LM35 Temp       */
#define ADC_CH_AN2   2   /* RA2 — ACS712 Current  */

/* ── Clock source (ADCS1:ADCS0) — Fosc/32 @ 20MHz → Tad = 1.6µs (min 1.2µs) ── */
#define ADC_CLK_FOSC_32   0b10   /* ADCS1=1, ADCS0=0 */

/* ── ADCON1 config: all AN0..AN4 analog, VREF=VDD/VSS, right-justified ── */
/*    ADFM=1 (right), PCFG3:0 = 0b0000 → AN0..AN7 analog                 */
#define ADC_ADCON1_CONFIG   0b10000100   /* ADFM=1 right-just; PCFG=0100: AN0-AN2 analog, RE1/RE2 digital */

/* ── Acquisition delay: Tad * 12 ≈ 20µs minimum required ── */
#define ADC_ACQ_DELAY_US    25

/* ── Safe ADCON0 bit positions (explicit, never rely on xc.h names) ──
 *   xc.h may define ADON/GO_DONE as bitmasks (0x01/0x04) not positions.
 *   BIT_MATH macros need positions (0..7). Use these names instead.  */
#define ADC_BIT_ADON    0   /* ADCON0 bit 0: ADC module on */
#define ADC_BIT_GO      2   /* ADCON0 bit 2: start / not-done */

#endif /* ADC_PRIVATE_H */
