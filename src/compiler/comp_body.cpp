#include "../include/body.h"

int make_body_command(Stack <variable_t> *vars, node_t *node);
void free_local_mem(Stack <variable_t> *vars, int var_num);

int compile_body(Stack <variable_t> *vars, node_t *body_root)
{
    assert(asm_file);
    assert(body_root);
    assert(vars);
    if (body_root->data_type != CONN || body_root->data.command != BODY)
    {
        LOG("fatal error occured, body root is not a body node%40s\n", "[error]");
        return BODY_IS_NOT_NODE_ERR;
    }

    int first_lcl_var_pos = free_mem_ptr;
    for (int i = 0; i < body_root->branch_number; i++)
    {
        int error = make_body_command(vars, body_root->branches[i]);
        if (error)
        {
            free_local_mem(vars, vars->getStackSize());
            return error;
        }
    }
    LOG("> body was read successfully\n");
    int last_lcl_var_pos = free_mem_ptr;

    var_dump(vars);
    free_local_mem(vars, last_lcl_var_pos - first_lcl_var_pos);

    return 0;
}

int make_body_command(Stack <variable_t> *vars, node_t *node)
{
    assert(node);
    assert(asm_file);
    assert(vars);

    LOG("> searching for body command:\n");
    int error = 0;
    switch (node->data_type)
    {
    case OP:
        switch (node->data.command)
        {
        case E:
            error = assign_variable(vars, node);
            break;
        
        case IF:
            if (node->branch_number != 2)
            {
                LOG(">>> error occured: if connection node don't have only 2 branches%40s\n", "[error]");
                return IF_N_TWO_BRANCH;
            }

            error = expr_in_asm(vars, node->branches[R]->branches[0]);
            if (error)
                return error;

            fprintf(asm_file, "push 0\nje L%p\n\n", node);

            error = compile_body(vars, node->branches[L]);

            fprintf(asm_file, "\nL%p:\n", node);
            break;

        case WHILE:
            if (node->branch_number != 2)
            {
                LOG(">>> error occured: if connection node don't have only 2 branches%40s\n", "[error]");
                return IF_N_TWO_BRANCH;
            }

            LOG("> translating while\n");
            fprintf(asm_file, "\nS%p:\n", node);
            error = expr_in_asm(vars, node->branches[R]->branches[0]);
            if (error)
                return error;

            fprintf(asm_file, "push 0\nje L%p\n\n", node);

            error = compile_body(vars, node->branches[L]);

            fprintf(asm_file, "jmp S%p\n\n", node);
            fprintf(asm_file, "\nL%p:\n", node);
            break;

        default:
            LOG(">>> undefined compilation operator, yet to define%40s\n", "[error]");
            return FATAL_ERR;
        }
        break;

    case VAR:
        if (find_var(vars, node->data.string))
            LOG("variable already exists, continuing forward\n");
        else
            error = create_variable(vars, node);
        break;

    case CONN:
        switch (node->data.command)
        {
        case BODY:
            error = compile_body(vars, node);
            break;

        case EXPR:
            if (node->branch_number != 1)
            {
                LOG(">>> error occured: expression connection node don't have only 1 branch%40s\n", "[error]");
                return EXPR_CONN_N_ONE_BRANCH;
            }
            node = node->branches[0];

            error = expr_in_asm(vars, node);
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

    return error;
}

void free_local_mem(Stack <variable_t> *vars, int var_num)
{
    assert(vars);
    
    LOG("> freeing %d local variables:\n", var_num);
    for (int i = 0; i < var_num; i++)
    {
        variable_t var_to_del = {0};
        vars->stackPop(&var_to_del);
        fprintf(asm_file, "mov [rbp+%d], 0\n", var_to_del.rel_address);
        free_mem_ptr--;
    }
    
    LOG("> current variables number: %d\n", vars->getStackSize());
    var_dump(vars);

    LOG("> memory was free'd successfully\n");
    return;
}