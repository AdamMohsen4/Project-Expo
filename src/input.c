/* input.c — board input abstraction (skeleton)
 * TODO: implement reading of buttons/switches for DTEK-V board.
 */

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_init(void) {
    /* prepare input (placeholder) */
}

int input_get_action(void) {
    /* Host-mode input: prompt the user for a numeric choice. Returns the
     * integer the player entered, or -1 on invalid input. This is blocking
     * and meant for keyboard-driven testing on a host machine. For board
     * integration replace this implementation with button reading.
     */
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    /* strip newline */
    buf[strcspn(buf, "\r\n")] = '\0';
    if (buf[0] == '\0') return -1;
    char *endptr = NULL;
    long v = strtol(buf, &endptr, 10);
    if (endptr == buf) return -1;
    return (int)v;
}
