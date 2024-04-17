#ifndef STRUCTS
#define STRUCTS

#include <stdio.h>

typedef struct
{
    int bp;
    int frame_size;
} stk_frame;

typedef struct
{
    const char *var;
    int rel_address;
} variable_t;

typedef struct
{
    variable_t *vars;
    int var_num;
} variables;

extern int free_mem_ptr;
extern FILE *asm_file;
//extern variables *var_arr;

#endif