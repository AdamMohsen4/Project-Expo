/* Print single character to JTAG UART */
void printc(char );

/* Print null-terminated string to JTAG UART */
void print(char *);

/* Print unsigned integer in decimal format */
void print_dec(unsigned int);

/* Print unsigned integer in hexadecimal format (0x prefix) */
void print_hex32 ( unsigned int);

/* Handle system exceptions and syscalls */
void handle_exception ( unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3, unsigned arg4, unsigned arg5, unsigned mcause, unsigned syscall_num );

/* Find the first prime number larger than input value */
int nextprime( int inval );




