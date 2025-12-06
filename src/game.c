#include "game.h"
#include "ui.h"
#include "input.h"
#include "world.h"
#include "board.h"

static uint8_t last_room = 255;

static void print_menu(void){
    int n = world_exit_count();
    ui_println("");
    ui_println("Pick # with SW0..SW4, press BTN2.");
    ui_print("Exits 1.."); ui_print_dec((unsigned)n); ui_println(" = move.");
    ui_print("Look=");     ui_print_dec((unsigned)(n+1)); ui_println("");
    ui_print("Take=");     ui_print_dec((unsigned)(n+2)); ui_println("");
    ui_print("Use=");      ui_print_dec((unsigned)(n+3)); ui_println("");
    ui_print("Quit=");     ui_print_dec((unsigned)(n+4)); ui_println("");
}

void game_init(void){
    ui_println("Switchback (DTEK-V)");
    ui_println("Goal: collect 3 artifacts -> MASTER KEY -> reach Sealed Gate.");
    ui_wait_btn2();
    print_menu();
}

static void maybe_print_room(void){
    uint8_t r = world_room_id();
    if(r==last_room) return;
    last_room = r;
    ui_println("");
    ui_println(world_room_name());
    ui_println(world_room_desc());
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
            if(cmd==0){ ui_println(world_room_desc()); ui_wait_btn2(); }
            else if(cmd==1){ world_take_item(); ui_wait_btn2(); }
            else if(cmd==2){ world_use_action(); ui_wait_btn2(); }
            else if(cmd==3){ ui_println("Bye."); break; }
            else { ui_println("Unknown."); ui_wait_btn2(); }
        }

        if(world_check_win()){
            ui_println("MASTER KEY accepted. Gate opens. YOU WIN.");
            break;
        }
    }
}
