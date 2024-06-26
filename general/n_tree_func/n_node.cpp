#include "n_node.h"

node_t *create_node(const int data, const unsigned char data_type, const int branch_number, ...)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (!node)
        return NULL;
        
    node_t **nodes_arr  = (node_t **)calloc(branch_number, sizeof(node_t *));
    if (!nodes_arr)
    {
        free(node);
        return NULL;
    }

    node->data.address  = data;
    node->data_type     = data_type;
    
    va_list nodes       = {};
    va_start(nodes, branch_number);
    for (int i = 0; i < branch_number; i++)
        nodes_arr[i] = va_arg(nodes, node_t*);
    
    va_end(nodes);

    node->branch_number = branch_number;
    node->branches      = nodes_arr;
    return node;
}

node_t *create_node(double data, const unsigned char data_type, const int branch_number, ...)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (!node)
        return NULL;

    node_t **nodes_arr  = (node_t **)calloc(branch_number, sizeof(node_t *));
    if (!nodes_arr)
    {
        free(node);
        return NULL;
    }

    node->data.number   = data;
    node->data_type     = data_type;
    
    va_list nodes       = {};
    va_start(nodes, branch_number);
    for (int i = 0; i < branch_number; i++)
        nodes_arr[i] = va_arg(nodes, node_t*);
    
    va_end(nodes);

    node->branch_number = branch_number;
    node->branches      = nodes_arr;
    return node;
}

node_t *create_node(const char *data, const unsigned char data_type, const int branch_number, ...)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (!node)
        return NULL;
    
    node_t **nodes_arr  = (node_t **)calloc(branch_number, sizeof(node_t *));
    if (!nodes_arr)
    {
        free(node);
        return NULL;
    }

    node->data.string   = data;
    node->data_type     = data_type;
    
    va_list nodes       = {};
    va_start(nodes, branch_number);
    for (int i = 0; i < branch_number; i++)
        nodes_arr[i] = va_arg(nodes, node_t*);
    
    va_end(nodes);

    node->branch_number = branch_number;
    node->branches      = nodes_arr;
    return node;
}

node_t *create_node(unsigned char data, const unsigned char data_type, const int branch_number, ...)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (!node)
        return NULL;
    
    node_t **nodes_arr  = (node_t **)calloc(branch_number, sizeof(node_t *));
    if (!nodes_arr)
    {
        free(node);
        return NULL;
    }

    node->data.command  = data;
    node->data_type     = data_type;
    
    va_list nodes       = {};
    va_start(nodes, branch_number);
    for (int i = 0; i < branch_number; i++)
        nodes_arr[i] = va_arg(nodes, node_t*);
    
    va_end(nodes);

    node->branch_number = branch_number;
    node->branches      = nodes_arr;
    return node;
}

node_t *create_node(data_t data, const unsigned char data_type, const int branch_number, ...)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (!node)
        return NULL;
    
    node_t **nodes_arr  = (node_t **)calloc(branch_number, sizeof(node_t *));
    if (!nodes_arr)
    {
        free(node);
        return NULL;
    }

    node->data          = data;
    node->data_type     = data_type;
    
    va_list nodes       = {};
    va_start(nodes, branch_number);
    for (int i = 0; i < branch_number; i++)
        nodes_arr[i] = va_arg(nodes, node_t*);
    
    va_end(nodes);

    node->branch_number = branch_number;
    node->branches      = nodes_arr;
    return node;
}

int add_branch(node_t *parent_node, node_t *branch)
{
    assert(parent_node);

    if (!branch)
        return 1;

    parent_node->branch_number++;
    node_t **branch_holder = (node_t **)realloc(parent_node->branches, parent_node->branch_number * sizeof(node_t *));
    if (!branch_holder)
        return NODE_MEM_ALC_ERR;

    parent_node->branches = branch_holder;
    parent_node->branches[parent_node->branch_number - 1] = branch;

    return 0;
}

void print_node(FILE *out, node_t *node)
{
    assert(node);
    assert(out);
    setbuf(out, NULL);

    fprintf(out, "---------node number: %p---------\n", node);
    fprintf(out, "> data:               %d\n", node->data.command);
    fprintf(out, "> data type:          %d\n", node->data_type);
    fprintf(out, "> number of branches: %d\n", node->branch_number);
    for (int i = 0; i < node->branch_number; i++)
        fprintf(out, "> node %d:            %p\n", i + 1, node->branches[i]);
    fprintf(out, "---------------------------------\n");
    
    return;
}

void kill_tree(node_t *node, int mode)
{
    if (!node)
        return;

    for (int i = 0; i < node->branch_number; i++)
        if (node->branches[i])
            kill_tree(node->branches[i], mode);

    if (mode)
        if (node->data_type == VAR || node->data_type == FUNC || node->data_type == MAIN)
            free((void *)node->data.string);
    
    free(node->branches);
    free(node);
}