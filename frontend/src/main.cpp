#include "include/lang.h"
#include "include/save_tree.h"

_INIT_LOG();

int main(int argc, char const *argv[])
{
    if (argc != 3 || !argv[argc - 2] || !argv[argc - 1])
    {
        printf("[error]>>> not enough arguments in programm call\n");
        return ERR;
    }
    _OPEN_LOG("logs/main.log");
    clear_all_png();

    token_t *token_arr = {0};
    LOG("> starting tokenization\n");
    int error = tokenize(&token_arr, argv[argc - 1]);
    if (error)
    {
        _CLOSE_LOG();
        return error;
    }
    LOG("> starting parsing\n");
    node_t *root = create_syntax_tree(token_arr);
    if (!root)
    {
        free_tok_strings(token_arr);
        free(token_arr);
        _CLOSE_LOG();
        return NULL_ROOT_ERR;
    }

    LOG("> starting graph creation\n");
    _CREATE_GRAPH(root);

    error = save_tree(argv[argc - 2], root);
    
    kill_tree(root);
    free(token_arr);
    LOG(">> programm ended\n");
    _CLOSE_LOG();
    return error;
}