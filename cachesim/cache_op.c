#include "./cache_op.h"

static const int PENALTY = 100;

struct cache
{
    int block_cache;
    set_t *sets;
    int sets_number;
    int associativity;
};

struct set
{
    line_t *lines;
};

struct line
{
    int tag;
    bool valid_bit;
    bool dirty_bit;
    int index;
    int last_used;
};

struct trace
{
    int tag;
    int index;
    int offset;
};

// AUXILIARY FUNCTIONS
bool search_in_the_cache(cache_t *cache, set_t set, int tag, int operation_index, verbose_t *verbose, char op)
{
    for (int i = 0; i < cache->associativity; i++)
    {
        if (set.lines[i].valid_bit && set.lines[i].tag == tag)
        {
            verbose->dirty_bit = set.lines[i].dirty_bit;
            verbose->last_used = set.lines[i].last_used;
            verbose->case_identifier = "1";
            verbose->cache_line = i;
            verbose->line_tag = set.lines[i].tag;
            verbose->valid_bit = true;
            set.lines[i].last_used = operation_index;
            if (op == 'W')
                set.lines[i].dirty_bit = true;
            return true; //is hit
        }
    }
    return false;
}

int get_free_line_index(set_t set, int number_of_lines)
{
    for (int i = 0; i < number_of_lines; i++)
    {
        if (set.lines[i].tag == 0)
            return i;
    }
    return -1;
}

line_t *find_line_with_less_time_access(set_t set, int number_of_lines, verbose_t *verbose)
{
    int time = set.lines[0].last_used;
    int index = 0;
    for (int i = 1; i < number_of_lines; i++)
    {
        if (set.lines[i].valid_bit == 0 || time > set.lines[i].last_used)
        {
            time = set.lines[i].last_used;
            index = i;
            verbose->cache_line = i;
        }
    }
    return &set.lines[index];
}

void vacate_line(line_t *line_to_vacate, trace_t *trace, verbose_t *verbose, int operation_index)
{

    if (line_to_vacate->dirty_bit)
    { //dirty cache miss
        verbose->case_identifier = "2b";
    }
    else
    { // clean cache miss
        verbose->case_identifier = "2a";
    }
    verbose->last_used = line_to_vacate->last_used;
    verbose->dirty_bit = line_to_vacate->dirty_bit;
    line_to_vacate->last_used = operation_index;
    line_to_vacate->valid_bit = true;
    line_to_vacate->index = trace->index;
    line_to_vacate->tag = trace->tag;
    verbose->line_tag = line_to_vacate->tag;
    verbose->valid_bit = line_to_vacate->valid_bit;
}

void vacate_free_line(set_t set, int index, int operation_index, trace_t *trace, verbose_t *verbose)
{
    set.lines[index].last_used = operation_index;
    set.lines[index].valid_bit = true;
    set.lines[index].dirty_bit = false;
    set.lines[index].index = trace->index;
    set.lines[index].tag = trace->tag;
    verbose->case_identifier = "2a";
    verbose->cache_line = index;
    verbose->line_tag = -1;
    verbose->valid_bit = false;
    verbose->dirty_bit = false;
    verbose->last_used = 0;
}

// MAIN FUNCTIONS

cache_t *create_cache(args_t *args)
{
    cache_t *cache = (cache_t *)calloc(1, sizeof(cache_t));
    cache->block_cache = args->block_cache;
    if (!cache)
        return NULL;

    set_t *sets = (set_t *)calloc(args->sets_number, sizeof(set_t));
    if (!sets)
    {
        free(cache);
        return NULL;
    }

    bool is_success_create = true;
    int last_created_line = 0;
    for (int i = 0; i < args->sets_number; i++)
    {
        line_t *lines = (line_t *)calloc(args->associativity_cache, sizeof(line_t));
        if (!lines)
        {
            last_created_line = i;
            is_success_create = false;
            break;
        }
        sets[i].lines = lines;
    }

    if (!is_success_create)
    {
        for (int i = 0; i < last_created_line; i++)
        {
            free(sets[i].lines);
        }
        free(sets);
        free(cache);
        return NULL;
    }

    cache->sets = sets;
    cache->sets_number = args->sets_number;
    cache->associativity = args->associativity_cache;

    return cache;
}

