#ifndef PROG_LANG
#define PROG_LANG

#include "../lang_parser/tokenizator.h"
#include "../../graph_creator/create_graph.h"
#include "../compiler/compiler.h"

#ifdef DEBUG
#define TOK_SHIFT()\
    tkns++;\
    LOG("> tokens shifted, current token is:\n");\
    token_dump(logs, tkns, 1);
#else
#define TOK_SHIFT() tkns++
#endif

#define _ADD_B(arg1, arg2)\
    do {if (add_branch(arg1, arg2) == NODE_MEM_ALC_ERR)\
        return NULL;} while(0)

node_t *create_syntax_tree(token_t *token_arr);    

enum MAIN_ERR
{
    NULL_ROOT_ERR = 404,
};

#endif