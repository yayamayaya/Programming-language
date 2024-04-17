#ifndef BODY_F
#define BODY_F

#include "variable_func.h"
#include "expr_func.h"

enum BODY_ERRS
{
    VAR_ARR_MEM_ALC_ERR = 1010,
    EXPR_CONN_N_ONE_BRANCH,
    FATAL_ERR = 4040,
    BODY_IS_NOT_NODE_ERR = 1015,
};

int compile_body(node_t *body_root);

#endif