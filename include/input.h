#ifndef INPUT_H
#define INPUT_H

/* Input abstraction: map board buttons/switches to simple actions */

void input_init(void);

/* return an integer action code, -1 for none */
int input_get_action(void);

#endif /* INPUT_H */
