#include "../include/body.h"

int make_body_command(variables *var_arr, node_t *node);
void free_local_mem(variables *var_arr);

int compile_body(node_t *body_root)
{
    assert(asm_file);
    assert(body_root);
    if (body_root->data_type != CONN || body_root->data.command != BODY)
    {
        LOG("fatal error occured, body root is not a body node%40s\n", "[error]");
        return BODY_IS_NOT_NODE_ERR;
    }

    variables var_arr = {0};
    var_arr.vars = (variable_t *)calloc(10, sizeof(variable_t));
    if (!var_arr.vars)
    {
        LOG(">>> couldn't allocate memory for variable array%40s\n", "[error]");
        return VAR_ARR_MEM_ALC_ERR;
    }

    for (int i = 0; i < body_root->branch_number; i++)
    {
        int error = make_body_command(&var_arr, body_root->branches[i]);
        if (error)
        {
            free_local_mem(&var_arr);
            return error;
        }
    }
    LOG("> body was read successfully\n");
    var_dump(&var_arr);
    free_local_mem(&var_arr);

    return 0;
}

int make_body_command(variables *var_arr, node_t *node)
{
    assert(node);
    assert(asm_file);
    LOG("> searching for body command:\n");

    int error = 0;
    switch (node->data_type)
    {
    case OP:
        switch (node->data.command)
        {
        case E:
            error = assign_variable(var_arr, node);
            if (error)
                return error;
            break;

        default:
            LOG(">>> undefined compilation operator, yet to define%40s\n", "[error]");
            return FATAL_ERR;
        }
        break;

    case VAR:
        error = create_variable(var_arr, node);
        if (error)
            return error;
        break;

    case CONN:
        switch (node->data.command)
        {
        case BODY:
            error = compile_body(node);
            if (error)
                return error;
            break;

        case EXPR:
            if (node->branch_number != 1)
            {
                LOG(">>> error occured: expression connection node don't have only 1 branch%40s\n", "[error]");
                return EXPR_CONN_N_ONE_BRANCH;
            }
            node = node->branches[0];

            error = expr_in_asm(var_arr, node);
            if (error)
                return error;
            break;

        default:
            LOG(">>> undefined compilation connection, yet to define%40s\n", "[error]");
            return FATAL_ERR;
            break;
        }
        break;

    default:
        LOG(">>> fatal error occured: couldn't detect node data type%40s\n", "[error]");
        return FATAL_ERR;
        break;
    }

    return 0;
}

void free_local_mem(variables *var_arr)
{
    assert(var_arr);

    LOG("> freeing all local variables:\n");
    for (int i = 0; i < var_arr->var_num; i++)
    {
        fprintf(asm_file, "mov [rbp+%d], 0\n", var_arr->vars[i].rel_address);
        var_arr->vars[i].var = NULL; 
    }
    
    free(var_arr->vars);
    free_mem_ptr = 0;

    LOG("> memory was free'd successfully\n");
    return;
}