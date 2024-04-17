#ifndef COMPILER
#define COMPILER

#include "n_node.h"
#include "commands.h"
#include "body.h"
#include <math.h>

enum COMP_ERRS
{
    AR_OP_BR_ERR = 1010,
    ASM_F_OPEN_ERR = 1011,
};

int compiler(node_t *root);

#endif