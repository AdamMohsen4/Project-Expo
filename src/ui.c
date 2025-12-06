#include "ui.h"
#include "board.h"

/* From dtekv-lib.c (note: signatures are non-const) */
extern void print(char *);
extern void printc(char);
extern void print_dec(unsigned int);

static void print_const(const char *s) { print((char*)s); }

void ui_init(void) {}

void ui_print(const char *s) { print_const(s); }

void ui_println(const char *s) {
    print_const(s);
    printc('\n');
}

void ui_print_dec(unsigned x) {
    print_dec(x);
}

void ui_wait_btn2(void) {
    ui_println("[BTN2]");
    while (!board_get_button()) {}
    while (board_get_button()) {}
}

void ui_hud(uint8_t room_id, uint8_t inv_count, uint8_t choice, uint16_t led_mask) {
    board_set_hex_2dec(0, choice);
    board_set_hex_2dec(2, inv_count);
    board_set_hex_2dec(4, room_id);
    board_set_leds(led_mask);
}
