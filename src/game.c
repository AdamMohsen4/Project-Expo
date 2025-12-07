// Authors: Adam Mohsen, Hjalmar Albinsson
#include "game.h"
#include "ui.h"
#include "input.h"
#include "world.h"
#include "board.h"

/* Track room changes for printing */
static uint8_t last_room = 255;

/* 16-bit LFSR (Linear Feedback Shift Register) */
static uint16_t lfsr16_step(uint16_t x) {
    unsigned lsb = x & 1u;
    x >>= 1;
    if (lsb) x ^= 0xB400u;
    return x;
}

/* Count set bits in lower 10 bits */
static int popcount10(uint16_t x) {
    int c = 0;
    x &= 0x03FFu;
    while (x) { c += (int)(x & 1u); x >>= 1; }
    return c;
}

/* Separate button handler for boss fight */
static int btn_edge_pressed_local(void) {
    static unsigned armed = 1;
    unsigned b = board_get_button();
    if (!b) { armed = 1; return 0; }
    if (b && armed) { armed = 0; return 1; }
    return 0;
}

/* Restore normal HUD after boss fight */
static void restore_normal_io(void) {
    ui_hud(world_room_id(),
           world_inventory_count(),
           (uint8_t)input_peek_choice(),
           world_led_mask());
}

// Hjalmar
/* Boss fight minigame - extinguish spreading flames */
static int boss_fire_fight(void) {
    ui_println("");
    ui_println("As you open the gate ready to leave, you trip on a root.");
    ui_println("You fall — the lamp shatters — oil splashes.");
    ui_println("The roots catch fire and the flames spread fast.");
    ui_println("");
    ui_println("BOSS FIGHT: PUT OUT THE FIRE!");
    ui_println("LEDs = flames. They will spread until everything is burning.");
    ui_println("Use SW0..SW3 to pick a LED index (0-9). BTN2 sprays it.");
    ui_println("Extinguish all flames to escape!");
    ui_println("");
    ui_wait_btn2();

    /* Seed RNG from switches */
    uint16_t rng = (uint16_t)(board_get_switches() ^ 0xACE1u);
    if (rng == 0) rng = 0x1u;

    /* Light 3 random flames */
    uint16_t fire = 0;
    for (int i = 0; i < 3; ++i) {
        for (int tries = 0; tries < 20; ++tries) {
            rng = lfsr16_step(rng);
            int bit = (int)(rng % 10u);
            uint16_t m = (uint16_t)(1u << bit);
            if (!(fire & m)) {
                fire |= m; break;
            }
        }
    }

    board_set_leds(fire);

    /* Fire spreads every 5 seconds */
    const int SPREAD_EVERY = 50;
    int spread_acc = 0;

    while (1) {
        /* Player spray action */
        if (btn_edge_pressed_local()) {
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;
            uint16_t m = (uint16_t)(1u << target);
            if (fire & m) {
                fire &= (uint16_t)~m;
                board_set_leds(fire);
            }
        }

        /* Update boss state on timer tick */
        if (board_timer_poll_timeout()) {
            spread_acc++;

            /* Update HUD: target, flame count, room */
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;

            int flames = popcount10(fire);
            board_set_hex_2dec(0, (uint8_t)target);
            board_set_hex_2dec(2, (uint8_t)flames);
            board_set_hex_2dec(4, world_room_id());

            /* Spread fire periodically */
            if (spread_acc >= SPREAD_EVERY) {
                spread_acc = 0;

                for (int tries = 0; tries < 25; ++tries) {
                    rng = lfsr16_step(rng);
                    int bit = (int)(rng % 10u);
                    uint16_t m = (uint16_t)(1u << bit);
                    if (!(fire & m)) { fire |= m; break; }
                }
                board_set_leds(fire);
            }

            /* Check win/lose conditions */
            if ((fire & 0x03FFu) == 0) {
                ui_println("");
                ui_println("You choke the last flame into smoke.");
                ui_println("The roots blacken and crumble.");
                ui_println("");
                ui_println("*** FIRE EXTINGUISHED ***");
                return 1;
            }

            if ((fire & 0x03FFu) == 0x03FFu) {
                ui_println("");
                ui_println("The fire races through the roots.");
                ui_println("Everything burns. The exit is lost.");
                ui_println("");
                ui_println("*** YOU LOSE ***");
                return 0;
            }
        }
    }
}

// Adam:
/* Print available exits from current room */
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

/* Print command menu */
static void print_menu(void){
    int n = world_exit_count();
    ui_println("");
    ui_println("--- Commands ---");

    for(int i = 0; i < n; i++){
        ui_print("["); ui_print_dec((unsigned)(i+1)); ui_print("] Go ");
        ui_println(world_exit_name(i));
    }

    ui_print("["); ui_print_dec((unsigned)(n+1)); ui_println("] Look around");
    ui_print("["); ui_print_dec((unsigned)(n+2)); ui_println("] Take item");
    ui_print("["); ui_print_dec((unsigned)(n+3)); ui_println("] Use item");
    ui_print("["); ui_print_dec((unsigned)(n+4)); ui_println("] Quit game");
    ui_println("");
}

/* Display game intro and instructions */
void game_init(void){
    ui_println("=== GET OUT ===");
    ui_println("");
    ui_println("You awaken in an ancient dungeon.");
    ui_println("To escape, you must:");
    ui_println("");
    ui_println("1. Find 3 ARTIFACTS hidden in");
    ui_println("   locked and dark chambers");
    ui_println("2. They will fuse into a MASTER KEY");
    ui_println("3. Reach the Sealed Gate to escape");
    ui_println("");
    ui_println("Press BTN2 to begin...");
    ui_wait_btn2();
    print_menu();
}

/* Print room info when entering new room */
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

// Adam:
/* Main game loop */
void game_loop(void){
    while(1){
        /* Update HUD on timer tick */
        if(board_timer_poll_timeout()){
            ui_hud(world_room_id(), world_inventory_count(),
                   (uint8_t)input_peek_choice(), world_led_mask());
        }

        maybe_print_room();

        int choice = input_get_action();
        if(choice < 0) continue;

        /* Handle movement commands */
        int n = world_exit_count();
        if(choice >= 1 && choice <= n){
            if(!world_try_move(choice-1)){
                ui_wait_btn2();
            }
        } else {
            /* Handle action commands */
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
                break;
            }
            else { ui_println("Unknown command."); ui_wait_btn2(); }
        }

        /* Check win condition and trigger boss fight */
        if(world_check_win()){
            ui_println("");
            ui_println("================================");
            ui_println("    THE GATE OPENS...");
            ui_println("================================");

            int won = boss_fire_fight();

            restore_normal_io();

            if (won) {
                ui_println("");
                ui_println("================================");
                ui_println("    VICTORY ACHIEVED!");
                ui_println("================================");
                ui_println("The path is clear. You escape into sunlight.");
                ui_println("");
                ui_println("Press BTN2 to restart for the next player...");
                ui_wait_btn2();

                world_init();
                last_room = 255;
                restore_normal_io();
                print_menu();
            } else {
                ui_println("");
                ui_println("You blink—and find yourself back at the entrance.");
                ui_println("Press BTN2 to try again...");
                ui_wait_btn2();

                world_init();
                last_room = 255;
                restore_normal_io();
                print_menu();
            }
        }
    }
}
