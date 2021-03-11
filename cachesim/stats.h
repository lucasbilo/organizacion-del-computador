#ifndef STATS_H
#define STATS_H

#include "./args.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int loads;
    int rmiss;
    int dirty_rmiss;
    int bytes_read;
    int read_time;
} stats_read_t;

typedef struct
{
    int stores;
    int wmiss;
    int dirty_wmiss;
    int bytes_written;
    int write_time;
} stats_write_t;

typedef struct
{
    stats_write_t *stats_write;
    stats_read_t *stats_read;
} stats_t;

typedef struct
{
    int operation_index;
    char *case_identifier;
    int cache_index;
    int cache_tag;
    int cache_line;
    int line_tag;
    int valid_bit;
    int dirty_bit;
    int last_used;
} verbose_t;

//
stats_t *create_stats();
void print_stats(stats_t *stats, args_t *args);
void free_stats(stats_t *stats);
//
verbose_t *create_verbose();
void print_verbose(verbose_t *verbose);
void free_verbose();
#endif