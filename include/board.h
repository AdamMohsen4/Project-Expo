#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void board_init(void);

uint16_t board_get_switches(void); /* 10 LSBs */
uint8_t  board_get_button(void);   /* BTN2 bit0 */

void board_set_leds(uint16_t mask);
void board_set_hex_digit(int idx, int digit);      /* idx 0..5, digit 0..9 or -1 blank */
void board_set_hex_2dec(int start_idx, uint8_t v); /* two decimal digits */

void board_timer_init_100ms(void);
int  board_timer_poll_timeout(void); /* 1 if timeout (and clears), else 0 */

#endif
