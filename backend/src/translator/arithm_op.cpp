#include "../include/arithm.h"

void turn_op_to_num(node_t *node, double number);

_INIT_LOG();

int calc_consts(node_t *node)
{
    assert(node);

    for (int i = 0; i < node->branch_number; i++)
        calc_consts(node->branches[i]);

    if (node->data_type == OP && node->data.command & AR_OP)
    {
        _CHECK_NODE_NUM(2);

        if (node->branches[L]->data_type == NUM && node->branches[R]->data_type == NUM)
        {
            double result = make_operation(node->branches[L]->data.number, node->branches[R]->data.number, node->data.command);

            turn_op_to_num(node, result);
            return 0;
        }
    }
    return 0;
}

void turn_op_to_num(node_t *node, double number)
{
    assert(node);

    kill_tree(node->branches[L], KILL_STRINGS);
    kill_tree(node->branches[R], KILL_STRINGS);
    free(node->branches);
    node->branches = NULL;
    node->branch_number = 0;
    node->data_type = NUM;
    node->data.number = number;

    return;
}

double make_operation(double first_arg, double second_arg, const unsigned char op)
{
    double result = 0;

    switch (op)
    {
    case PLUS:      _ARITHM_OP(+);
    case MINUS:     _ARITHM_OP(-);
    case STAR:      _ARITHM_OP(*);
    case LOG_E:     _ARITHM_OP(==);
    case LOG_NE:    _ARITHM_OP(!=);
    case LOG_A:     _ARITHM_OP(>);
    case LOG_AE:    _ARITHM_OP(>=);
    case LOG_B:     _ARITHM_OP(<);
    case LOG_BE:    _ARITHM_OP(<=);

    case SLASH:     
        if (!second_arg)
        {
            printf("[error]>>> cannot divide into 0\n");
            return 0;
        }
        _ARITHM_OP(/);

    case POW:       result = pow(first_arg, second_arg);    break;

    default:
        printf("[error]>>> fatal error in making an operation\n");
        return 0;
    }
     
    return result;
}