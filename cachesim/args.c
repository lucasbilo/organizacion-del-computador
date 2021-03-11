#include "./args.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Check that a number is a power of two 
    return: 1 if number is power of two
            0 if number is not power of two
*/
int check_exponent(int number)
{
    return (number != 0) && ((number & (number - 1)) == 0);
}

bool check_exponents(args_t *args)
{
    return check_exponent(args->size_cache) && check_exponent(args->associativity_cache) && check_exponent(args->sets_number);
}

bool check_verbose_mode_range(args_t *args)
{
    return 0 <= args->start_range && args->start_range <= args->end_range;
}

char *checks(int argc, char *argv[], args_t *args_p)
{
    if (argc != 5 && argc != 8)
    {
        return "The length of the args should be 5 or 8\n";
    }
    parse_args(args_p, argc, argv);
    int are_exponents_valid = check_exponents(args_p);
    if (!are_exponents_valid)
    {
        return "The size cache, associativity cache and set number should be a power of 2";
    }

    if (argc == 8 && !check_verbose_mode_range(args_p))
    {
        return "N should be greather than 0 and less than M";
    }
    return NULL;
}

void parse_args(args_t *args, int argc, char *argv[])
{
    args->size_cache = atoi(argv[2]);
    args->associativity_cache = atoi(argv[3]);
    args->sets_number = atoi(argv[4]);

    args->is_verbose = false;
    args->block_cache = args->size_cache / (args->sets_number * args->associativity_cache);

    args->set_bits = log2(args->sets_number);
    args->block_offset_bits = log2(args->block_cache);
    args->tag_bits = 32 - args->set_bits - args->block_offset_bits;

    args->block_offset_mask = ~(~0 << args->block_offset_bits);
    args->set_index_mask = ~(~0 << args->set_bits) << args->block_offset_bits;
    args->tag_mask = ~0 & ~args->block_offset_mask & ~args->set_index_mask;

    if (argc != 8)
        return;

    args->is_verbose = true;
    args->start_range = atoi(argv[6]);
    args->end_range = atoi(argv[7]);
    return;
}

bool is_in_range(args_t *args, int operation_index)
{
    return args->is_verbose && (args->start_range <= operation_index && operation_index <= args->end_range);
}