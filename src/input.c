#include "input.h"
#include "board.h"

static unsigned armed = 1;

void input_init(void) { armed = 1; }

int input_peek_choice(void) {
    return (int)(board_get_switches() & 0x1Fu); /* SW0..SW4 */
}

int input_get_action(void) {
    unsigned b = board_get_button();
    if (!b) { armed = 1; return -1; }
    if (b && armed) {
        armed = 0;
        int c = input_peek_choice();
        return (c > 0) ? c : -1;
    }
    return -1;
}
