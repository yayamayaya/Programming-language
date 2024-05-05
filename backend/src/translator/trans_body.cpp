#include "../include/body.h"

int make_body_command(FILE* asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node);
void free_local_mem(FILE *asm_file, Stack <variable_t> *vars, int var_num);
void var_dump(Stack <variable_t> *vars);
//void free_stk_frame(Stack <variable_t> *vars);

//Разбить большую функцию, избавиться от глобальных переменных,  сделать общую структур для глобально используемых переменных

_INIT_LOG();

/*int first_lcl_var_pos = 0;
int last_lcl_var_pos = 0;*/

int translate_body(FILE* asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *body_root)
{
    assert(asm_file);
    assert(memory);
    assert(body_root);
    assert(vars);

    _OPEN_N_LOG("logs/body%d.log");

    if (body_root->data_type != CONN)
    {
        LOG("fatal error occured, body root is not a connection node%40s\n", "[error]");
        _CLOSE_LOG();
        return BODY_IS_NOT_NODE_ERR;
    }
    int first_lcl_var_pos = 0;
    int last_lcl_var_pos = 0;

    first_lcl_var_pos = vars->getStackSize();
    for (int i = 0; i < body_root->branch_number; i++)
    {
        int error = make_body_command(asm_file, memory, vars, body_root->branches[i]);
        if (error)
        {
            _CLOSE_LOG();
            return error;
        }
    }
    LOG("> body was read successfully\n");
    last_lcl_var_pos = vars->getStackSize();

    var_dump(vars);
    free_local_mem(asm_file, vars, last_lcl_var_pos - first_lcl_var_pos);

    _CLOSE_LOG();

    return 0;
}

int make_body_command(FILE* asm_file, memory_work *memory, Stack <variable_t> *vars, node_t *node)
{
    assert(node);
    assert(asm_file);
    assert(memory);
    assert(vars);

    LOG("> searching for body command:\n");
    int error = 0;
    switch (node->data_type)
    {
    case OP:

        switch (node->data.command)
        {

        case E:
            error = assign_variable(asm_file, memory, vars, node);
            break;
        
        case IF:
            _CHECK_NODE_NUM(2);

            error = expr_in_asm(asm_file, memory, vars, node->branches[R]->branches[0]);
            if (error)
                return error;

            _PUSH_NUM(0);
            _JE_LABEL('L', node);
            _POP_REG("bx");

            error = translate_body(asm_file, memory, vars, node->branches[L]);

            _LABEL('L', node);
            _POP_REG("bx");
            break;

        case WHILE:
            _CHECK_NODE_NUM(2);

            LOG("> translating while\n");
            _LABEL('S', node);
            error = expr_in_asm(asm_file, memory, vars, node->branches[R]->branches[0]);
            if (error)
                return error;

            _PUSH_NUM(0);
            _JE_LABEL('L', node);
            _POP_REG("bx");

            error = translate_body(asm_file, memory, vars, node->branches[L]);

            _JMP_LABEL('S', node);
            _LABEL('L', node);
            _POP_REG("bx");
            break;

        case RET:
            LOG("> return was found\n");
            error = expr_in_asm(asm_file, memory, vars, node->branches[0]);
            
            _POP_REG("ax");
            _POP_REG("cx");
            _PUSH_REG("ax");
            _RET();
            //return_flag = 1;

            break;

        case SCAN:
        {
            LOG("> translating scan:\n");

            _CHECK_NODE_NUM(1);
            if (node->branches[0]->data_type != VAR)
            {
                LOG("[error]>>> fatal error, scan argument is not an variable\n");
                return FATAL_ERR;
            }

            _IN();
            
            error = pop_var_in_asm(asm_file, memory->global_vars, vars, node->branches[0]->data.string);

            break;
        }
        default:
            LOG(">>> undefined compilation operator, yet to define%40s\n", "[error]");
            return FATAL_ERR;
        }
        break;

    case VAR:
        if (find_var(vars, node->data.string))
            {LOG("variable already exists, continuing forward\n");}
        else
            error = create_variable(asm_file, vars, node);
        break;

    case CONN:
        _CHECK_NODE_NUM(1);

        error = expr_in_asm(asm_file, memory, vars, node);

        _POP_REG("dx");

        break;

    case FUNC:
        error = call_func(asm_file, memory, vars, node);
        break;

    default:
        LOG("[error]>>> fatal error occured: couldn't detect node data type\n");
        return FATAL_ERR;
    }

    return error;
}

void free_local_mem(FILE *asm_file, Stack <variable_t> *vars, int var_num)
{
    assert(vars);
    assert(asm_file);
    
    LOG("> freeing %d local variables:\n", vars->getStackSize());
    for (int i = 0; i < var_num; i++)
    {
        variable_t var_to_del = {};
        vars->stackPop(&var_to_del);
        _FREE_LCL_MEM(var_to_del.rel_address);
    }
    
    LOG("> current variables number: %d\n", vars->getStackSize());
    var_dump(vars);

    LOG("> memory was free'd successfully\n");
    return;
}

void var_dump(Stack <variable_t> *vars)
{
    assert(vars);

    LOG("\n ----------------------VARIABLES DUMP:------------------------\n");

    for (int i = 0; i < vars->getStackSize(); i++)
        LOG("%d) %s, memory location: %d\n", i, vars->getDataOnPos(i).var, vars->getDataOnPos(i).rel_address);
    
    LOG("--------------------------DUMP ENDED----------------------------\n");
}

/*void free_stk_frame(Stack <variable_t> *vars)
{
    assert(vars);

    LOG("> freeing all memory in stk_frame:\n");
    for (int i = 0; i < vars->getStackSize(); i++)
        _FREE_LCL_MEM(i);

    return;
}*/