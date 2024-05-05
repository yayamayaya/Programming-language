#ifndef STRUCTS
#define STRUCTS

#include <stdio.h>
#include "../../../../stack/src/stack.h"

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
    const char *func;
    int arg_num;
    int mem_size;
} func_t;

typedef struct
{
    int free_mem_ptr;
    Stack <variable_t> *global_vars;
    Stack <func_t> *funcs;
} memory_work;


/*extern int free_mem_ptr;
extern FILE *asm_file;
extern Stack <variable_t> *global_vars;
extern Stack <func_t> *funcs;
extern int return_flag;*/

#endif