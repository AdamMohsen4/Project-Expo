#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* Initialize board hardware - LEDs, hex displays, and timer */
void board_init(void);

/* Read switch values - returns 10 LSBs */
uint16_t board_get_switches(void);

/* Read BTN2 state - returns bit0 */
uint8_t  board_get_button(void);

/* Set LED pattern from mask */
void board_set_leds(uint16_t mask);

/* Set single hex digit (idx 0..5, digit 0..9 or -1 for blank) */
void board_set_hex_digit(int idx, int digit);

/* Set two decimal digits starting at index */
void board_set_hex_2dec(int start_idx, uint8_t v);

/* Initialize hardware timer for 100ms intervals */
void board_timer_init_100ms(void);

/* Check and clear timer timeout - returns 1 if timeout occurred, else 0 */
int  board_timer_poll_timeout(void);

#endif
