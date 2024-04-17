#include "../include/variable_func.h"

int assign_variable(variables *var_arr, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(var_arr);
    assert(node->data.command == E);
    if (node->branches[L]->data_type != VAR)
    {
        LOG(">>> language parser error occured: the left branch of assign command is NOT an variable%40s\n", "[error]");
        return ASS_L_NODE_ERR;
    }

    LOG("calculating expression:\n");
    int error = expr_in_asm(var_arr, node->branches[R]->branches[0]);
    if (error)
        return error;
    


    LOG("> creating an assembly command:\n");
    fprintf(asm_file, "pop [rbp+%d]\n\n", free_mem_ptr);
    create_variable(var_arr, node->branches[L]);

    return 0;
}

int create_variable(variables *var_arr, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(var_arr);
    assert(node->data_type == VAR);

    LOG("> locating a memory for variable:\n");
    var_arr->vars[var_arr->var_num].var         = node->data.string;
    var_arr->vars[var_arr->var_num].rel_address = free_mem_ptr;
    LOG("> variable was written in the array\n");

    var_arr->var_num++;
    free_mem_ptr++;
    LOG("> current relative free mem pointer: %d\n", free_mem_ptr);
    return 0;
}

variable_t *find_var(variables *var_arr, const char *var_name)
{
    for (int i = 0; i < var_arr->var_num; i++)
        if (!strcmp(var_arr->vars[i].var, var_name))
        {
            LOG("> variable %s was found\n", var_arr->vars[i].var);
            return var_arr->vars + i;
        }
    
    LOG("> variable wasn't found\n");
    printf(">>> Compilation error:\n variable %s was not defined in this scope\n", var_name);

    return NULL;
}

void var_dump(variables *var_arr)
{
    assert(var_arr);
    LOG("\n ----------------------VARIABLES DUMP:------------------------\n");

    for (int i = 0; i < var_arr->var_num; i++)
        LOG("%d) %s, memory location: %d\n", i, var_arr->vars[i].var, var_arr->vars[i].rel_address);
    
    LOG("--------------------------DUMP ENDED----------------------------\n");
}

void clear_all_mem(double *mem_arr)
{
    for (int i = 0; i < SIZE_OF_RAM; i++)
        mem_arr[i] = 0;
    LOG("> all data in the memory was cleared\n");
    free_mem_ptr = 0;
    return;
}