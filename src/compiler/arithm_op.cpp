#include "../include/arithm.h"

int calc_consts(node_t *node)
{
    static node_t *root = node;
    assert(root);

    for (int i = 0; i < node->branch_number; i++)
        calc_consts(node->branches[i]);

    if (node->data_type == OP && node->data.command & 0x20)
    {
        if (node->branch_number != 2)
        {
            LOG(">>> arithmetic operator have %d branches, instead of 2%40s\n", node->branch_number, "[error]");
            return AR_OP_BR_ERR;
        }

        if (node->branches[L]->data_type == NUM && node->branches[R]->data_type == NUM)
        {
            double result = make_operation(node->branches[L]->data.number, node->branches[R]->data.number, node->data.command);

            kill_tree(node->branches[L]);
            kill_tree(node->branches[R]);
            free(node->branches);
            node->branches = NULL;
            node->branch_number = 0;
            node->data_type = NUM;
            node->data.number = result;

            LOG("> operation was done, operation node was translated to number node\n");
            return 0;
        }
    }
    
    LOG("> this branch don't have any arithm. operations\n");
    return 0;
}

double make_operation(double first_arg, double second_arg, const unsigned char op)
{
    LOG("> making an operation: %#04x beetwen %.2lf and %.2lf\n", op, first_arg, second_arg);
    double result = 0;

    switch (op)
    {
    case PLUS:      result = first_arg + second_arg;        break;
    case MINUS:     result = first_arg - second_arg;        break;
    case STAR:      result = first_arg * second_arg;        break;
    case SLASH:     result = first_arg / second_arg;        break;
    case POW:       result = pow(first_arg, second_arg);    break;
    case LOG_E:     result = first_arg == second_arg;       break;
    case LOG_NE:    result = first_arg != second_arg;       break;
    case LOG_A:     result = first_arg > second_arg;        break;
    case LOG_AE:    result = first_arg >= second_arg;       break;
    case LOG_B:     result = first_arg < second_arg;        break;
    case LOG_BE:    result = first_arg <= second_arg;       break;

    default:
        LOG(">>> fatal error in making an operation%40s\n", "[error]");
        return 0;
    }

    LOG("> result of calc is: %lf\n", result);     
    return result;
}