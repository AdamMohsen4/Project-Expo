#ifndef GAME_H
#define GAME_H

/* Core game public API (minimal) */

#include <stdbool.h>

/* Initialize the game world */
void game_init(void);

/* Run one tick of the main loop */
void game_tick(void);

/* Query if player has won */
bool game_won(void);

#endif /* GAME_H */
