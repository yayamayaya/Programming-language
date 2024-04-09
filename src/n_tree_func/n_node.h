#ifndef NODE_FUNC
#define NODE_FUNC

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "../../../common/log/log.h"

#ifdef DEBUG
#define PRINT_NODE() print_node(logs, node)
#else
#define PRINT_NODE()
#endif

typedef union
{
    unsigned char command;
    double number;
    char *string;
} data_t;


typedef struct node_t
{
    data_t data;
    unsigned char data_type;

    node_t **branches;
    int branch_number;
} node_t;

enum DATA_TYPES
{
    NUMBER  = 1,
    COMMAND = 2,
    STRING  = 3,
    YET_TO_DET  = 15,
    
};

enum NODE_ERRORS
{
    NODE_MEM_ALC_ERR = 11,
};

node_t *create_node(data_t data, const unsigned char data_type, const int branch_number, ...);
void print_node(FILE *out, node_t *node);
void kill_tree(node_t *node);

#endif