#include "../include/trans_func.h"

int stk_frame_pos = 0;
int return_flag = 0;

void allocate_stk_frames(node_t *node);
int translate_function(node_t *node);
int create_global_vars(node_t *node);

//TO DO посокращать длинные строки через функции, убрать костыль с Elbasy, венести кодогенерацию в дефайны

int compile_func(node_t *root)
{
    assert(funcs);
    assert(root);
    if (root->data_type != CONN || root->data.command != PROGRAMM)
    {
        LOG(">>> root is not a PROGRAMM node, programm will not be compiled%40s\n", "[error]");
        return ROOT_NOT_PROG_ERR;
    }
    
    LOG("> starting frame allocation for global variables\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == VAR || root->branches[br]->data_type == OP)
            stk_frame_pos++;
    LOG("> frame allocation complete, global variables number is: %d\n", stk_frame_pos);

    LOG("> starting stack frames allocations for functions\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == FUNC)
        {
            const char *func_name = root->branches[br]->data.string;
            int arg_num = root->branches[br]->branches[L]->branch_number;
            LOG("> function %s definition was found\n", func_name);
            func_t func = {func_name, arg_num, stk_frame_pos};
            funcs->stackPush(func);
            allocate_stk_frames(root->branches[br]);
        }
    stk_frame_pos = 0;
    LOG("> stack frame allocations were completed\n");

    if (find_func("Elbasy") == NULL)
    {
        LOG(">>> main function not found, comp. error%40s\n", "[error]");
        return MAIN_NOT_FOUND;
    }

    int error = 0;
    LOG("> creating global variables:\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type != FUNC)
        {
            error = create_global_vars(root->branches[br]);
            if (error)
                return error;
        }
    
    _CALL_MAIN();

    LOG("> compiling functions\n");
    for (int br = 0; br < root->branch_number; br++)
        if (root->branches[br]->data_type == FUNC)
        {
            error = translate_function(root->branches[br]);
            if (error)
                return error;
        }

    LOG("functions compilations were done\n");
    return error;
}

void allocate_stk_frames(node_t *node)
{
    for (int br = 0; br < node->branch_number; br++)
        allocate_stk_frames(node->branches[br]);

    if (node->data_type == VAR)
        stk_frame_pos++;
}

int create_global_vars(node_t *node)
{
    assert(node);
    assert(asm_file);
    assert(funcs);

    LOG("allocating global variable\n");
    int error = 0;
    switch (node->data_type)
    {
    case OP:
        if (node->data.command != E)
        {
            LOG(">>> fatal error: operator is not assignment%40s\n", "[error]");
            return OP_NOT_ASS_ERR;
        }

        error = assign_variable(global_vars, node);
        break;

    case VAR:
        if (find_var(global_vars, node->data.string))
            {LOG("variable already exists, continuing forward\n");}
        else
            error = create_variable(global_vars, node);
        break;
    
    default:
        LOG(">>> unaccaptable programm operation%40s\n", "[error]");
        return FATAL_ERR;
    }

    return error;
}

int translate_function(node_t *node)
{
    assert(node->data_type == FUNC);
    LOG("> translating a function to asm code:\n");

    func_t *func = find_func(node->data.string);
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
    for (int arg = 0; arg < node->branches[L]->branch_number; arg++)
    {
        error = create_variable(&vars, node->branches[L]->branches[arg]);
        if (error)
        {
            vars.stackDtor();
            return error;
        }
    }

    int gl_var_mem_ptr = free_mem_ptr;
    free_mem_ptr = 0;
    LOG("> arguments were allocated, compiling a body:\n");
    error = compile_body(&vars, node->branches[R]);
    LOG("> body compilation finished\n");
    free_mem_ptr = gl_var_mem_ptr;

    if (!return_flag)
    {
        LOG(">>> return in function wasn't found, compilation error%40s\n", "[error]");
        error = RET_NOT_FOUND_ERR;
    }
    vars.stackDtor();
    return_flag = 0;

    return error;
}

int call_func(Stack <variable_t> *vars, node_t *node)
{
    assert(funcs);
    assert(node);
    assert(node->data_type == FUNC);

    func_t *function = find_func(node->data.string);
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

    LOG("> changing cx loaction:\n");
    _PUSH_REG("cx");
    _PUSH_NUM(function->rbp);
    _POP_REG("cx");

    for (int i = 0; i < function->arg_num; i++)
    {
        int error = expr_in_asm(vars, node->branches[0]->branches[i]);
        if (error)
        {
            LOG("> variable was not defined in this scope%40s\n", "[error]");
            return VAR_DEF_NOT_F_ERR;
        }
        
        _POP_REL(i);
    }
    _CALL_FUNC(function->func);
    return 0;
}

func_t *find_func(const char *func_name)
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