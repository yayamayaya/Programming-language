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

enum TOK_DATA_TYPES
{
    NUMBER  = 1,
    COMMAND = 2,
    YET_TO_DET  = 15,
    $           = 20,
};

enum TOK_ERRS
{
    TOK_MEM_ALC_ERR = 111,
    TOK_NULL_STR    
};

int tokenize(token_t **tokens, const char *file_loc);
void token_dump(FILE* out, token_t *tokens, int token_num);
void free_tok_strings(token_t *tokens);

#endif
