#include "../include/trans_func.h"

void mem_to_call(int *mem_size, node_t *node);
int create_global_vars(FILE *asm_file, memory_work *memory, node_t *node);
int translate_function(FILE *asm_file, memory_work *memory, node_t *node);

_INIT_LOG();

int compile_func(FILE *asm_file, memory_work *memory, node_t *root)
{
    assert(memory);
    assert(asm_file);
    assert(root);
    _OPEN_LOG("logs/function_translation.log");
    if (root->data_type != LINKER)
    {
        LOG(">>> root is not a PROGRAMM node, programm will not be compiled%40s\n", "[error]");
        _CLOSE_LOG();
        return ROOT_NOT_PROG_ERR;
    }
    
    LOG("> starting frame allocation for global variables\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == VAR || root->branches[br]->data_type == OP)
            memory->free_mem_ptr++;
    LOG("> frame allocation complete, global variables number is: %d\n", memory->free_mem_ptr);

    LOG("> detecting memory size for call of each function\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == FUNC || root->branches[br]->data_type == MAIN)
        {
            const char *func_name = root->branches[br]->data.string;
            int arg_num = root->branches[br]->branches[R]->branch_number;
            LOG("> function %s definition was found\n", func_name);

            int mem_size = 0;
            mem_to_call(&mem_size, root->branches[br]);
            func_t func = {func_name, arg_num, mem_size};
            memory->funcs->stackPush(func);
        }
    LOG("> memory size allocations were completed\n");

    int error = 0;
    int main_flag = 0;
    LOG("> creating global variables:\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type != FUNC)
        {
            if (root->branches[br]->data_type == MAIN)
            {
                LOG("> main found\n");
                main_flag = 1;
                continue;
            }
            
            error = create_global_vars(asm_file, memory, root->branches[br]);
            if (error)
            {
                _CLOSE_LOG();
                return error;
            }
        }

    if (!main_flag)
    {
        LOG("[error]>>> main function wasn't found, stopping translation\n");
        printf("[error]>>> main function wasn't found, stopping translation\n");
        _CLOSE_LOG();
        return MAIN_NOT_FOUND;
    }
    
    _CALL_MAIN(memory->global_vars->getStackSize());

    LOG("> compiling functions\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == FUNC || root->branches[br]->data_type == MAIN)
        {
            error = translate_function(asm_file, memory, root->branches[br]);
            if (error)
            {
                _CLOSE_LOG();
                return error;
            }
        }

    LOG("functions compilations were done\n");
    _CLOSE_LOG();
    return error;
}

void mem_to_call(int *mem_size, node_t *node)
{
    for (int br = 0; br < node->branch_number; br++)
        mem_to_call(mem_size, node->branches[br]);

    if (node->data_type == VAR)
        (*mem_size)++;
}

int create_global_vars(FILE *asm_file, memory_work *memory, node_t *node)
{
    assert(node);
    assert(memory);
    assert(asm_file);

    LOG("creating global variable\n");
    int error = 0;
    switch (node->data_type)
    {
    case OP:
        if (node->data.command != ASSIGN)
        {
            LOG(">>> fatal error: operator is not assignment%40s\n", "[error]");
            return OP_NOT_ASS_ERR;
        }

        error = assign_variable(asm_file, memory, memory->global_vars, node);
        break;

    case VAR:
        if (find_var(memory->global_vars, node->data.string))
            {LOG("> variable already exists, continuing forward\n");}
        else
            create_variable(asm_file, memory->global_vars, node);
        break;
    
    default:
        LOG(">>> unaccaptable programm operation%40s\n", "[error]");
        return FATAL_ERR;
    }

    return error;
}

int translate_function(FILE *asm_file, memory_work *memory, node_t *node)
{
    assert(asm_file);
    assert(node);
    assert(memory);
    LOG("> translating a function to asm code:\n");

    func_t *func = find_func(memory->funcs, node->data.string);
    if (!func)
    {
        LOG(">>> function def. wasn't found, fatal error%40s\n", "[error]");
        return FUNC_FAT_ERR;
    }
    
    _FUNC(func->func);

    LOG(">creating a variable stack\n");
    Stack <variable_t> vars = {};
    vars.stackCtor(10, "logs/vars.log");

    int error = 0;
    LOG("> allocating memory for the arguments:\n");
    for (int arg = 0; arg < node->branches[R]->branch_number; arg++)
        create_variable(asm_file, &vars, node->branches[R]->branches[arg]);

    LOG("> arguments were allocated, compiling a body:\n");
    error = start_body_transl(asm_file, memory, &vars, node->branches[L]);
    LOG("> body compilation finished\n");

    vars.stackDtor();

    return error;
}

int call_func(FILE *asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node)
{
    assert(asm_file);
    assert(memory);
    assert(vars);
    assert(node);
    assert(node->data_type == FUNC);

    LOG("> ---------------------------------------------FUNCTION CALL FOUND--------------------------\n");

    func_t *function = find_func(memory->funcs, node->data.string);
    if (!function)
    {
        LOG(">>> function definition wasn't found, compilation error%40s\n", "[error]");
        return FUNC_DEF_NOT_F_ERR;
    }
    if (function->arg_num != node->branches[0]->branch_number)
    {
        LOG(">>> the number of the arguments in %s function call does not match the arg number in function%40s\n", function->func, "[error]");
        return ARG_NUM_NOT_MATCH_ERR;
    }

    LOG("> allocating stack frame:\n");
    _PUSH_REG("cx");
    _PUSH_REG("cx");
    _PUSH_NUM(memory->free_mem_ptr);
    _PR("add\n");
    _POP_REG("ax");

    for (int i = 0; i < function->arg_num; i++)
    {
        int error = expr_in_asm(asm_file, memory, vars, node->branches[0]->branches[i]);
        if (error)
        {
            LOG("> variable was not defined in this scope%40s\n", "[error]");
            return VAR_DEF_NOT_F;
        }
        
        _POP_REL_AX(i);
    }

    _MOV("cx", "ax");

    _CALL_FUNC(function->func);
    memory->free_mem_ptr += function->mem_size;
    return 0;
}

func_t *find_func(Stack <func_t> *funcs, const char *func_name)
{
    assert(funcs);
    LOG("> searching for function\n");

    for (int i = 0; i < funcs->getStackSize(); i++)
        if (!strcmp((funcs->getDataOnPos(i)).func, func_name))
        {
            LOG("> function %s was found\n", funcs->getDataOnPos(i).func);
            return funcs->getDataPtr() + i;
        }

    LOG(">>> function definition wasn't found programm will not be compiled%40s\n", "[error]");
    return NULL;
}