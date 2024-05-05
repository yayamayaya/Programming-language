#ifndef ARITHM
#define ARITHM

#include <stdio.h>
#include "../../../../common/logging/log.h"
#include "../../../general/n_tree_func/n_node.h"
#include "../../../general/commands/commands.h"
#include <math.h>
#include "global_defines.h"

enum AR_ERRS
{
    AR_OP_BR_ERR = 1010,
};

#define _ARITHM_OP(op) result = first_arg op second_arg; break

int calc_consts(node_t *node);
double make_operation(double first_arg, double second_arg, const unsigned char op);

#endif