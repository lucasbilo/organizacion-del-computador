#ifndef CACHE_OP_H
#define CACHE_OP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "./args.h"
#include "./stats.h"

typedef struct line line_t;
typedef struct set set_t;

typedef struct cache cache_t;

typedef struct address address_t;
typedef struct trace trace_t;

// trace
trace_t *create_trace();
void load_trace(trace_t *trace, args_t *args, unsigned int mem_addr);
void free_trace(trace_t *trace);
// cache
cache_t *create_cache(args_t *args);
void cache_operate(cache_t *cache, trace_t *trace, stats_t *stats, verbose_t *verbose, int operation_index, char op);
void free_cache(cache_t *cache);

#endif
