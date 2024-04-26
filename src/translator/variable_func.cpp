#include "../include/variable_func.h"

int assign_variable(Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(node->data.command == E);
    assert(vars);
    if (node->branches[R]->data_type != VAR)
    {
        LOG(">>> language parser error occured: the left branch of assign command is NOT an variable%40s\n", "[error]");
        return ASS_L_NODE_ERR;
    }

    LOG("calculating expression:\n");
    int error = expr_in_asm(vars, node->branches[L]->branches[0]);
    if (error)
        return error;

    LOG("> creating an assembly command:\n");

    variable_t *existing_var = find_var(vars, node->branches[R]->data.string);
    if (existing_var)
    {
        LOG("existing variable found, assigning a value to it:\n");
        fprintf(asm_file, "pop [cx+%d]\n", existing_var->rel_address);
        return 0;
    }
    fprintf(asm_file, "pop [cx+%d]\n\n", free_mem_ptr);
    
    create_variable(vars, node->branches[R]);

    return 0;
}

int create_variable(Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(node->data_type == VAR);
    assert(vars);

    LOG("> locating a memory for variable:\n");
    variable_t new_var = {node->data.string, free_mem_ptr};
    vars->stackPush(new_var);
    LOG("> variable was written in the array\n");

    free_mem_ptr++;
    LOG("> current relative free mem pointer: %d\n", free_mem_ptr);
    return 0;
}

variable_t *find_var(Stack <variable_t> *vars, const char *var_name)
{
    assert(vars);
    assert(var_name);

    for (int i = 0; i < global_vars->getStackSize(); i++)
        if (!strcmp((global_vars->getDataOnPos(i)).var, var_name))
        {
            LOG("> global variable %s was found\n", global_vars->getDataOnPos(i).var);
            return global_vars->getDataPtr() + i;
        }

    for (int i = 0; i < vars->getStackSize(); i++)
        if (!strcmp((vars->getDataOnPos(i)).var, var_name))
        {
            LOG("> variable %s was found\n", vars->getDataOnPos(i).var);
            return vars->getDataPtr() + i;
        }
    
    LOG("> variable wasn't found\n");
    return NULL;
}

int push_var_in_asm(Stack <variable_t> *vars, const char *var_name)
{
    assert(vars);
    assert(var_name);

    for (int i = 0; i < global_vars->getStackSize(); i++)
        if (!strcmp((global_vars->getDataOnPos(i)).var, var_name))
        {
            LOG("> global variable %s was found\n", global_vars->getDataOnPos(i).var);
            fprintf(asm_file, "push [%d]\n", (global_vars->getDataOnPos(i)).rel_address);
            return 0;
        }
    
    variable_t *lcl_var = find_var(vars, var_name);
    if (lcl_var)
    {
        LOG("> local variable %s was found on the address %d and will be pushed in stack\n", lcl_var->var, lcl_var->rel_address);
        fprintf(asm_file, "push [cx+%d]\n", lcl_var->rel_address);
        return 0;
    }
    
    LOG("> variable wasn't found\n");
    return VAR_DEF_NOT_F_ERR;
}

void var_dump(Stack <variable_t> *vars)
{
    assert(vars);
    LOG("\n ----------------------VARIABLES DUMP:------------------------\n");

    for (int i = 0; i < vars->getStackSize(); i++)
        LOG("%d) %s, memory location: %d\n", i, vars->getDataOnPos(i).var, vars->getDataOnPos(i).rel_address);
    
    LOG("--------------------------DUMP ENDED----------------------------\n");
}