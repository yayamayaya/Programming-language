#include "../include/compiler.h"
//Задача компилятора: провести первичные математические преобразования, перевести всё в команды ассемблера
/*
итерация 0.5: сокращение констант
итерация 1: научится выделять память под переменные
итерация 2: сделать условия, циклы
итерация 2,5: улучшить сохранение и создание переменных
итерация 3: реализовать функции
*/

FILE *asm_file = NULL;
int free_mem_ptr = 0;

variables var_arr = {0};
local_memory lcl_mem = {0};

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

    LOG("> initializing memory arrays\n");
    var_arr.vars = (variable_t *)calloc(10, sizeof(variable_t));
    if (!var_arr.vars)
    {
        LOG(">>> couldn't allocate memory for variable array%40s\n", "[error]");
        return VAR_ARR_MEM_ALC_ERR;
    }
    /*lcl_mem.loc_mems_size = (int *)calloc(10, sizeof(int));
    if (!lcl_mem.loc_mems_size)
    {
        LOG(">>> couldn't allocate memory for the local_mem_arr%40s\n", "[error]");
        fclose(asm_file);
        return MEM_NUM_ARR_ALC_ERR;
    }*/

    LOG("> compiling body\n");
    error = compile_body(root);
    if (error)
    {
        LOG("translation error occured%40s\n", "[error]");
        fclose(asm_file);
        return error;
    }
    
    free(var_arr.vars);
    fclose(asm_file);
    free(lcl_mem.loc_mems_size);
    var_arr.vars = NULL;
    var_arr.var_num = 0;
    asm_file = NULL;
    LOG(">> compilation successfull\n");

    return 0;
}