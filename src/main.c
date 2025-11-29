/*
 * Switchback - main loop (skeleton)
 * Authors: Adam Mohsen & Hjalmar Albinsson
 * Date: 2025-10-11
 *
 * Very small starter main to compile and run on a host system. Board-specific input
 * will be added under `board/` and `input.c` will be adapted.
 */

#include <stdio.h>
#include "game.h"

int main(void) {
    printf("Switchback — host playable demo\n");
    game_init();
    game_loop();
    return 0;
}
