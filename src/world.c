/* world.c — world data and puzzle flags (skeleton)
 * TODO: fill with room_t, item_t definitions and helper functions
 */

#include "world.h"
#include <stdio.h>
#include <string.h>

struct room {
    const char *name;
    const char *desc;
    int exits[4]; /* direction -> room index or -1 */
    item_t item;
    bool dark;
    bool locked; /* locked when approaching this room from certain exit */
};

static struct room rooms[8];
static int current_room = 0;
static item_t inventory[10];
static int inventory_count = 0;
static bool lamp_lit = false;

static void add_item_to_inventory(item_t it) {
    if (it == ITEM_NONE) return;
    for (int i = 0; i < inventory_count; ++i) if (inventory[i] == it) return; /* no duplicates */
    if (inventory_count < (int)(sizeof(inventory)/sizeof(inventory[0]))) {
        inventory[inventory_count++] = it;
    }
}

void world_init(void) {
    /* Room 0: Entrance */
    rooms[0].name = "Entrance";
    rooms[0].desc = "You stand at the entrance of an old facility. There's a faint breeze.";
    rooms[0].exits[DIR_NORTH] = -1;
    rooms[0].exits[DIR_EAST] = 1;
    rooms[0].exits[DIR_SOUTH] = 7;
    rooms[0].exits[DIR_WEST] = -1;
    rooms[0].item = ITEM_LAMP;
    rooms[0].dark = false;
    rooms[0].locked = false;

    /* Room 1: Hallway */
    rooms[1].name = "Hallway";
    rooms[1].desc = "A narrow hallway with doors on the side and a suspicious locked door to the north.";
    rooms[1].exits[DIR_WEST] = 0;
    rooms[1].exits[DIR_EAST] = 3;
    rooms[1].exits[DIR_NORTH] = 2; /* locked door */
    rooms[1].exits[DIR_SOUTH] = -1;
    rooms[1].item = ITEM_NONE;
    rooms[1].dark = false;
    rooms[1].locked = false;

    /* Room 2: Locked Chamber (north of hallway) */
    rooms[2].name = "Locked Chamber";
    rooms[2].desc = "A heavy door stands here, seal marks around it.";
    rooms[2].exits[DIR_SOUTH] = 1;
    rooms[2].exits[DIR_NORTH] = 6; /* gate room */
    rooms[2].exits[DIR_EAST] = -1;
    rooms[2].exits[DIR_WEST] = -1;
    rooms[2].item = ITEM_ART1;
    rooms[2].dark = false;
    rooms[2].locked = true; /* locked from hallway -> 2 */

    /* Room 3: Key room */
    rooms[3].name = "Supply Closet";
    rooms[3].desc = "A small closet with scattered equipment.";
    rooms[3].exits[DIR_WEST] = 1;
    rooms[3].exits[DIR_EAST] = 5;
    rooms[3].exits[DIR_NORTH] = 4;
    rooms[3].exits[DIR_SOUTH] = -1;
    rooms[3].item = ITEM_KEY;
    rooms[3].dark = false;
    rooms[3].locked = false;

    /* Room 4: Dark Room (north of supply closet) */
    rooms[4].name = "Dark Chamber";
    rooms[4].desc = "It's pitch dark here. You can barely sense a cold, damp floor.";
    rooms[4].exits[DIR_SOUTH] = 3;
    rooms[4].exits[DIR_NORTH] = -1;
    rooms[4].exits[DIR_EAST] = -1;
    rooms[4].exits[DIR_WEST] = -1;
    rooms[4].item = ITEM_ART2;
    rooms[4].dark = true;
    rooms[4].locked = false;

    /* Room 5: Battery room */
    rooms[5].name = "Maintenance";
    rooms[5].desc = "A maintenance alcove with a battery pack on a shelf.";
    rooms[5].exits[DIR_WEST] = 3;
    rooms[5].exits[DIR_EAST] = -1;
    rooms[5].exits[DIR_NORTH] = -1;
    rooms[5].exits[DIR_SOUTH] = -1;
    rooms[5].item = ITEM_BATTERY;
    rooms[5].dark = false;
    rooms[5].locked = false;

    /* Room 6: Gate Room (north of locked chamber) */
    rooms[6].name = "Sealed Gate";
    rooms[6].desc = "A massive sealed gate blocks the way out. There is a mechanism that looks like it needs a master key.";
    rooms[6].exits[DIR_SOUTH] = 2;
    rooms[6].exits[DIR_NORTH] = -1;
    rooms[6].exits[DIR_EAST] = -1;
    rooms[6].exits[DIR_WEST] = -1;
    rooms[6].item = ITEM_NONE;
    rooms[6].dark = false;
    rooms[6].locked = false;

    /* Room 7: Artifact 3 room (south of entrance) */
    rooms[7].name = "Hidden Alcove";
    rooms[7].desc = "A shallow alcove with ancient markings; something glints inside.";
    rooms[7].exits[DIR_NORTH] = 0;
    rooms[7].exits[DIR_SOUTH] = -1;
    rooms[7].exits[DIR_EAST] = -1;
    rooms[7].exits[DIR_WEST] = -1;
    rooms[7].item = ITEM_ART3;
    rooms[7].dark = false;
    rooms[7].locked = false;

    current_room = 0;
    inventory_count = 0;
    lamp_lit = false;
}

