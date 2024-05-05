#ifndef COMPILER
#define COMPILER

#include "body.h"
#include "arithm.h"

enum COMP_ERRS
{
    ASM_F_OPEN_ERR = 1011,
    MEM_NUM_ARR_ALC_ERR,
};

int translator(node_t *root);

#endif