#ifndef PERF_H
#define PERF_H

#include <stdint.h>

typedef struct {
  uint32_t cycles;
  uint32_t instret;
  uint32_t mem_inst;
  uint32_t ic_miss;
  uint32_t dc_miss;
  uint32_t ic_stall;
  uint32_t dc_stall;
  uint32_t haz_stall;
  uint32_t alu_stall;
} perf_t;

void perf_clear_all(void);
void perf_read_all(perf_t *p);
void perf_print_csv(const char *tag, const perf_t *p);

/* Bench entrypoint (called from main when SW9 is set) */
void perf_run_benchmarks(void);

#endif
