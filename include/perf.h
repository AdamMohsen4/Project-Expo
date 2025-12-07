#ifndef PERF_H
#define PERF_H

#include <stdint.h>

/* Performance counter structure holding CPU metrics */
typedef struct {
  uint32_t cycles;      /* Total CPU cycles */
  uint32_t instret;     /* Instructions retired */
  uint32_t mem_inst;    /* Memory instructions */
  uint32_t ic_miss;     /* Instruction cache misses */
  uint32_t dc_miss;     /* Data cache misses */
  uint32_t ic_stall;    /* Instruction cache stall cycles */
  uint32_t dc_stall;    /* Data cache stall cycles */
  uint32_t haz_stall;   /* Hazard stall cycles */
  uint32_t alu_stall;   /* ALU stall cycles */
} perf_t;

/* Reset all performance counters to zero */
void perf_clear_all(void);

/* Read all performance counters into structure */
void perf_read_all(perf_t *p);

/* Print performance data in CSV format with tag label */
void perf_print_csv(const char *tag, const perf_t *p);

/* Bench entrypoint (called from main when SW9 is set) */
void perf_run_benchmarks(void);

#endif
