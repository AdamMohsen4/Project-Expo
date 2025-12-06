#include "board.h"
#include "world.h"
#include "input.h"
#include "game.h"
#include <stdio.h>

int main(void){
    printf("\033[2J\033[H");  /* Clear screen */

    board_init();
    input_init();
    world_init();
    game_init();
    game_loop();

    printf("\n\nThank you for playing!\n");
    return 0;
}
