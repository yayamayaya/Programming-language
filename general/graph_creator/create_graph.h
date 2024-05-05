#ifndef GRAPH_CREATE
#define GRAPH_CREATE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../n_tree_func/n_node.h"
#include "../../../common/logging/log.h"
#include "../commands/commands.h"

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

#ifdef SIMPLE_GRAPH

#define GRAPH_NUMBER_NODE   "\tnode_num%p [shape = \"record\", color=\"#478056\", label = \"%.2lf\"];\n", \
                                                                                node, node->data.number
#define GRAPH_OP_NODE       "\tnode_num%p [shape = \"record\", color=\"#632b2b\", label = \"%#04x\"];\n", \
                                                                                node, node->data.command
#define GRAPH_VAR_NODE      "\tnode_num%p [shape = \"record\", color=\"#70578a\", label = \"%s\"];\n", \
                                                                                node, node->data.string
#define GRAPH_CONN_NODE     "\tnode_num%p [shape = \"record\", color=\"#003366\", label = \"LINKER\"];\n", \
                                                                                node 
#define GRAPH_FUNC_NAME_NODE      "\tnode_num%p [shape = \"record\", color=\"#001296\", label = \"%s\"];\n", \
                                                                                node, node->data.string
#define GRAPH_MAIN_NODE     "\tnode_num%p [shape = \"record\", color=\"#0111c0\", label = \"MAIN\"];\n", \
                                                                                node

#else

#define GRAPH_NUMBER_NODE   "\tnode_num%p [shape = \"record\", color=\"#478056\", label = \"{%.2lf | {NUM | %p | %db}}\"];\n", \
                                                                                node, node->data.number, node, node->branch_number
#define GRAPH_OP_NODE       "\tnode_num%p [shape = \"record\", color=\"#632b2b\", label = \"{%#04x | {OP | %p | %db}}\"];\n", \
                                                                                node, node->data.command, node, node->branch_number
#define GRAPH_VAR_NODE      "\tnode_num%p [shape = \"record\", color=\"#70578a\", label = \"{%s | {VAR | %p | %db}}\"];\n", \
                                                                                node, node->data.string, node, node->branch_number
#define GRAPH_CONN_NODE     "\tnode_num%p [shape = \"record\", color=\"#003366\", label = \"{LINKER | {%p | %db}}\"];\n", \
                                                                                node, node, node->branch_number   
#define GRAPH_FUNC_NAME_NODE      "\tnode_num%p [shape = \"record\", color=\"#001296\", label = \"{%s | {FUNC | %p | %db}}\"];\n", \
                                                                                node, node->data.string, node, node->branch_number
#define GRAPH_MAIN_NODE     "\tnode_num%p [shape = \"record\", color=\"#0111c0\", label = \"{MAIN | {%p | %db}}\"];\n", \
                                                                                node, node, node->branch_number
#endif

#define DOT_CALL    "dot graph/graphcode.txt -Tpng -o"

#ifdef GRAPH
#define _CREATE_GRAPH(arg1)\
    create_gparh_code(arg1)
#else
#define _CREATE_GRAPH(arg1)
#endif

enum PNG_NAMES
{
    EXPRESSION = 0,
    STAGE = 1,
};

int create_gparh_code(node_t *node);
void print_png();
void clear_all_png();
int go_through_tree(FILE *gcode, node_t *node);

#endif