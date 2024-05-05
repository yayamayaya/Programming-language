#include "../include/translator.h"

//TO DO избавиться от 4 переменных ниже

_INIT_LOG();

int translator(node_t *root)
{
    assert(root);
    _OPEN_LOG("logs/translator.log");

    LOG("------------------------STARTING COMPILATION------------------------------\n");
    LOG("> calculating constants:\n");
    int error = calc_consts(root);
    if (error)
        return error;
    LOG("> const calculations were done\n");

    LOG("\n\n> starting assembly translation:\n");
    FILE *asm_file = fopen("../data/code.txt", "wb");
    if (!asm_file)
    {
        LOG(">>> cannot open assembly file %40s\n", "[error]");
        _CLOSE_LOG();
        return ASM_F_OPEN_ERR;
    }

    LOG("> creating functions and global variables stacks\n");
    Stack <variable_t> gl_vars = {};
    Stack <func_t> funcs = {};

    memory_work memory = {0, &gl_vars, &funcs};

    memory.funcs->stackCtor(10, "logs/func_stack.log");
    memory.global_vars->stackCtor(10, "logs/global_var.log");

    LOG("> compiling programm\n");
    error = compile_func(asm_file, &memory, root);
    if (error)
        {LOG("translation error occured%40s\n", "[error]");}
    else
        {LOG(">> compilation successfull\n");}

    memory.global_vars->stackDtor();
    memory.funcs->stackDtor();
    fclose(asm_file);
    _CLOSE_LOG();

    return error;
}