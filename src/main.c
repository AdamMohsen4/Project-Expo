#include "board.h"
#include "world.h"
#include "input.h"
#include "game.h"

/* boot.S expects these symbols to exist */
void handle_interrupt(unsigned cause) { (void)cause; }
void labinit(void) {}

int main(void){
    board_init();
    input_init();
    world_init();
    game_init();
    game_loop();
    while(1) {}
    return 0;
}
