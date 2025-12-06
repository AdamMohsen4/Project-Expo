#include "game.h"
#include "ui.h"
#include "input.h"
#include "world.h"
#include "board.h"

static uint8_t last_room = 255;

static void print_exits(void){
    int n = world_exit_count();
    if(n == 0){
        ui_println("Exits: None");
        return;
    }

    ui_print("Exits: ");
    for(int i = 0; i < n; i++){
        const char *dir = world_exit_name(i);
        ui_print(dir);
        if(i < n-1) ui_print(", ");
    }
    ui_println("");
}

static void print_menu(void){
    int n = world_exit_count();
    ui_println("");
    ui_println("--- Commands ---");

    /* Movement options */
    for(int i = 0; i < n; i++){
        ui_print("["); ui_print_dec((unsigned)(i+1)); ui_print("] Go ");
        ui_println(world_exit_name(i));
    }

    /* Action options */
    ui_print("["); ui_print_dec((unsigned)(n+1)); ui_println("] Look around");
    ui_print("["); ui_print_dec((unsigned)(n+2)); ui_println("] Take item");
    ui_print("["); ui_print_dec((unsigned)(n+3)); ui_println("] Use item");
    ui_print("["); ui_print_dec((unsigned)(n+4)); ui_println("] Quit game");
    ui_println("");
}

void game_init(void){
    ui_println("=== SWITCHBACK ===");
    ui_println("");
    ui_println("You awaken in an ancient dungeon.");
    ui_println("To escape, you must:");
    ui_println("");
    ui_println("1. Find 3 ARTIFACTS hidden in");
    ui_println("   locked and dark chambers");
    ui_println("2. They will fuse into a MASTER KEY");
    ui_println("3. Reach the Sealed Gate to escape");
    ui_println("");
    ui_println("Explore, solve puzzles, collect");
    ui_println("items. Good luck, adventurer.");
    ui_println("");
    ui_println("Press BTN2 to begin...");
    ui_wait_btn2();
    print_menu();
}

static void maybe_print_room(void){
    uint8_t r = world_room_id();
    if(r==last_room) return;
    last_room = r;
    ui_println("");
    ui_println("================================");
    ui_println(world_room_name());
    ui_println("================================");
    ui_println(world_room_desc());
    ui_println("");
    print_exits();
    print_menu();
}

void game_loop(void){
    while(1){
        /* HUD updates while waiting for input */
        if(board_timer_poll_timeout()){
            ui_hud(world_room_id(), world_inventory_count(),
                   (uint8_t)input_peek_choice(), world_led_mask());
        }

        maybe_print_room();

        int choice = input_get_action();
        if(choice < 0) continue;

        int n = world_exit_count();
        if(choice >= 1 && choice <= n){
            if(!world_try_move(choice-1)){
                ui_wait_btn2();
            }
        } else {
            int cmd = choice - (n+1);
            if(cmd==0){
                ui_println("");
                ui_println(world_room_desc());
                ui_println("");
                print_exits();
                print_menu();
                ui_wait_btn2();
            }
            else if(cmd==1){ world_take_item(); ui_wait_btn2(); }
            else if(cmd==2){ world_use_action(); ui_wait_btn2(); }
            else if(cmd==3){
                ui_println("");
                ui_println("Thanks for playing SWITCHBACK!");
                ui_println("See you next time, adventurer.");
                break;
            }
            else { ui_println("Unknown command."); ui_wait_btn2(); }
        }

        if(world_check_win()){
            ui_println("");
            ui_println("================================");
            ui_println("    VICTORY ACHIEVED!");
            ui_println("================================");
            ui_println("");
            ui_println("The MASTER KEY slides into the");
            ui_println("ancient mechanism. Gears grind");
            ui_println("and the massive gate slowly opens,");
            ui_println("revealing sunlight beyond!");
            ui_println("");
            ui_println("You step through to freedom.");
            ui_println("");
            ui_println("*** YOU WIN ***");
            ui_println("");
            break;
        }
    }
}
