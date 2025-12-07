#include "perf.h"
#include "ui.h"
#include "board.h"
#include "world.h"

/* Direct access to print functions for perf output */
extern void print(char *);
extern void printc(char);
extern void print_dec(unsigned int);

/* CSR access macros */
#define CSR_READ(name, outvar) asm volatile("csrr %0, " #name : "=r"(outvar))
#define CSR_ZERO(name)         asm volatile("csrw " #name ", x0")

/* Zero all performance counters */
void perf_clear_all(void) {
  CSR_ZERO(mcycle);
  CSR_ZERO(minstret);
  CSR_ZERO(mhpmcounter3);
  CSR_ZERO(mhpmcounter4);
  CSR_ZERO(mhpmcounter5);
  CSR_ZERO(mhpmcounter6);
  CSR_ZERO(mhpmcounter7);
  CSR_ZERO(mhpmcounter8);
  CSR_ZERO(mhpmcounter9);
}

/* Read all counters into struct */
void perf_read_all(perf_t *p) {
  CSR_READ(mcycle,       p->cycles);
  CSR_READ(minstret,     p->instret);
  CSR_READ(mhpmcounter3, p->mem_inst);
  CSR_READ(mhpmcounter4, p->ic_miss);
  CSR_READ(mhpmcounter5, p->dc_miss);
  CSR_READ(mhpmcounter6, p->ic_stall);
  CSR_READ(mhpmcounter7, p->dc_stall);
  CSR_READ(mhpmcounter8, p->haz_stall);
  CSR_READ(mhpmcounter9, p->alu_stall);
}

static void print_str(const char *s){ print((char*)s); }

/* Print perf data as CSV row */
void perf_print_csv(const char *tag, const perf_t *p) {
  print_str(tag); printc(',');
  print_dec(p->cycles); printc(',');
  print_dec(p->instret); printc(',');
  print_dec(p->mem_inst); printc(',');
  print_dec(p->ic_miss); printc(',');
  print_dec(p->dc_miss); printc(',');
  print_dec(p->ic_stall); printc(',');
  print_dec(p->dc_stall); printc(',');
  print_dec(p->haz_stall); printc(',');
  print_dec(p->alu_stall); printc('\n');
}

/* Benchmark kernels */

/* Scripted game win path - tests world logic */
static void kernel_win_path(unsigned iters) {
  for (unsigned t = 0; t < iters; t++) {
    world_init();
    world_take_item();               /* lamp in room 0 */

    /* 0 -> 1 (east) -> 3 (east) take key -> 5 (east) take matches */
    world_try_move(0);               /* exit index 0 from room0 is east (only east/south exist) */
    world_try_move(1);               /* from hallway, exit index 1 is east (depends on ordering) */

    world_take_item();               /* key */
    world_try_move(1);               /* east to maintenance */
    world_take_item();               /* matches */
    world_use_action();              /* light lamp */

    /* back to closet -> dark chamber */
    world_try_move(0);               /* west */
    world_try_move(0);               /* north */
    world_take_item();               /* A2 */

    /* back to hallway -> locked chamber -> A1 */
    world_try_move(0);               /* south */
    world_try_move(1);               /* west */
    world_try_move(0);               /* north (unlock) */
    world_take_item();               /* A1 */

    /* back to entrance -> alcove -> A3 (fuse master) */
    world_try_move(0);               /* south */
    world_try_move(0);               /* west */
    world_try_move(1);               /* south to alcove */
    world_take_item();               /* A3 => master */

    /* go to sealed gate (room 6) */
    world_try_move(0);               /* north */
    world_try_move(0);               /* east */
    world_try_move(0);               /* north */
    world_try_move(0);               /* north to gate */
    (void)world_check_win();
  }
}

/* 16-bit LFSR for pseudo-random numbers */
static uint16_t lfsr16_step(uint16_t x) {
  unsigned lsb = x & 1u;
  x >>= 1;
  if (lsb) x ^= 0xB400u;  /* Tap polynomial */
  return x;
}

/* Count set bits in lower 10 bits */
static int popcount10(uint16_t x) {
  int c = 0;
  x &= 0x03FFu;
  while (x) { c += (int)(x & 1u); x >>= 1; }
  return c;
}

/* Boss fight simulator - stresses ALU with modulo/div ops */
static void kernel_boss_sim(unsigned steps) {
  uint16_t rng = 0xACE1u;
  uint16_t fire = 0x0007u;

  for (unsigned i = 0; i < steps; i++) {
    rng = lfsr16_step(rng);

    unsigned target = (unsigned)(rng % 10u);
    fire &= (uint16_t)~(1u << target);

    if ((i & 31u) == 0u) {
      unsigned bit = (unsigned)((rng >> 3) % 10u);
      fire |= (uint16_t)(1u << bit);
      fire &= 0x03FFu;
    }

    if (popcount10(fire) == 0) fire = 1u;
  }
}

/* Run single benchmark and print results */
static void run_one(const char *tag, void (*fn)(unsigned), unsigned n) {
  perf_t p;
  perf_clear_all();
  fn(n);
  perf_read_all(&p);
  perf_print_csv(tag, &p);
}

/* Main benchmark entry point */
void perf_run_benchmarks(void) {
  print_str("\n=== PERF MODE (SW9=1) ===\n");
  print_str("CSV: tag,cycles,instret,mem,ic_miss,dc_miss,ic_stall,dc_stall,haz,alu\n");

  kernel_win_path(10);
  kernel_boss_sim(200);

  run_one("WINPATH_50",   kernel_win_path, 50);
  run_one("WINPATH_200",  kernel_win_path, 200);
  run_one("WINPATH_800",  kernel_win_path, 800);

  run_one("BOSS_1000",    kernel_boss_sim, 1000);
  run_one("BOSS_5000",    kernel_boss_sim, 5000);
  run_one("BOSS_20000",   kernel_boss_sim, 20000);

  print_str("=== END PERF ===\n");
  while (1) {}
}
