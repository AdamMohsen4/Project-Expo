#include "game.h"
#include "ui.h"
#include "input.h"
#include "world.h"
#include "board.h"

static uint8_t last_room = 255;

/* --- helpers --- */

static uint16_t lfsr16_step(uint16_t x) {
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

/* Local button edge detector for boss fight (separate from input.c’s armed state) */
static int btn_edge_pressed_local(void) {
    static unsigned armed = 1;
    unsigned b = board_get_button();
    if (!b) { armed = 1; return 0; }
    if (b && armed) { armed = 0; return 1; }
    return 0;
}

static void restore_normal_io(void) {
    /* Force exit from “fire mode”: restore normal HUD + inventory LEDs immediately */
    ui_hud(world_room_id(),
           world_inventory_count(),
           (uint8_t)input_peek_choice(),
           world_led_mask());
}

/* Returns 1 if boss cleared, 0 if failed */
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

    /* Seed RNG from switches so it feels different each run */
    uint16_t rng = (uint16_t)(board_get_switches() ^ 0xACE1u);
    if (rng == 0) rng = 0x1u;

    /* Start with a few flames */
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

    /* Enter fire mode: override LEDs */
    board_set_leds(fire);

    /* Spread once per second (10 * 100ms) */
    const int SPREAD_EVERY = 50;
    int spread_acc = 0;

    while (1) {
        /* Spray action is immediate on BTN2 edge */
        if (btn_edge_pressed_local()) {
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;
            uint16_t m = (uint16_t)(1u << target);
            if (fire & m) {
                fire &= (uint16_t)~m;
                board_set_leds(fire);
            }
        }

        /* Tick boss logic on timer */
        if (board_timer_poll_timeout()) {
            spread_acc++;

            /* Boss HUD on HEX:
               HEX0-1 = target
               HEX2-3 = flames count
               HEX4-5 = room id (still 6, so player knows “final gate”) */
            int target = (int)(board_get_switches() & 0x0Fu);
            if (target > 9) target = 9;

            int flames = popcount10(fire);
            board_set_hex_2dec(0, (uint8_t)target);
            board_set_hex_2dec(2, (uint8_t)flames);
            board_set_hex_2dec(4, world_room_id());

            /* Spread */
            if (spread_acc >= SPREAD_EVERY) {
                spread_acc = 0;

                /* Add one new flame on a random unlit LED */
                for (int tries = 0; tries < 25; ++tries) {
                    rng = lfsr16_step(rng);
                    int bit = (int)(rng % 10u);
                    uint16_t m = (uint16_t)(1u << bit);
                    if (!(fire & m)) { fire |= m; break; }
                }
                board_set_leds(fire);
            }

            /* Win / Lose */
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

/* --- existing UI helpers --- */

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
        /* Normal HUD updates */
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
                break;
            }
            else { ui_println("Unknown command."); ui_wait_btn2(); }
        }

        if(world_check_win()){
            ui_println("");
            ui_println("================================");
            ui_println("    THE GATE OPENS...");
            ui_println("================================");

            /* Boss replaces instant win */
            int won = boss_fire_fight();

            /* IMPORTANT: leave fire mode immediately */
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

                /* Reset entire game state (items/bools/locks/etc.) */
                world_init();
                last_room = 255;
                restore_normal_io();
                print_menu();
            } else {
                ui_println("");
                ui_println("You blink—and find yourself back at the entrance.");
                ui_println("Press BTN2 to try again...");
                ui_wait_btn2();

                /* Reset entire game state (items/bools/locks/etc.) */
                world_init();
                last_room = 255;
                restore_normal_io();
                print_menu();
            }
        }
    }
}
