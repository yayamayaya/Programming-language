#ifndef VAR_FUNC
#define VAR_FUNC

#include <string.h>
#include "compiler_structs.h"
#include "n_node.h"
//#include "../../../processor/src/processor.h"
#include "commands.h"
#include "expr_func.h"

#define SIZE_OF_RAM 256

int assign_variable(variables *var_arr, node_t *node);
int create_variable(variables *var_arr, node_t *node);
variable_t *find_var(variables *var_arr, const char *var_name);
void clear_all_mem(double *mem_arr);
void var_dump(variables *var_arr);

enum VAR_ERRORS
{
    ASS_L_NODE_ERR = 10110,
};

#endif