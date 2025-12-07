#ifndef INPUT_H
#define INPUT_H

/* Initialize input system state */
void input_init(void);

/* Read current switch choice without consuming button press */
int  input_peek_choice(void);

/* Returns choice >=1 on BTN2 press edge, else -1 */
int  input_get_action(void);

#endif
