#include "perf.h"
#include "ui.h"
#include "board.h"
#include "world.h"

/* lab printf helpers (ui wraps them, but we want exact control) */
extern void print(char *);
extern void printc(char);
extern void print_dec(unsigned int);

static volatile uint32_t perf_sink;

#define CSR_READ(name, outvar) asm volatile("csrr %0, " #name : "=r"(outvar))
#define CSR_ZERO(name)         asm volatile("csrw " #name ", x0")

void perf_clear_all(void) {
  /* If your core supports mcountinhibit and it’s nonzero, you can zero it:
     asm volatile("csrw mcountinhibit, x0");
     (Leave out if assembler complains.) */
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

void perf_print_csv(const char *tag, const perf_t *p) {
  /* Easy to paste into Excel:
     tag,cycles,instret,mem,ic_miss,dc_miss,ic_stall,dc_stall,haz,alu */
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

/* ---------------- Bench kernels ---------------- */

/* This avoids UART prints in world.c affecting results:
   compile perf build with -DPERF_SILENT and make ui_print... no-op (see section D). */

/* scripted “win path” to stress your actual world logic */
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

/* LFSR + popcount + modulo/div-heavy logic: good for mhpmcounter9 */
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

static void kernel_boss_sim(unsigned steps) {
  uint16_t rng = 0xACE1u;
  uint16_t fire = 0x0007u; /* start with 3 flames */

  for (unsigned i = 0; i < steps; i++) {
    rng = lfsr16_step(rng);

    /* spray */
    unsigned target = (unsigned)(rng % 10u);      /* modulo => possible div/rem */
    fire &= (uint16_t)~(1u << target);

    /* spread occasionally */
    if ((i & 31u) == 0u) {
      unsigned bit = (unsigned)((rng >> 3) % 10u);
      fire |= (uint16_t)(1u << bit);
      fire &= 0x03FFu;
    }

    /* keep it “alive” */
    if (popcount10(fire) == 0) fire = 1u;
  }
  perf_sink ^= (uint32_t)fire;
  perf_sink ^= (uint32_t)rng;
}

/* ---------------- Top-level runner ---------------- */

static void run_one(const char *tag, void (*fn)(unsigned), unsigned n) {
  perf_t p;
  perf_clear_all();
  fn(n);
  perf_read_all(&p);
  perf_print_csv(tag, &p);
}

void perf_run_benchmarks(void) {
  print_str("\n=== PERF MODE (SW9=1) ===\n");
  print_str("CSV: tag,cycles,instret,mem,ic_miss,dc_miss,ic_stall,dc_stall,haz,alu\n");

  /* Warm-up (optional): */
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
