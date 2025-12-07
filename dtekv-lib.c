#include "dtekv-lib.h"

/* JTAG UART memory-mapped I/O addresses */
#define JTAG_UART ((volatile unsigned int*) 0x04000040)
#define JTAG_CTRL ((volatile unsigned int*) 0x04000044)

/* Print single character to JTAG UART */
void printc(char s)
{
    /* Wait for write FIFO space (check upper 16 bits of control register) */
    while (((*JTAG_CTRL)&0xffff0000) == 0);
    *JTAG_UART = s;
}

/* Print null-terminated string character by character */
void print(char *s)
{
  while (*s != '\0') {
      printc(*s);
      s++;
  }
}

/* Print unsigned integer in decimal format */
void print_dec(unsigned int x)
{
  unsigned divident = 1000000000;
  char first = 0;  /* Track if first non-zero digit found */
  do {
    int dv = x / divident;
    if (dv != 0) first = 1;  /* Mark first non-zero digit */
    if (first != 0)
      printc(48+dv);  /* Convert digit to ASCII (0 = 48) */
    x -= dv*divident;
    divident /= 10;
  } while (divident != 0);
    if (first == 0)  /* Handle special case: print '0' for zero input */
	printc(48);
}

/* Print unsigned 32-bit integer in hexadecimal with 0x prefix */
void print_hex32 ( unsigned int x)
{
  printc('0');
  printc('x');
  /* Process 8 hex digits from most to least significant */
  for (int i = 7; i >= 0; i--) {
    char hd = (char) ((x >> (i*4)) & 0xf);  /* Extract 4-bit nibble */
    if (hd < 10)
      hd += '0';  /* Convert 0-9 to ASCII */
    else
      hd += ('A' - 10);  /* Convert 10-15 to A-F */
    printc(hd);
  }
}

/* Handle CPU exceptions and syscalls based on mcause register */
void handle_exception ( unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3, unsigned arg4, unsigned arg5, unsigned mcause, unsigned syscall_num )
{
  switch (mcause)
    {
    case 0:  /* Instruction address misalignment */
      print("\n[EXCEPTION] Instruction address misalignment. ");
      break;
    case 2:  /* Illegal instruction */
      print("\n[EXCEPTION] Illegal instruction. ");
      break;
    case 11:  /* Environment call (syscall) */
      if (syscall_num == 4)  /* Print string syscall */
	print((char*) arg0);
      if (syscall_num == 11)  /* Print char syscall */
	printc(arg0);
      return ;
      break;
    default:
      print("\n[EXCEPTION] Unknown error. ");
      break;
    }

  /* Print exception address and halt */
  print("Exception Address: ");
  print_hex32(arg0); printc('\n');
  while (1);  /* Infinite loop - system halt */
}

/*
 * nextprime
 * 
 * Return the first prime number larger than the integer
 * given as a parameter. The integer must be positive.
 */
#define PRIME_FALSE   0     /* Constant to help readability. */
#define PRIME_TRUE    1     /* Constant to help readability. */
int nextprime( int inval )
{
   register int perhapsprime = 0; /* Holds a tentative prime while we check it. */
   register int testfactor; /* Holds various factors for which we test perhapsprime. */
   register int found;      /* Flag, false until we find a prime. */

   /* Initial sanity check of parameter */
   if (inval < 3 )
   {
     if(inval <= 0) return(1);  /* Return 1 for zero or negative input. */
     if(inval == 1) return(2);  /* Easy special case. */
     if(inval == 2) return(3);  /* Easy special case. */
   }
   else
   {
     /* Testing an even number for primeness is pointless, since
      * all even numbers are divisible by 2. Therefore, we make sure
      * that perhapsprime is larger than the parameter, and odd. */
     perhapsprime = ( inval + 1 ) | 1 ;  /* Ensure odd number greater than input */
   }
   /* While prime not found, loop through odd candidates */
   for( found = PRIME_FALSE; found != PRIME_TRUE; perhapsprime += 2 )
   {
     /* Check factors from 3 up to perhapsprime/2 */
     for( testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1 )
     {
       found = PRIME_TRUE;      /* Assume we will find a prime. */
       if( (perhapsprime % testfactor) == 0 ) /* If testfactor divides perhapsprime... */
       {
         found = PRIME_FALSE;   /* ...then, perhapsprime was non-prime. */
         goto check_next_prime; /* Break the inner loop, go test a new perhapsprime. */
       }
     }
     check_next_prime:;         /* Label used to break the inner loop */
     if( found == PRIME_TRUE )  /* If the loop ended normally, we found a prime */
     {
       return( perhapsprime );  /* Return the prime we found */
     }
   }
   return( perhapsprime );      /* When the loop ends, perhapsprime is a real prime */
} 
