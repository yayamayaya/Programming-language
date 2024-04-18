#ifndef COMPILER
#define COMPILER

#include "body.h"
#include "arithm.h"

enum COMP_ERRS
{
    ASM_F_OPEN_ERR = 1011,
};

int compiler(node_t *root);

#endif