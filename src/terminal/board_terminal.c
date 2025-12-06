#include "board.h"
#include <stdio.h>
#include <time.h>

/* Terminal version - simulates board hardware with keyboard input */

static uint16_t switches = 0;
static uint8_t button = 0;
static uint16_t leds = 0;
static int hex_digits[6] = {-1, -1, -1, -1, -1, -1};
static clock_t last_timer = 0;
static int timer_initialized = 0;

void board_init(void) {
    board_set_leds(0);
    for (int i = 0; i < 6; ++i) board_set_hex_digit(i, -1);
    timer_initialized = 0;
}

uint16_t board_get_switches(void) {
    return switches;
}

uint8_t board_get_button(void) {
    return button;
}

void board_set_leds(uint16_t mask) {
    leds = mask & 0x03FFu;
}

void board_set_hex_digit(int idx, int digit) {
    if (idx >= 0 && idx <= 5) {
        hex_digits[idx] = digit;
    }
}

void board_set_hex_2dec(int start_idx, uint8_t v) {
    int ones = (int)(v % 10u);
    int tens = (int)((v / 10u) % 10u);
    board_set_hex_digit(start_idx, ones);
    board_set_hex_digit(start_idx + 1, tens);
}

void board_timer_init_100ms(void) {
    last_timer = clock();
    timer_initialized = 1;
}

int board_timer_poll_timeout(void) {
    if (!timer_initialized) return 0;

    clock_t now = clock();
    double elapsed = ((double)(now - last_timer)) / CLOCKS_PER_SEC;

    if (elapsed >= 0.1) {  /* 100ms */
        last_timer = now;
        return 1;
    }
    return 0;
}

/* Terminal helper functions */
void board_terminal_set_choice(int choice) {
    switches = (uint16_t)(choice & 0x1F);
}

void board_terminal_press_button(void) {
    button = 1;
}

void board_terminal_release_button(void) {
    button = 0;
}
