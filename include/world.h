#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include <stdbool.h>

/* Initialize world state - rooms, inventory, and game flags */
void world_init(void);

/* Get current room ID */
uint8_t     world_room_id(void);

/* Get current room name */
const char *world_room_name(void);

/* Get current room description (considers darkness and item state) */
const char *world_room_desc(void);

/* Get number of available exits from current room */
int         world_exit_count(void);

/* Get name of exit at given index */
const char *world_exit_name(int exit_index);

/* Attempt to move through exit - handles locks and darkness */
bool        world_try_move(int exit_index);

/* Take item in current room if present */
bool        world_take_item(void);

/* Use items in inventory (e.g., light lamp with matches) */
void        world_use_action(void);

/* Get current inventory item count */
uint8_t     world_inventory_count(void);

/* Get LED mask representing inventory state */
uint16_t    world_led_mask(void);

/* Check if win condition is met */
bool        world_check_win(void);

/* Check if player has fire extinguisher */
bool        world_has_extinguisher(void);

#endif
