#include "board.h"
#include "world.h"
#include "input.h"
#include "game.h"
#include "perf.h"
// Authors: Adam Mohsen
/* boot.S expects these symbols to exist */
void handle_interrupt(unsigned cause) { (void)cause; }
void labinit(void) {}

/* Main entry point - initializes systems and runs game or benchmarks */
int main(void){
    board_init();

    /* If SW9 is ON at boot, run benchmarks instead of the interactive game */
    // if (board_get_switches() & (1u << 9)) {
    //     perf_run_benchmarks();
    // }

    /* Initialize game systems */
    input_init();
    world_init();
    game_init();
    game_loop();
    while(1) {}
    return 0;
}
