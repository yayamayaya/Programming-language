#include "../include/trans_expr.h"

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
        _PUSH_NUM(node->data.number);
        return 0;

    case OP:
        _CHECK_NODE_NUM(2);
        
        if (expr_in_asm(vars, node->branches[L]))
            return FATAL_ERR;
        if (expr_in_asm(vars, node->branches[R]))
            return FATAL_ERR;

        if (make_command(node->data.command))
            return FATAL_ERR;
            
        return 0;
    
    case VAR:
        if (!find_var(vars, node->data.string))
        {
            LOG("[error]>>> variable was not defined in this scope\n");
            printf(">>> compilation error: variable %s was not defined\n", node->data.string);
            return ERR;
        }

        error = push_var_in_asm(vars, node->data.string);
        return 0;
        
    case FUNC:
        LOG("> function call in expression was found\n");
        error = call_func(vars, node);

        return error;
    case CONN:
        if (node->data.command == EXPR)
            return expr_in_asm(vars, node->branches[0]);
        
    default:
        break;
    }

    LOG(">>> fatal error: unacceptable data type of the node for the expression%40s\n", "[error]");
    return FATAL_ERR;
}

int make_command(unsigned char cmd)
{
    switch (cmd)
    {
    case PLUS:      _TRANSL_OP("add");
    case MINUS:     _TRANSL_OP("sub");
    case STAR:      _TRANSL_OP("mult");
    case SLASH:     _TRANSL_OP("div");
    case POW:       _TRANSL_OP("pow");
    case LOG_E:     _TRANSL_OP("log_e");
    case LOG_NE:    _TRANSL_OP("log_ne");
    case LOG_A:     _TRANSL_OP("log_a");
    case LOG_AE:    _TRANSL_OP("log_ae");
    case LOG_B:     _TRANSL_OP("log_b");
    case LOG_BE:    _TRANSL_OP("log_be");

    default:
        LOG(">>> fatal error occured: couldn't detect arithmetic operation: %d%40s\n", cmd, "[error]");
        return FATAL_ERR;
    }

    return 0;
}