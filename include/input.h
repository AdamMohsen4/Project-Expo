#ifndef INPUT_H
#define INPUT_H

void input_init(void);
int  input_peek_choice(void);
int  input_get_action(void); /* returns choice >=1 on BTN2 press edge, else -1 */

#endif
