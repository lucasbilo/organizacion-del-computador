#include "args.h"
#include "cachesim.h"
#include "./cache_op.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void free_memory(cache_t *cache, FILE *file, stats_t *stats, verbose_t *verbose, trace_t *trace, char *row)
{
    free_cache(cache);
    fclose(file);
    if (stats) free_stats(stats);
    if (verbose) free_verbose(verbose);
    if (trace) free_trace(trace);
    if (row) free(row);
}

int main(int argc, char *argv[])
{
    args_t args;
    args_t *args_p = &args;

    char *error = checks(argc, argv, args_p);
    if (error != NULL)
    {
        printf("%s\n", error);
        return -1;
    }

    cache_t *cache = create_cache(args_p);
    if (!cache)
    {
        printf("An error has ocurred");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        free_memory(cache, NULL, NULL, NULL, NULL, NULL);
        printf("File does not exist");
        return -1;
    }

    stats_t *stats = create_stats();
    if (!stats) {
        free_memory(cache, file, NULL, NULL, NULL, NULL);
        printf("An error has ocurred");
        return -1;
    }
    verbose_t *verbose = create_verbose();
    if (!verbose) {
        free_memory(cache, file, stats, NULL, NULL, NULL);
        printf("An error has ocurred");
        return -1;
    }
    trace_t *trace = create_trace();
    if (!trace)  {
        free_memory(cache, file, stats, verbose, NULL, NULL);
        printf("An error has ocurred");
        return -1;
    }
    size_t size = 40;
    char *row = malloc(size), op;
    unsigned int instruction_pointer, data, bytes_to_op, mem_addr;
    int operation_index = 0;
    while (getline(&row, &size, file) != EOF)
    {
        sscanf(row, "%x: %c %x %d %x", &instruction_pointer, &op, &mem_addr, &bytes_to_op, &data);
        load_trace(trace, args_p, mem_addr);
        cache_operate(cache, trace, stats, verbose, operation_index, op);
        if (is_in_range(args_p, operation_index))
            print_verbose(verbose);
        operation_index++;
    }
    print_stats(stats, args_p);
    free_memory(cache, file, stats, verbose, trace, row);
}