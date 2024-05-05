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
    VAR_DEF_NOT_F,
    MAIN_NOT_FOUND,
};

int compile_func(FILE *asm_file, memory_work *memory, node_t *root);
int call_func(FILE *asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node);
func_t *find_func(Stack <func_t> *funcs, const char *func_name);

#endif