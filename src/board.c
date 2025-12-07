// Author: Hjalmar Albinsson 
#include "board.h"

/* Memory-mapped I/O addresses for board peripherals */
#define LED_ADDR   0x04000000u  /* LED array base address */
#define SW_ADDR    0x04000010u  /* Switch array base address */
#define TIMER_BASE 0x04000020u  /* Timer peripheral base address */
#define HEX_BASE   0x04000050u  /* Hex display base address */
#define BTN2_ADDR  0x040000d0u  /* Button 2 address */

/* Timer register accessors */
#define T_STATUS  (*(volatile unsigned *)(TIMER_BASE + 0x00))
#define T_CONTROL (*(volatile unsigned *)(TIMER_BASE + 0x04))
#define T_PERIODL (*(volatile unsigned *)(TIMER_BASE + 0x08))
#define T_PERIODH (*(volatile unsigned *)(TIMER_BASE + 0x0C))

/* Timer control bits */
#define T_ITO   (1u<<0)  /* Interrupt timeout enable */
#define T_CONT  (1u<<1)  /* Continuous mode */
#define T_START (1u<<2)  /* Start timer */
#define T_TO    (1u<<0)  /* Timeout flag */

/* Timer clock configuration */
#define TIMER_CLK_HZ 30000000u
#define TICKS_100MS  (TIMER_CLK_HZ/10u)

/* Get pointer to hex display at given index */
static inline volatile unsigned *hex_ptr(int idx) {
    return (volatile unsigned *)(HEX_BASE + (unsigned)idx * 0x10u);
}

/* Seven-segment display lookup table - active-low encoding */
static const unsigned char seg_lut[10] = { 64,121,36,48,25,18,2,120,0,16 };
static const unsigned char seg_blank = 0x7F;

/* Set LED pattern from 10-bit mask */
void board_set_leds(uint16_t mask) {
    *(volatile unsigned *)LED_ADDR = (unsigned)(mask & 0x03FFu);
}

/* Read switch values - returns 10 LSBs */
uint16_t board_get_switches(void) {
    return (uint16_t)(*(volatile unsigned *)SW_ADDR & 0x03FFu);
}

/* Read BTN2 state */
uint8_t board_get_button(void) {
    return (uint8_t)(*(volatile unsigned *)BTN2_ADDR & 0x1u);
}

/* Set single hex display digit (0-9 or -1 for blank) */
void board_set_hex_digit(int idx, int digit) {
    if (idx < 0 || idx > 5) return;
    unsigned v = (digit >= 0 && digit <= 9) ? seg_lut[digit] : seg_blank;
    *hex_ptr(idx) = v;
}

/* Display two decimal digits starting at index */
void board_set_hex_2dec(int start_idx, uint8_t v) {
    int ones = (int)(v % 10u);
    int tens = (int)((v / 10u) % 10u);
    board_set_hex_digit(start_idx, ones);      /* Lower digit first */
    board_set_hex_digit(start_idx + 1, tens);  /* Higher digit second */
}

/* Initialize hardware timer for 100ms periodic interrupts */
void board_timer_init_100ms(void) {
    unsigned period = TICKS_100MS - 1u;
    T_PERIODL = (period & 0xFFFFu);  /* Set lower 16 bits of period */
    T_PERIODH = (period >> 16);      /* Set upper 16 bits of period */
    T_STATUS  = T_TO;                /* Clear pending timeout flag */
    T_CONTROL = T_CONT | T_ITO | T_START;  /* Start timer in continuous mode */
}

/* Poll timer timeout status and clear if set */
int board_timer_poll_timeout(void) {
    if (T_STATUS & T_TO) {
        T_STATUS = T_TO;  /* Write-1-to-clear timeout flag */
        return 1;
    }
    return 0;
}

/* Initialize all board hardware - LEDs, hex displays, and timer */
void board_init(void) {
    board_set_leds(0);
    for (int i = 0; i < 6; ++i) board_set_hex_digit(i, -1);  /* Blank all displays */
    board_timer_init_100ms();
}
