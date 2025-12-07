#include "input.h"
#include "board.h"
// Author: Adam Mohsen
/* Edge detection state - prevents repeated reads from held button */
static unsigned armed = 1;

/* Initialize input system, reset armed state */
void input_init(void) { armed = 1; }

/* Read current choice from switches without consuming button press */
int input_peek_choice(void) {
    return (int)(board_get_switches() & 0x1Fu); /* SW0..SW4 */
}

/* Get user action on button press edge */
int input_get_action(void) {
    unsigned b = board_get_button();
    if (!b) { armed = 1; return -1; }  /* Button released - arm for next press */
    if (b && armed) {  /* Button pressed while armed - trigger action */
        armed = 0;
        int c = input_peek_choice();
        return (c > 0) ? c : -1;  /* Return choice if valid, else -1 */
    }
    return -1;  /* Button held - no action */
}
