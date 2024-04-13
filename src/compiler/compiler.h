#ifndef COMPILER
#define COMPILER

#include "../n_tree_func/n_node.h"
#include "../include/commands.h"
#include <math.h>

enum COMP_ERRS
{
    AR_OP_BR_ERR = 1010,
};

int compiler(node_t *root);

#endif