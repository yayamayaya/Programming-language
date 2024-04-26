#ifndef VAR_FUNC
#define VAR_FUNC

#include <string.h>
#include "translator_structs.h"
#include "n_node.h"
#include "commands.h"
#include "trans_expr.h"

#define SIZE_OF_RAM 256

int assign_variable(Stack <variable_t> *vars, node_t *node);
int create_variable(Stack <variable_t> *vars, node_t *node);
variable_t *find_var(Stack <variable_t> *vars, const char *var_name);
int push_var_in_asm(Stack <variable_t> *vars, const char *var_name);
void var_dump(Stack <variable_t> *vars);

enum VAR_ERRORS
{
    ASS_L_NODE_ERR = 10110,
};

#endif