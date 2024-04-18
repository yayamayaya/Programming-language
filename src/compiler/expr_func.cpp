#include "../include/expr_func.h"

int make_command(unsigned char cmd);

int expr_in_asm(variables *var_arr, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(var_arr);
    
    LOG("node %p was given for calculation\n", node);

    if (node->data_type == NUM)
    {
        LOG("> number %.2lf was found and will be pushed in stack\n", node->data.number);
        fprintf(asm_file, "push %lf\n", node->data.number);
        return 0;
    }
    if (node->data_type == VAR)
    {
        variable_t *var = find_var(var_arr, node->data.string);
        if (!var)
        {
            LOG(">>> variable wasn't defined in expression%40s\n", "[error]");
            return 1;
        }

        LOG("> variable %s was found on the address %d and will be pushed in stack\n", var->var, var->rel_address);
        fprintf(asm_file, "push [rbp+%d]\n", var->rel_address);
        return 0;
    }
    if (node->data_type == OP)
    {
        if (node->branch_number != 2)
        {
            LOG(">>> fatal error, arithmetic OP has more than 2 nodes%40s\n", "[error]");
            return 1;
        }
        
        if (expr_in_asm(var_arr, node->branches[L]))
            return 1;
        if (expr_in_asm(var_arr, node->branches[R]))
            return 1;

        if (make_command(node->data.command))
            return 1;
            
        return 0;
    }
    

    LOG(">>> fatal error: unacceptable data type of the node for the expression%40s\n", "[error]");
    return 1;
}

int make_command(unsigned char cmd)
{
    switch (cmd)
    {
    case PLUS:
        fprintf(asm_file, "add\n");
        break;
    case MINUS:
        fprintf(asm_file, "sub\n");
        break;
    case STAR:
        fprintf(asm_file, "mult\n");
        break;
    case SLASH:
        fprintf(asm_file, "div\n");
        break;
    case POW:
        fprintf(asm_file, "pow\n");
        break;
    case LOG_E:
        fprintf(asm_file, "log_e\n");
        break;
    case LOG_NE:
        fprintf(asm_file, "log_ne\n");
        break;
    case LOG_A:
        fprintf(asm_file, "log_a\n");
        break;
    case LOG_AE:
        fprintf(asm_file, "log_ae\n");
        break;
    case LOG_B:
        fprintf(asm_file, "log_b\n");
        break;
    case LOG_BE:
        fprintf(asm_file, "log_be\n");
        break;

    default:
        LOG(">>> fatal error occured: couldn't detect arithmetic operation: %d%40s\n", cmd, "[error]");
        return 1;
        break;
    }

    return 0;
}