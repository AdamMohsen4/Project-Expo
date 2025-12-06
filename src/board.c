#include "board.h"

/* MMIO from your labs */
#define LED_ADDR   0x04000000u
#define SW_ADDR    0x04000010u
#define TIMER_BASE 0x04000020u
#define HEX_BASE   0x04000050u
#define BTN2_ADDR  0x040000d0u

#define T_STATUS  (*(volatile unsigned *)(TIMER_BASE + 0x00))
#define T_CONTROL (*(volatile unsigned *)(TIMER_BASE + 0x04))
#define T_PERIODL (*(volatile unsigned *)(TIMER_BASE + 0x08))
#define T_PERIODH (*(volatile unsigned *)(TIMER_BASE + 0x0C))

#define T_ITO   (1u<<0)
#define T_CONT  (1u<<1)
#define T_START (1u<<2)
#define T_TO    (1u<<0)

#define TIMER_CLK_HZ 30000000u
#define TICKS_100MS  (TIMER_CLK_HZ/10u)

static inline volatile unsigned *hex_ptr(int idx) {
    return (volatile unsigned *)(HEX_BASE + (unsigned)idx * 0x10u);
}

/* Active-low LUT (same as your lab) */
static const unsigned char seg_lut[10] = { 64,121,36,48,25,18,2,120,0,16 };
static const unsigned char seg_blank = 0x7F;

void board_set_leds(uint16_t mask) {
    *(volatile unsigned *)LED_ADDR = (unsigned)(mask & 0x03FFu);
}

uint16_t board_get_switches(void) {
    return (uint16_t)(*(volatile unsigned *)SW_ADDR & 0x03FFu);
}

uint8_t board_get_button(void) {
    return (uint8_t)(*(volatile unsigned *)BTN2_ADDR & 0x1u);
}

void board_set_hex_digit(int idx, int digit) {
    if (idx < 0 || idx > 5) return;
    unsigned v = (digit >= 0 && digit <= 9) ? seg_lut[digit] : seg_blank;
    *hex_ptr(idx) = v;
}

void board_set_hex_2dec(int start_idx, uint8_t v) {
    int ones = (int)(v % 10u);
    int tens = (int)((v / 10u) % 10u);
    board_set_hex_digit(start_idx, ones);
    board_set_hex_digit(start_idx + 1, tens);
}

void board_timer_init_100ms(void) {
    unsigned period = TICKS_100MS - 1u;
    T_PERIODL = (period & 0xFFFFu);
    T_PERIODH = (period >> 16);
    T_STATUS  = T_TO; /* clear pending */
    T_CONTROL = T_CONT | T_ITO | T_START;
}

int board_timer_poll_timeout(void) {
    if (T_STATUS & T_TO) {
        T_STATUS = T_TO; /* write-1-to-clear */
        return 1;
    }
    return 0;
}

void board_init(void) {
    board_set_leds(0);
    for (int i = 0; i < 6; ++i) board_set_hex_digit(i, -1);
    board_timer_init_100ms();
}