void free_cache(cache_t *cache)
{
    for (int i = 0; i < cache->sets_number; i++)
    {
        free(cache->sets[i].lines);
    }
    free(cache->sets);
    free(cache);
}

trace_t *create_trace()
{
    trace_t *trace = (trace_t *)calloc(1, sizeof(trace_t));
    return !trace ? NULL : trace;
}

void load_trace(trace_t *trace, args_t *args, unsigned int mem_addr)
{
    trace->offset = mem_addr & args->block_offset_mask;
    trace->index = (mem_addr & args->set_index_mask) >> args->block_offset_bits;
    trace->tag = (mem_addr & args->tag_mask) >> (args->block_offset_bits + args->set_bits);
}

void free_trace(trace_t *trace)
{
    free(trace);
}

void cache_read(cache_t *cache, trace_t *trace, stats_t *stats, int operation_index, verbose_t *verbose)
{
    stats->stats_read->loads += 1;
    set_t set = cache->sets[trace->index];
    verbose->cache_index = trace->index;
    if (search_in_the_cache(cache, set, trace->tag, operation_index, verbose, 'R'))
    {
        stats->stats_read->read_time += 1;
        return;
    }

    stats->stats_read->rmiss += 1;
    stats->stats_read->bytes_read += cache->block_cache;

    int free_line_index = get_free_line_index(set, cache->associativity);
    if (free_line_index == -1) // set is full
    {
        line_t *line_to_vacate = find_line_with_less_time_access(set, cache->associativity, verbose); // LRU
        vacate_line(line_to_vacate, trace, verbose, operation_index);
        if (line_to_vacate->dirty_bit)
        { //dirty cache miss
            stats->stats_read->read_time += (1 + (2 * PENALTY));
            stats->stats_read->dirty_rmiss += 1;
            stats->stats_write->bytes_written += cache->block_cache;
        }
        else
        { //clean cache miss
            verbose->last_used = 0;
            stats->stats_read->read_time += (1 + PENALTY);
        }
        line_to_vacate->dirty_bit = false;
    }
    else
    {
        vacate_free_line(set, free_line_index, operation_index, trace, verbose);
        stats->stats_read->read_time += (1 + PENALTY);
        set.lines[free_line_index].dirty_bit = false;
    }
}

void cache_write(cache_t *cache, trace_t *trace, stats_t *stats, int operation_index, verbose_t *verbose)
{
    stats->stats_write->stores += 1;
    set_t set = cache->sets[trace->index];
    verbose->cache_index = trace->index;
    if (search_in_the_cache(cache, set, trace->tag, operation_index, verbose, 'W'))
    {
        stats->stats_write->write_time += 1;
        return;
    }

    stats->stats_write->wmiss += 1;
    stats->stats_read->bytes_read += cache->block_cache;
    int free_line_index = get_free_line_index(set, cache->associativity);
    if (free_line_index == -1) // set is full
    {
        line_t *line_to_vacate = find_line_with_less_time_access(set, cache->associativity, verbose); // LRU
        vacate_line(line_to_vacate, trace, verbose, operation_index);
        if (line_to_vacate->dirty_bit)
        { //dirty cache miss
            stats->stats_write->bytes_written += cache->block_cache;
            stats->stats_write->write_time += (1 + (2 * PENALTY));
            stats->stats_write->dirty_wmiss += 1;
        }
        else
        { //clean cache miss
            stats->stats_write->write_time += (1 + PENALTY);
        }
        line_to_vacate->dirty_bit = true;
        verbose->last_used = operation_index;
    }
    else
    { // clean cache miss
        vacate_free_line(set, free_line_index, operation_index, trace, verbose);
        stats->stats_write->write_time += (1 + PENALTY);
        set.lines[free_line_index].dirty_bit = true;
    }
}

void cache_operate(cache_t *cache, trace_t *trace, stats_t *stats, verbose_t *verbose, int operation_index, char op)
{
    if (op == 'R')
        cache_read(cache, trace, stats, operation_index, verbose);
    else
        cache_write(cache, trace, stats, operation_index, verbose);
    verbose->operation_index = operation_index;
    verbose->cache_index = trace->index;
    verbose->cache_tag = trace->tag;
}