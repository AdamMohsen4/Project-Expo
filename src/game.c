/* game.c - host-mode game wiring and loop
 * Implements a simple blocking menu loop that uses the world, ui, and input
 * abstractions. This is intended for local testing without the DTEK-V board.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "game.h"
#include "world.h"
#include "ui.h"
#include "input.h"

static bool quit_requested = false;

void game_init(void) {
    input_init();
    world_init();
}

static void show_room(void) {
    const char *name = world_get_room_name();
    const char *desc = world_get_room_description();
    ui_print_paged(name);
    ui_print_paged(desc);
}

static void show_menu(void) {
    printf("\nActions:\n");
    int n = world_get_exit_count();
    for (int i = 0; i < n; ++i) {
        const char *dir = world_get_exit_name(i);
        printf(" %d) Go %s\n", i+1, dir);
    }
    int base = n + 1;
    printf(" %d) Look\n", base);
    printf(" %d) Take item\n", base+1);
    printf(" %d) Inventory\n", base+2);
    printf(" %d) Use item\n", base+3);
    printf(" %d) Quit\n", base+4);
    printf("Choose an action: ");
}

void game_tick(void) {
    show_room();
    show_menu();
    int choice = input_get_action();
    if (choice <= 0) return;

    int n = world_get_exit_count();
    if (choice >= 1 && choice <= n) {
        int dir = choice - 1;
        if (!world_try_move(dir)) {
            printf("You can't go that way.\n");
        }
        return;
    }
    int base = n + 1;
    switch (choice - base) {
        case 0: /* Look */
            ui_print_paged(world_get_room_description());
            break;
        case 1: /* Take */
            if (world_take_room_item()) {
                printf("You picked up the item.\n");
            } else {
                printf("There is nothing to take here.\n");
            }
            break;
        case 2: /* Inventory */
            world_print_inventory();
            break;
        case 3: /* Use */
            world_use_menu();
            break;
        case 4: /* Quit */
            quit_requested = true;
            break;
        default:
            printf("Unknown action.\n");
    }
}

bool game_won(void) {
    return world_check_win() || quit_requested;
}

void game_loop(void) {
    while (!game_won()) {
        game_tick();
    }
    if (world_check_win()) {
        ui_print_paged("Congratulations — you assembled the master key and escaped!");
    } else {
        ui_print_paged("Goodbye.");
    }
}
