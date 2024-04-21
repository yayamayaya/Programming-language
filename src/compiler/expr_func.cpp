#include "../include/expr_func.h"

int make_command(unsigned char cmd);

int expr_in_asm(Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(vars);
    
    LOG("node %p was given for calculation\n", node);

    int error = 0;
    switch (node->data_type)
    {
    case NUM:
        LOG("> number %.2lf was found and will be pushed in stack\n", node->data.number);
        fprintf(asm_file, "push %lf\n", node->data.number);
        return 0;

    case OP:
        if (node->branch_number != 2)
        {
            LOG(">>> fatal error, arithmetic OP has more than 2 nodes%40s\n", "[error]");
            return 1;
        }
        
        if (expr_in_asm(vars, node->branches[L]))
            return 1;
        if (expr_in_asm(vars, node->branches[R]))
            return 1;

        if (make_command(node->data.command))
            return 1;
            
        return 0;
    
    case VAR:
        error = push_var_in_asm(vars, node->data.string);
        return 0;
    case FUNC:
        LOG("> function call in expression was found\n");
        error = call_func(vars, node);

        fprintf(asm_file, "push dx\n");
        return error;
    case CONN:
        if (node->data.command == EXPR)
            return expr_in_asm(vars, node->branches[0]);
        
    
    default:
        break;
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