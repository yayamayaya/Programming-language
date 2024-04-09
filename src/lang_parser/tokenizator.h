#ifndef TOKENIZATOR
#define TOKENIZATOR

#include <stdio.h>
#include "../../../common/log/log.h"
#include "../../../common/file_reading/file_reader.h"
#include "../include/commands.h"
#include "locale.h"
#include "../n_tree_func/n_node.h"
#include "ctype.h"
#include "string.h"

#ifdef DEBUG
#define PRINT_TOKENS() token_dump(logs, *tokens, token_number)
#else
#define PRINT_TOKENS()
#endif

typedef struct
{
    data_t data;
    unsigned char data_type;
} token_t;

enum TOK_ERRS
{
    TOK_MEM_ALC_ERR = 111,
    TOK_NULL_STR    
};

int tokenize(token_t **tokens, const char *file_loc);

#endif
