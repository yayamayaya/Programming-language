#ifndef FUNC_COMP
#define FUNC_COMP

#include <ctype.h>
#include "body.h"

enum FUNC_COMP_ERRS
{
    ROOT_NOT_PROG_ERR = 1818,
    OP_NOT_ASS_ERR,
    RET_NOT_FOUND_ERR,
    FUNC_FAT_ERR,
    FUNC_DEF_NOT_F_ERR,
    ARG_NUM_NOT_MATCH_ERR,
    VAR_DEF_NOT_F_ERR,
    MAIN_NOT_FOUND,
};

func_t *find_func(const char *func_name);
int compile_func(node_t *root);
int call_func(Stack <variable_t> *vars, node_t *node);

#endif