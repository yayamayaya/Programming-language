#include "../include/compiler.h"
//Задача компилятора: провести первичные математические преобразования, перевести всё в команды ассемблера
/*
итерация 0.5: сокращение констант
итерация 1: научится выделять память под переменные
итерация 2: сделать условия, циклы
итерация 2,5: исправить сохранение и создание переменных
итерация 3: реализовать функции
*/

FILE *asm_file = NULL;
int free_mem_ptr = 0;
Stack <variable_t> *global_vars = NULL;
Stack <func_t> *funcs = NULL;

int compiler(node_t *root)
{
    assert(root);

    LOG("------------------------STARTING COMPILATION------------------------------\n");
    LOG("> calculating constants:\n");
    int error = calc_consts(root);
    if (error)
        return error;
    LOG("> const calculations were done\n");

    LOG("\n\n> starting assembly translation:\n");
    asm_file = fopen("data/code.txt", "wb");
    if (!asm_file)
    {
        LOG(">>> cannot open assembly file %40s\n", "[error]");
        return ASM_F_OPEN_ERR;
    }

    LOG("> creating functions and global variables stacks\n");
    Stack <func_t> functions = {};
    funcs = &functions;
    funcs->stackCtor(10, "logs/func_stack.log");

    Stack <variable_t> gl_vars = {};
    global_vars = &gl_vars;
    global_vars->stackCtor(10, "logs/global_var.log");

    LOG("> compiling programm\n");
    error = compile_func(root);
    if (error)
        LOG("translation error occured%40s\n", "[error]");
    else
        LOG(">> compilation successfull\n");

    global_vars->stackDtor();
    funcs->stackDtor();
    fclose(asm_file);
    asm_file = NULL;

    return error;
}