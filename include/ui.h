#ifndef UI_H
#define UI_H

#include <stdint.h>

/* Initialize user interface */
void ui_init(void);

/* Print string followed by newline */
void ui_println(const char *s);

/* Print string without newline */
void ui_print(const char *s);

/* Print unsigned integer in decimal format */
void ui_print_dec(unsigned x);

/* Wait for BTN2 press and release */
void ui_wait_btn2(void);

/* Update HUD display on hex digits and LEDs */
void ui_hud(uint8_t room_id, uint8_t inv_count, uint8_t choice, uint16_t led_mask);

#endif
