#ifndef PROG_LANG
#define PROG_LANG

#include "tokenizator.h"
#include "../../../general/graph_creator/create_graph.h"

#ifdef DEBUG
#define TOK_SHIFT()\
    (*pos)++;\
    LOG("> tokens shifted, current token is:\n");\
    token_dump(log_file, tkns + *pos, 1);
#else
#define TOK_SHIFT() (*pos)++
#endif

node_t *create_syntax_tree(token_t *token_arr);    

enum MAIN_ERR
{
    NULL_ROOT_ERR = 404,
};

#endif