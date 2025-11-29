#ifndef WORLD_H
#define WORLD_H

/* Minimal world header — expand with structs and enums */

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	ITEM_NONE = 0,
	ITEM_KEY,
	ITEM_LAMP,
	ITEM_BATTERY,
	ITEM_ART1,
	ITEM_ART2,
	ITEM_ART3,
} item_t;

/* Directions: 0=N,1=E,2=S,3=W */
#define DIR_NORTH 0
#define DIR_EAST 1
#define DIR_SOUTH 2
#define DIR_WEST 3

void world_init(void);

/* Room info */
const char *world_get_room_name(void);
const char *world_get_room_description(void);

/* Exits: the API exposes a count and name for each available exit from
 * the current room. Exits are numbered 0..count-1 and map to internal
 * direction indices for movement via world_try_move().
 */
int world_get_exit_count(void);
const char *world_get_exit_name(int exit_index);

/* Attempt to move through an exit index (0..count-1). Returns true on
 * successful move, false if move was blocked (locked door, dark room, no exit).
 */
bool world_try_move(int exit_index);

/* Room items */
item_t world_get_room_item(void);
bool world_take_room_item(void);

/* Inventory */
void world_print_inventory(void);
bool world_has_item(item_t item);
bool world_use_item(item_t item);
void world_use_menu(void);

/* Win check */
bool world_check_win(void);

void world_init(void);
void debug_print_world(void);

#endif /* WORLD_H */
