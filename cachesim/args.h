#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>

typedef struct {
    int size_cache;
    int associativity_cache;
    int sets_number;
    bool is_verbose;
    int start_range;
    int end_range;
    int block_cache;
    int block_offset_mask;
    int set_index_mask;
    int tag_mask;
    unsigned int set_bits;
    unsigned int block_offset_bits;
    unsigned int tag_bits;
} args_t;

int check_exponent(int number);
bool check_exponents(args_t *args);
bool check_verbose_mode_range(args_t *args);
char *checks(int argc, char *argv[], args_t *args_p);
void parse_args(args_t *args, int argc, char *argv[]);
bool is_in_range(args_t *args, int operation_index);
#endif // ARGS_H