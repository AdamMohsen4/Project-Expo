#include "input.h"
#include "board.h"
#include <stdio.h>

extern void board_terminal_set_choice(int choice);
extern void board_terminal_press_button(void);
extern void board_terminal_release_button(void);

void input_init(void) {
    /* Nothing needed */
}

int input_peek_choice(void) {
    return (int)(board_get_switches() & 0x1Fu);
}

int input_get_action(void) {
    printf("\n> ");
    fflush(stdout);

    int choice;
    if (scanf("%d", &choice) != 1) {
        /* Clear input buffer on error */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }

    /* Clear remaining input */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (choice <= 0 || choice > 31) {
        return -1;
    }

    board_terminal_set_choice(choice);
    board_terminal_press_button();
    board_terminal_release_button();

    return choice;
}
