#include "./stats.h"

stats_t *create_stats()
{
    stats_write_t *stats_write = (stats_write_t *)calloc(1, sizeof(stats_write_t));
    if (!stats_write)
    {
        return NULL;
    }
    stats_read_t *stats_read = (stats_read_t *)calloc(1, sizeof(stats_read_t));
    if (!stats_read)
    {
        free(stats_write);
        return NULL;
    }

    stats_t *stats = (stats_t *)calloc(1, sizeof(stats_t));
    if (!stats)
    {
        free(stats_read);
        free(stats_write);
        return NULL;
    }

    stats->stats_read = stats_read;
    stats->stats_write = stats_write;
    return stats;
}

void print_stats(stats_t *stats, args_t *args)
{
    stats_read_t *stats_read = stats->stats_read;
    stats_write_t *stats_write = stats->stats_write;
    int stats_read_loads = stats_read->loads;
    int stats_write_stores = stats_write->stores;
    int total_loads_stores = stats_read_loads + stats_write_stores;

    int stats_read_rmiss = stats_read->rmiss;
    int stats_write_wmiss = stats_write->wmiss;
    int total_missess = stats_read->rmiss + stats_write->wmiss;

    printf("%d-way, %d sets, size = %dKB\n", args->associativity_cache, args->sets_number, args->size_cache / 1024);
    printf("loads %d stores %d total %d\n", stats_read_loads, stats_write_stores, total_loads_stores);
    printf("rmiss %d wmiss %d total %d\n", stats_read_rmiss, stats_write_wmiss, total_missess);
    printf("dirty rmiss %d dirty wmiss %d\n", stats_read->dirty_rmiss, stats_write->dirty_wmiss);
    printf("bytes read %d bytes written %d\n", stats_read->bytes_read, stats_write->bytes_written);
    printf("read time %d write time %d\n", stats_read->read_time, stats_write->write_time);
    printf("miss rate %f\n", ((float)total_missess / (float)total_loads_stores));
}

void free_stats(stats_t *stats)
{
    free(stats->stats_read);
    free(stats->stats_write);
    free(stats);
}

verbose_t *create_verbose()
{
    verbose_t *verbose = (verbose_t *)calloc(1, sizeof(verbose_t));
    return !verbose ? NULL : verbose;
}

void print_verbose(verbose_t *verbose)
{
    char *formatter = verbose->line_tag == -1 ? "%d %s %x %x %d %d %d %d %d \n" : "%d %s %x %x %d %x %d %d %d \n";
    printf(formatter, verbose->operation_index, verbose->case_identifier, verbose->cache_index,
           verbose->cache_tag, verbose->cache_line, verbose->line_tag,
           verbose->valid_bit, verbose->dirty_bit, verbose->last_used);
}

void free_verbose(verbose_t *verbose)
{
    free(verbose);
}