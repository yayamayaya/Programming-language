#include "tokenizator.h"

#define INITIAL_TOKEN_ARR_SIZE 100

token_t *token_realloc(token_t *token_arr, int arr_size);
token_t detect_token(char *buff, int *pos);
void token_dump(FILE *out, token_t *tokens, int token_num);

int tokenize(token_t **tokens, const char *file_loc)
{
    //setlocale(LC_ALL, "ru_RU");

    char *buff = NULL;
    int file_size = 0;
    int error = read_file(&buff, &file_size, file_loc);
    if (error)
        return error;
    
    int token_arr_size = INITIAL_TOKEN_ARR_SIZE;
    token_t *token_arr = (token_t *)calloc(token_arr_size, sizeof(token_t));
    if (!token_arr)
    {
        LOG(">>> couldn't allocate memory for token array%40s\n", "[error]");
        free(buff);
        return TOK_MEM_ALC_ERR;
    }
    int token_number = 0;
    for (int pos = 0; pos < file_size;)
    {
        token_arr[token_number] = detect_token(buff, &pos);
        if ((token_arr[token_number].data_type == YET_TO_DET) && (token_arr[token_number].data.string == NULL))
        {
            LOG(">>> string has NULL pointer in tokens%40s\n", "[error]");
            free(buff);
            free(token_arr);
            return TOK_NULL_STR;
        }
        token_number++;
        if (token_number == token_arr_size)
            token_arr = token_realloc(token_arr, token_arr_size *= 2);
        
    }
    LOG("> all tokens were found, token number is: %d\n", token_number);
    token_number++;

    token_arr =  token_realloc(token_arr, token_number);
    token_arr[token_number - 1].data_type = $;
    *tokens = token_arr;

    LOG(">> tokenizator worked successfull, token array pointer is: %p\n", *tokens);
    PRINT_TOKENS();
    free(buff);

    return 0;
}

token_t *token_realloc(token_t *token_arr, int arr_size)
{
    assert(token_arr);
    assert(arr_size > 0);

    token_t *arr_holder = (token_t *)realloc(token_arr, arr_size * sizeof(token_t));
    if (!arr_holder)
    {
        LOG(">>> couldn't reallocate memory for token array%40s\n", "[error]");
        return token_arr;
    }
    LOG(">> token array size was reallocated to %d\n", arr_size);
    
    return arr_holder;
}

token_t detect_token(char *buff, int *pos)
{
    token_t token = {0};

    while (isspace(buff[*pos]))
    {
        (*pos)++;
        LOG("> current pos: %d, the rest is: %s\n", *pos, buff + *pos);
    }

    double number_holder = 0;
    int n = 0;
    
    if (sscanf(buff + *pos, "%lf%n", &number_holder, &n))
    {
        LOG("> number found: %lf\n", number_holder);
        (*pos) += n;
        token.data.number = number_holder;
        token.data_type = NUM;
        LOG("> current pos: %d, the rest is: %s\n", *pos, buff + *pos);
        return token;
    }

    for (int i = 0; i < COMMANDS_NUMBER; i++)
        if (!strncmp(buff + *pos, commands[i].cmd_name, commands[i].cmd_length))
        {
            LOG("> command %s found: %d\n", commands[i].cmd_name, commands[i].cmd);
            token.data.command = commands[i].cmd;
            token.data_type = COMMAND;
            (*pos) += commands[i].cmd_length;
            LOG("the smeshenie: %d\n", commands[i].cmd_length);
            LOG("> current pos: %d, the rest is: %s\n", *pos, buff + *pos);
            return token;
        }

    //char str[] = {0};
    sscanf(buff + *pos, "%*[a-zA-Z_]%n", &n);
    LOG("> scanf founded a word with length: %d\n\n\n", n);
    if (!n)
    {
        LOG(">>> sscanf didn't read symbol: %d, returning empty token.%40s\n", buff[*pos], "[error]");
        (*pos)++;
        return token;
    }

    char *string = (char *)calloc(n + 1, sizeof(char));
    if (!string)
        LOG("> couldn't allocate memory for the string, be carefull%40s\n", "[error]");

    
    sscanf(buff + *pos, "%[a-zA-Z_]", string);
    (*pos) += n;
    token.data.string   = string;
    token.data_type     = YET_TO_DET;

    LOG("> current pos: %d, the rest is: %s\n", *pos, buff + *pos);
    return token;
}

void free_tok_strings(token_t *tokens)
{
    for (int i = 0; (tokens + i)->data_type != $; i++)
        if (tokens[i].data_type == YET_TO_DET)
            free((void *)((tokens + i)->data.string));
}

void token_dump(FILE* out, token_t *tokens, int token_num)
{
    fprintf(out, "-------------TOKEN DUMP---------------\n");
    for (int i = 0; i < token_num; i++)
    {
        fprintf(out, "%d)\n", i + 1);
        fprintf(out, "> DATA:           ");
        switch (tokens[i].data_type)
        {
        case NUM:
            fprintf(out, "%.2lf\n", tokens[i].data.number);
            fprintf(out, "> DATA TYPE:      NUM\n");
            break;
        case COMMAND:
            fprintf(out, "%#04x\n", tokens[i].data.command);
            fprintf(out, "> DATA TYPE:      COMMAND\n");
            break;
        case YET_TO_DET:
            fprintf(out, "%s\n", tokens[i].data.string);
            fprintf(out, "> DATA TYPE:      YET TO DETERMINE\n");
            break;
        case $:
            fprintf(out, "$\n");
            fprintf(out, "> tokens ended\n");
            break;
        default:
            LOG(">>> fatal error: couldnt detect token type%40s\n", "[error]");
            break;
        }
    }
    fprintf(out, "-----------------------------------------\n");
    
}
