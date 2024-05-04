#include "include/bin_2_n.h"

node_t *convert_tree(node_t *node)
{
    if (node->data_type == CONN && !(node->branch_number != 2 || node->branches[R]->data_type != CONN))
    {   
        node_t *child_node = convert_tree(node->branches[R]);
        node->branches[R] = NULL;
        node->branch_number--;
        for (int i = 0; i < child_node->branch_number; i++)
        {
            _ADD_B(node, child_node->branches[i]);
            child_node->branches[i] = NULL;
        }
        kill_tree(child_node);
    }

    for (int i = 0; i < node->branch_number; i++)
        node->branches[i] = convert_tree(node->branches[i]);
    
    return node;
}