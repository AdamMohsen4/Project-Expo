#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include <stdbool.h>

void world_init(void);

uint8_t     world_room_id(void);
const char *world_room_name(void);
const char *world_room_desc(void);

int         world_exit_count(void);
const char *world_exit_name(int exit_index);
bool        world_try_move(int exit_index);

bool        world_take_item(void);
void        world_use_action(void);

uint8_t     world_inventory_count(void);
uint16_t    world_led_mask(void);

bool        world_check_win(void);

bool        world_has_extinguisher(void);

void        world_debug_give_extinguisher(void); //DEBUG

#endif
