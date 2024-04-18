#ifndef ARITHM
#define ARITHM

#include <stdio.h>
#include "../../../common/log/log.h"
#include "n_node.h"
#include "commands.h"
#include <math.h>

enum AR_ERRS
{
    AR_OP_BR_ERR = 1010,
};

int calc_consts(node_t *node);
double make_operation(double first_arg, double second_arg, const unsigned char op);

#endif