#ifndef UI_H
#define UI_H

#include <stdint.h>

void ui_init(void);
void ui_println(const char *s);
void ui_print(const char *s);
void ui_print_dec(unsigned x);
void ui_wait_btn2(void);

void ui_hud(uint8_t room_id, uint8_t inv_count, uint8_t choice, uint16_t led_mask);

#endif
