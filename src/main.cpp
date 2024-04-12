#include "include/lang.h"

FILE *logs = NULL;

int main(int argc, char const *argv[])
{
    assert(argc > 1);
    assert(argv[argc - 1]);
    _OPEN_LOG("logs/main.log");

    token_t *token_arr = {0};
    
    int error = tokenize(&token_arr, argv[argc - 1]);
    if (error)
    {
        _CLOSE_LOG();
        return error;
    }
    
    node_t *root = create_syntax_tree(token_arr);
    create_gparh_code(root, EXPR);

    kill_tree(root);
    free_tok_strings(token_arr);
    free(token_arr);
    _CLOSE_LOG();
    return 0;
}
