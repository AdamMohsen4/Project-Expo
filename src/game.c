#include "game.h"
#include "ui.h"
#include "input.h"
#include "world.h"
#include "board.h"

static uint8_t last_room = 255;

/* --- Boss fight helpers (no stdlib) --- */

static uint16_t lfsr16_step(uint16_t x) {
    /* 16-bit Galois LFSR */
    unsigned lsb = x & 1u;
    x >>= 1;
    if (lsb) x ^= 0xB400u;
    return x;
}

static int popcount10(uint16_t x) {
    int c = 0;
    x &= 0x03FFu;
    while (x) { c += (int)(x & 1u); x >>= 1; }
    return c;
}

static int btn_edge_pressed(void) {
    static unsigned armed = 1;
    unsigned b = board_get_button();
    if (!b) { armed = 1; return 0; }
    if (b && armed) { armed = 0; return 1; }
    return 0;
}

/* Returns 1 if won, 0 if lost */
static int boss_fire_fight(void) {
    /* Require extinguisher */
    if (!world_has_extinguisher()) {
        ui_println("You reach for something—anything—to stop the flames...");
        ui_println("But you have no fire extinguisher.");
        ui_println("The roots burn faster than you can react.");
        ui_println("");
        ui_println("*** YOU LOSE ***");
        return 0;
    }

    ui_println("");
    ui_println("As you push the gate open, you trip on a thick root.");
    ui_println("The lamp shatters—oil splashes—and the roots IGNITE.");
    ui_println("Fire crawls across the floor like living veins.");
    ui_println("");
    ui_println("BOSS FIGHT: PUT OUT THE FIRE");
    ui_println("LEDs = flames. They will spread.");
    ui_println("Use SW0..SW3 to choose an LED index (0-9).");
    ui_println("Press BTN2 to spray that LED (turn it off).");
    ui_println("Extinguish ALL flames before they fill the board!");
    ui_println("Press BTN2 to try proceed!");
    ui_println("");
    ui_wait_btn2();

    /* Fire uses LEDs directly (override inventory LEDs) */
    uint16_t fire = 0;
    uint16_t rng = (uint16_t)(board_get_switches() ^ 0xACE1u);
    if (rng == 0) rng = 0x1u;

    /* Ignite a few starting flames */
    for (int i = 0; i < 3; ++i) {
        for (int tries = 0; tries < 20; ++tries) {
            rng = lfsr16_step(rng);
            int bit = (int)(rng % 10u);
            uint16_t m = (uint16_t)(1u << bit);
            if (!(fire & m)) { fire |= m; break; }
        }
    }

    board_set_leds(fire);

    /* Timer: 30 seconds total, spreads every 1 second */
    const int TICKS_LIMIT = 300;     /* 30s @ 100ms */
    const int SPREAD_EVERY = 50;     /* 5s */
    int ticks = 0;
    int spread_acc = 0;

    while (1) {
        if (board_timer_poll_timeout()) {
            ticks++;
            spread_acc++;

            /* HUD for boss: HEX0-1 = target, HEX2-3 = flames count, HEX4-5 = seconds left */
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;

            int flames = popcount10(fire);
            int secs_left = (TICKS_LIMIT - ticks) / 10;
            if (secs_left < 0) secs_left = 0;

            board_set_hex_2dec(0, (uint8_t)target);
            board_set_hex_2dec(2, (uint8_t)flames);
            board_set_hex_2dec(4, (uint8_t)secs_left);

            /* Spread */
            if (spread_acc >= SPREAD_EVERY) {
                spread_acc = 0;

                /* Add one new flame on a random unlit LED */
                int placed = 0;
                for (int tries = 0; tries < 25; ++tries) {
                    rng = lfsr16_step(rng);
                    int bit = (int)(rng % 10u);
                    uint16_t m = (uint16_t)(1u << bit);
                    if (!(fire & m)) { fire |= m; placed = 1; break; }
                }
                (void)placed;
            }

            board_set_leds(fire);

            /* Lose if full fire or time out */
            if ((fire & 0x03FFu) == 0x03FFu || ticks >= TICKS_LIMIT) {
                ui_println("");
                ui_println("The flames race across the roots—too fast.");
                ui_println("The exit is swallowed by fire.");
                ui_println("");
                ui_println("*** YOU LOSE ***");
                return 0;
            }

            /* Win if no fire left */
            if ((fire & 0x03FFu) == 0) {
                ui_println("");
                ui_println("You choke the last flame into smoke.");
                ui_println("The roots blacken and crumble.");
                ui_println("");
                ui_println("*** FIRE EXTINGUISHED ***");
                return 1;
            }
        }

        /* Player action: BTN2 clears selected LED if on fire */
        if (btn_edge_pressed()) {
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;
            uint16_t m = (uint16_t)(1u << target);
            if (fire & m) {
                fire &= (uint16_t)~m;
                board_set_leds(fire);
            }
        }
    }
}

/* --- Existing UI helpers --- */

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

        if (choice == 31) {
            ui_println("");
            ui_println("[DEBUG] Boss fire fight test mode.");
            ui_println("Set SW0..SW3 to choose LED, BTN2 to spray.");
            ui_wait_btn2();
            world_debug_give_extinguisher();
            ui_println("[DEBUG] Fire extinguisher granted.");

            /* Call the same boss fight as the real ending */
            extern int boss_fire_fight(void); /* or move the prototype up if needed */
            {
                int won = boss_fire_fight();
                if (won) {
                    ui_println("[DEBUG] Boss cleared (world state unchanged).");
                } else {
                    ui_println("[DEBUG] Boss failed (world state unchanged).");
                }
                ui_println("Press BTN2 to return to the dungeon...");
                ui_wait_btn2();
            }
            /* Skip normal command handling this loop */
            continue;
        }


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
            ui_println("    THE GATE OPENS...");
            ui_println("================================");
            ui_println("");

            /* Boss fight replaces instant-win */
            int won = boss_fire_fight();

            if (won) {
                ui_println("");
                ui_println("================================");
                ui_println("    VICTORY ACHIEVED!");
                ui_println("================================");
                ui_println("");
                ui_println("The MASTER KEY holds. The path is clear.");
                ui_println("You step into sunlight—free at last.");
                ui_println("");
                ui_println("*** YOU WIN ***");
                ui_println("");
                break;
            } else {
                ui_println("");
                ui_println("You blink—and find yourself back at the entrance.");
                ui_println("The dungeon resets its cruel loop.");
                ui_println("");
                ui_println("Press BTN2 to try again...");
                ui_wait_btn2();

                /* Restart game state */
                world_init();
                last_room = 255;
                print_menu();
            }
        }
    }
}
