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

    LOG("> initializing memory array\n");
    Stack <variable_t> vars = {};
    vars.stackCtor(10, "logs/stack.log");

    LOG("> compiling body\n");
    error = compile_body(&vars, root);
    if (error)
    {
        LOG("translation error occured%40s\n", "[error]");
        fclose(asm_file);
        return error;
    }
    
    vars.stackDtor();
    fclose(asm_file);
    asm_file = NULL;
    LOG(">> compilation successfull\n");

    return 0;
}