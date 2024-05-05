#include "../include/trans_variable.h"

_INIT_LOG();

int assign_variable(FILE *asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(memory);
    assert(node);
    assert(node->data.command == E);
    assert(vars);

    _CHECK_NODE_NUM(2);
    if (node->branches[R]->data_type != VAR)
    {
        printf(">>> language parser error occured: the right branch of assign command is NOT an variable%40s\n", "[error]");
        return ASS_L_NODE_ERR;
    }

    int error = expr_in_asm(asm_file, memory, vars, node->branches[L]->branches[0]);
    if (error)
        return error;
    
    create_variable(asm_file, vars, node->branches[R]);

    error = pop_var_in_asm(asm_file, memory->global_vars, vars, node->branches[R]->data.string);
    if (error)
        return error;
    
    return 0;
}

int create_variable(FILE *asm_file, Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(node->data_type == VAR);
    assert(vars);

    printf("> creating a variable %s\n", node->data.string);
    variable_t new_var = {node->data.string, vars->getStackSize()};
    vars->stackPush(new_var);

    return 0;
}

variable_t *find_var(Stack <variable_t> *vars, const char *var_name)
{
    assert(vars);
    assert(var_name);

    for (int i = 0; i < vars->getStackSize(); i++)
        if (!strcmp((vars->getDataOnPos(i)).var, var_name))
            return vars->getDataPtr() + i;
    
    return NULL;
}

int push_var_in_asm(FILE *asm_file, Stack <variable_t> *global_vars, Stack <variable_t> *vars, const char *var_name)
{
    assert(vars);
    assert(var_name);

    _OPEN_A_LOG("logs/var_pushed.log");

    variable_t *existing_var = find_var(vars, var_name);
    if (existing_var)
    {
        LOG("> variable found, pushing it\n");
        _PUSH_REL(existing_var->rel_address);
        _CLOSE_LOG();
        return 0;
    }
    existing_var = find_var(global_vars, var_name);
    if (existing_var)
    {
        LOG("> global variable found, assigning a value to it:\n");
        _PUSH_MEM(existing_var->rel_address);
        _CLOSE_LOG();
        return 0;
    }
    
    LOG("[error]>>> variable was not defined in this scope\n");
    printf(">>> compilation error: variable %s was not defined\n", var_name);
    _CLOSE_LOG();
    return VAR_DEF_NOT_F_ERR;
}

int pop_var_in_asm(FILE *asm_file, Stack <variable_t> *global_vars, Stack <variable_t> *vars, const char *var_name)
{
    assert(vars);
    assert(var_name);

    _OPEN_A_LOG("logs/var_popped.log");

    variable_t *existing_var = find_var(vars, var_name);
    if (existing_var)
    {
        LOG("> variable found, pushing it\n");
        _POP_REL(existing_var->rel_address);
        _CLOSE_LOG();
        return 0;
    }
    existing_var = find_var(global_vars, var_name);
    if (existing_var)
    {
        LOG("> global variable found, assigning a value to it:\n");
        _POP_MEM(existing_var->rel_address);
        _CLOSE_LOG();
        return 0;
    }
    
    LOG("[error]>>> variable was not defined in this scope\n");
    printf(">>> compilation error: variable %s was not defined\n", var_name);
    _CLOSE_LOG();
    return VAR_DEF_NOT_F_ERR;
}