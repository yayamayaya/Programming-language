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

#define L 0
#define R 1

typedef union
{
    unsigned char command;
    int address;
    double number;
    const char *string;
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
    OP          = 2,
    NUM         = 1,
    VAR         = 3,
    CONN        = 10,

    //GRPS
    VAR_OP      = 0x40,
};

enum NODE_ERRORS
{
    NODE_MEM_ALC_ERR = 11,
};

node_t *create_node(const int data, const unsigned char data_type, const int branch_number, ...);
node_t *create_node(double data, const unsigned char data_type, const int branch_number, ...);
node_t *create_node(const char *data, const unsigned char data_type, const int branch_number, ...);
node_t *create_node(unsigned char data, const unsigned char data_type, const int branch_number, ...);
int add_branch(node_t *parent_node, node_t *branch);
void print_node(FILE *out, node_t *node);
void kill_tree(node_t *node);

#endif