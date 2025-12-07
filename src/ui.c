#include "ui.h"
#include "board.h"

/* From dtekv-lib.c (signatures are non-const) */
extern void print(char *);
extern void printc(char);
extern void print_dec(unsigned int);

/* Helper to print const strings, handles PERF_SILENT mode */
static void print_const(const char *s) {
#ifndef PERF_SILENT
  print((char*)s);
#else
  (void)s;
#endif
}

/* Initialize UI (currently no-op) */
void ui_init(void) {}

/* Print string without newline */
void ui_print(const char *s) {
#ifndef PERF_SILENT
  print_const(s);
#else
  (void)s;
#endif
}

/* Print string followed by newline */
void ui_println(const char *s) {
#ifndef PERF_SILENT
  print_const(s);
  printc('\n');
#else
  (void)s;
#endif
}

/* Print unsigned integer in decimal */
void ui_print_dec(unsigned x) {
    print_dec(x);
}

/* Wait for BTN2 press and release cycle */
void ui_wait_btn2(void) {
    ui_println("[BTN2]");
    while (!board_get_button()) {}  /* Wait for press */
    while (board_get_button()) {}   /* Wait for release */
}

/* Update HUD display - shows choice, inventory count, room ID on hex displays and LEDs */
void ui_hud(uint8_t room_id, uint8_t inv_count, uint8_t choice, uint16_t led_mask) {
    board_set_hex_2dec(0, choice);     /* HEX0-1: current choice */
    board_set_hex_2dec(2, inv_count);  /* HEX2-3: inventory count */
    board_set_hex_2dec(4, room_id);    /* HEX4-5: room ID */
    board_set_leds(led_mask);          /* LEDs: inventory state */
}
