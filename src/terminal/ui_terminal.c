#include "ui.h"
#include <stdio.h>

void ui_init(void) {
    /* Nothing needed for terminal */
}

void ui_print(const char *s) {
    printf("%s", s);
    fflush(stdout);
}

void ui_println(const char *s) {
    printf("%s\n", s);
    fflush(stdout);
}

void ui_print_dec(unsigned x) {
    printf("%u", x);
    fflush(stdout);
}

void ui_wait_btn2(void) {
    printf("\n[Press ENTER to continue]\n");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ui_hud(uint8_t room_id, uint8_t inv_count, uint8_t choice, uint16_t led_mask) {
    /* Display HUD info on terminal */
    printf("\r[Room:%u Inv:%u Choice:%u LEDs:0x%03X]",
           room_id, inv_count, choice, led_mask);
    fflush(stdout);
}
