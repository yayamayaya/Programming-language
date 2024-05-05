#ifndef BODY_F
#define BODY_F

#include "trans_variable.h"
#include "trans_expr.h"

enum BODY_ERRS
{
    VAR_ARR_MEM_ALC_ERR = 1010,
    EXPR_CONN_N_ONE_BRANCH,
    IF_N_TWO_BRANCH,
    FATAL_ERR = 4040,
    BODY_IS_NOT_NODE_ERR = 1015,
};

int translate_body(FILE* asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *body_root);

#endif