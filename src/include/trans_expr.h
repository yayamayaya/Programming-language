#ifndef EXPR_FUNC
#define EXPR_FUNC

#include "trans_variable.h"
#include "trans_func.h"
#include "global_defines.h"

int expr_in_asm(Stack <variable_t> *vars, node_t *node);

#define _TRANSL_OP(op) fprintf(asm_file, "%s\n", op); break

#endif