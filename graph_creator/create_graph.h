#ifndef GRAPH_CREATE
#define GRAPH_CREATE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/include/n_node.h"
#include "../../common/log/log.h"

#define GPRAPH_CODE_START   "digraph G\n\
{\n\
    graph [dpi = 1000];\n\
    bgcolor = \"#9F8C76\"\n\
\n\
    node[shape = \"box\", style = \"rounded, filled\",\n\
    height = 0.5, width = 1,\n\
    fillcolor = \"#F5F5DC\",\n\
    fontsize = 17,\n\
    fontname = \"Courier New\",\n\
    fontcolor = \"#1a1718\",\n\
    penwidth = 2, color = \"#1a1718\"\n\
    ]\n\n\
"
#define GRAPH_NUMBER_NODE   "\tnode_num%p [shape = \"record\", color=\"#478056\", label = \"{%.2lf | {NUM | %p | %db}}\"];\n", \
                                                                                node, node->data.number, node, node->branch_number
#define GRAPH_OP_NODE       "\tnode_num%p [shape = \"record\", color=\"#632b2b\", label = \"{%#04x | {OP | %p | %db}}\"];\n", \
                                                                                node, node->data.command, node, node->branch_number
#define GRAPH_VAR_NODE      "\tnode_num%p [shape = \"record\", color=\"#70578a\", label = \"{%s | {VAR | %p | %db}}\"];\n", \
                                                                                node, node->data.string, node, node->branch_number
#define GRAPH_CONN_NODE     "\tnode_num%p [shape = \"record\", color=\"#003366\", label = \"{%#04x | {CONN | %p | %db}}\"];\n", \
                                                                                node, node->data.command, node, node->branch_number   
#define GRAPH_FUNC_NAME_NODE      "\tnode_num%p [shape = \"record\", color=\"#001296\", label = \"{%s | {FUNC | %p | %db}}\"];\n", \
                                                                                node, node->data.string, node, node->branch_number

#define DOT_CALL    "dot graph_creator/graphcode.txt -Tpng -o"

#ifdef GRAPH
#define _CREATE_GRAPH(arg1, arg2)\
    create_gparh_code(arg1, arg2)
#else
#define _CREATE_GRAPH(arg1, arg2)
#endif

enum PNG_NAMES
{
    EXPRESSION = 0,
    STAGE = 1,
};

int create_gparh_code(node_t *node, const int PNG_TYPE);
void print_png(const int PNG_TYPE);
void clear_all_png();
int go_through_tree(FILE *gcode, node_t *node);

#endif