#include "include/lang.h"

FILE *logs = NULL;

int main(int argc, char const *argv[])
{
    assert(argc > 1);
    assert(argv[argc - 1]);
    _OPEN_LOG("logs/token_test.log");

    token_t *token_arr = {0};
    
    int error = tokenize(&token_arr, argv[argc - 1]);
    if (error)
        return error;
    

    _CLOSE_LOG();
    free(token_arr);
    return 0;
}
