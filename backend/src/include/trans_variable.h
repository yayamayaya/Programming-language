#ifndef VAR_FUNC
#define VAR_FUNC

#include <string.h>
#include "translator_structs.h"
#include "../../../general/n_tree_func/n_node.h"
#include "../../../general/commands/commands.h"
#include "trans_expr.h"

#define SIZE_OF_RAM 256

int assign_variable(FILE *asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node);
void create_variable(FILE *asm_file, Stack <variable_t> *vars, node_t *node);
variable_t *find_var(Stack <variable_t> *vars, const char *var_name);
int push_var_in_asm(FILE *asm_file, Stack <variable_t> *global_vars, Stack <variable_t> *vars, const char *var_name);
int pop_var_in_asm(FILE *asm_file, Stack <variable_t> *global_vars, Stack <variable_t> *vars, const char *var_name);

enum VAR_ERRORS
{
    ASS_L_NODE_ERR = 10110,
};

#endif