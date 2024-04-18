#ifndef COMPILER
#define COMPILER

#include "body.h"
#include "arithm.h"

enum COMP_ERRS
{
    ASM_F_OPEN_ERR = 1011,
    MEM_NUM_ARR_ALC_ERR,
};

int compiler(node_t *root);

/*typedef struct
{
    variables **var_arrs;
    int arrays_number;
} var_arrays;*/

#endif