const char *world_get_room_name(void) {
    return rooms[current_room].name;
}

const char *world_get_room_description(void) {
    if (rooms[current_room].dark && !lamp_lit) return "It's too dark to see anything.";
    return rooms[current_room].desc;
}

int world_get_exit_count(void) {
    int names = 0;
    for (int d = 0; d < 4; ++d) if (rooms[current_room].exits[d] != -1) ++names;
    return names;
}

const char *world_get_exit_name(int exit_index) {
    int found = 0;
    for (int d = 0; d < 4; ++d) {
        if (rooms[current_room].exits[d] != -1) {
            if (found == exit_index) {
                switch (d) {
                    case DIR_NORTH: return "north";
                    case DIR_EAST: return "east";
                    case DIR_SOUTH: return "south";
                    case DIR_WEST: return "west";
                }
            }
            ++found;
        }
    }
    return "";
}

static int exit_index_to_dir(int exit_index) {
    int found = 0;
    for (int d = 0; d < 4; ++d) {
        if (rooms[current_room].exits[d] != -1) {
            if (found == exit_index) return d;
            ++found;
        }
    }
    return -1;
}

bool world_try_move(int exit_index) {
    int dir = exit_index_to_dir(exit_index);
    if (dir < 0) return false;
    int next = rooms[current_room].exits[dir];
    if (next < 0) return false;
    /* Check locked approach: if next is room 2 and it's locked, need key */
    if (next == 2 && rooms[2].locked) {
        /* check key in inventory */
        for (int i = 0; i < inventory_count; ++i) if (inventory[i] == ITEM_KEY) {
            rooms[2].locked = false;
            printf("You use the key to unlock the heavy door.\n");
            return (current_room = next), true;
        }
        printf("The door is locked. You need a key.\n");
        return false;
    }
    /* Check dark room: if entering room 4 and lamp not lit, block */
    if (next == 4 && rooms[4].dark && !lamp_lit) {
        printf("It's pitch dark ahead — you need a lit lamp to proceed.\n");
        return false;
    }
    current_room = next;
    return true;
}

item_t world_get_room_item(void) {
    return rooms[current_room].item;
}

bool world_take_room_item(void) {
    item_t it = rooms[current_room].item;
    if (it == ITEM_NONE) return false;
    add_item_to_inventory(it);
    rooms[current_room].item = ITEM_NONE;
    /* auto-assemble master key when collecting 3 artifacts */
    if (it == ITEM_ART1 || it == ITEM_ART2 || it == ITEM_ART3) {
        int count = 0;
        for (int i = 0; i < inventory_count; ++i) if (inventory[i] == ITEM_ART1 || inventory[i] == ITEM_ART2 || inventory[i] == ITEM_ART3) ++count;
        if (count >= 3) {
            add_item_to_inventory(ITEM_KEY); /* repurpose KEY as master key for demo */
            printf("You feel the artifacts resonate and form a master key!\n");
        }
    }
    return true;
}

