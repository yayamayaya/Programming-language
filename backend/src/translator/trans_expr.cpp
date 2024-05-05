#include "../include/trans_expr.h"

int make_command(FILE *asm_file, unsigned char cmd);

_INIT_LOG();

int expr_in_asm(FILE *asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(vars);
    assert(memory);
    
    LOG("node %p was given for calculation\n", node);
    int error = 0;

    switch (node->data_type)
    {
    case NUM:
        LOG("> number %.2lf was found and will be pushed in stack\n", node->data.number);
        _PUSH_NUM(node->data.number);
        break;

    case OP:
        _CHECK_NODE_NUM(2);
        
        error = expr_in_asm(asm_file, memory, vars, node->branches[L]);
        if (error)
            break;
        error = expr_in_asm(asm_file, memory, vars, node->branches[R]);
        if (error)
            break;

        error = make_command(asm_file, node->data.command);

        break;

    case VAR:
        error = push_var_in_asm(asm_file, memory->global_vars, vars, node->data.string);

        break;
        
    case FUNC:
        LOG("> function call in expression was found\n");
        error = call_func(asm_file, memory, vars, node);

        break;
        
    case LINKER:
        error = expr_in_asm(asm_file, memory, vars, node->branches[0]);

        break;
        
    default:
        LOG(">>> fatal error: unacceptable data type of the node for the expression%40s\n", "[error]");
        _CLOSE_LOG();
        return FATAL_ERR;

    }

    _CLOSE_LOG();
    return error;
}

int make_command(FILE *asm_file, unsigned char cmd)
{
    assert(asm_file);

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

    default:        return FATAL_ERR;
    }

    return 0;
}