void world_print_inventory(void) {
    if (inventory_count == 0) { printf("Inventory: (empty)\n"); return; }
    printf("Inventory:\n");
    for (int i = 0; i < inventory_count; ++i) {
        switch (inventory[i]) {
            case ITEM_KEY: printf(" - Key\n"); break;
            case ITEM_LAMP: printf(" - Lamp\n"); break;
            case ITEM_BATTERY: printf(" - Battery\n"); break;
            case ITEM_ART1: printf(" - Artifact (1)\n"); break;
            case ITEM_ART2: printf(" - Artifact (2)\n"); break;
            case ITEM_ART3: printf(" - Artifact (3)\n"); break;
            default: break;
        }
    }
}

bool world_has_item(item_t item) {
    for (int i = 0; i < inventory_count; ++i) if (inventory[i] == item) return true;
    return false;
}

bool world_use_item(item_t item) {
    if (!world_has_item(item)) return false;
    if (item == ITEM_BATTERY && world_has_item(ITEM_LAMP)) {
        /* consume battery and light lamp */
        /* remove battery */
        int found = -1;
        for (int i = 0; i < inventory_count; ++i) if (inventory[i] == ITEM_BATTERY) { found = i; break; }
        if (found != -1) {
            for (int j = found; j+1 < inventory_count; ++j) inventory[j] = inventory[j+1];
            --inventory_count;
        }
        lamp_lit = true;
        printf("You install the battery in the lamp. The lamp lights up.\n");
        return true;
    }
    printf("You can't use that item here.\n");
    return false;
}

void world_use_menu(void) {
    printf("Use which item? Type number:\n");
    for (int i = 0; i < inventory_count; ++i) {
        printf(" %d) ", i+1);
        switch (inventory[i]) {
            case ITEM_KEY: printf("Key\n"); break;
            case ITEM_LAMP: printf("Lamp\n"); break;
            case ITEM_BATTERY: printf("Battery\n"); break;
            case ITEM_ART1: printf("Artifact (1)\n"); break;
            case ITEM_ART2: printf("Artifact (2)\n"); break;
            case ITEM_ART3: printf("Artifact (3)\n"); break;
            default: printf("Unknown\n"); break;
        }
    }
    printf("Choice: ");
    int choice = 0;
    if (fscanf(stdin, "%d", &choice) != 1) {
        /* consume rest of line */
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    /* consume rest of line */
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
    if (choice < 1 || choice > inventory_count) return;
    item_t it = inventory[choice-1];
    world_use_item(it);
}

bool world_check_win(void) {
    /* for demo: if player is in room 6 (gate) and has KEY (master) -> win */
    if (current_room == 6 && world_has_item(ITEM_KEY)) return true;
    return false;
}

void debug_print_world(void) {
    printf("debug: current_room=%d\n", current_room);
}

bool world_save_state(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return false;

    /* Save current game state: current_room, inventory, lamp_lit, room states */
    fwrite(&current_room, sizeof(current_room), 1, f);
    fwrite(&inventory_count, sizeof(inventory_count), 1, f);
    fwrite(inventory, sizeof(item_t), inventory_count, f);
    fwrite(&lamp_lit, sizeof(lamp_lit), 1, f);

    /* Save dynamic room state (items taken, doors unlocked) */
    for (int i = 0; i < 8; ++i) {
        fwrite(&rooms[i].item, sizeof(item_t), 1, f);
        fwrite(&rooms[i].locked, sizeof(bool), 1, f);
    }

    fclose(f);
    return true;
}

bool world_load_state(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return false;

    /* Load current game state */
    if (fread(&current_room, sizeof(current_room), 1, f) != 1) {
        fclose(f);
        return false;
    }
    if (fread(&inventory_count, sizeof(inventory_count), 1, f) != 1) {
        fclose(f);
        return false;
    }
    if (fread(inventory, sizeof(item_t), inventory_count, f) != (size_t)inventory_count) {
        fclose(f);
        return false;
    }
    if (fread(&lamp_lit, sizeof(lamp_lit), 1, f) != 1) {
        fclose(f);
        return false;
    }

    /* Load dynamic room state */
    for (int i = 0; i < 8; ++i) {
        if (fread(&rooms[i].item, sizeof(item_t), 1, f) != 1) {
            fclose(f);
            return false;
        }
        if (fread(&rooms[i].locked, sizeof(bool), 1, f) != 1) {
            fclose(f);
            return false;
        }
    }

    fclose(f);
    return true;
}

