#include "../include/lang.h"

//TO DO: избавиться от глобальных токенов, вынести некоторые ифы в дефайны

/*

1 итерация:
code        ::= {variable}+0
variable    ::= name [var_op E] "eu"
var_op      ::= "="
E           - стандартный парсер выражений из дифф
name        ::= [а-я, А-Я]

2 итерация:
code        ::= body+0
body        ::= STR+
STR         ::= if | variable
if          ::= "if" '('E')' ['{']variable['}']
variable    ::= name [var_op E] "eu"
var_op      ::= "="
E           - стандартный парсер выражений из дифф
name        ::= [а-я, А-Я]

3 итерация:
code        ::= body+0
body        ::= STR+
STR         ::= cond | variable
cond        ::= '{']variable['}' '('E')' {"if" | "while"}
variable    ::= "eu, " name [E "="]
E           - стандартный парсер выражений из дифф + лог. операции
name        ::= [а-я, А-Я]

4 итерация:
code        ::= {func}+$
func        ::= name {variable | {args body}}
body        ::= '{'STR+'}'
STR         ::= cond | func_call | E
cond        ::= body '('E')' {"if" | "while"}
variable    ::= E "="
E           - стандартный парсер выражений из дифф + лог. операции + вызов функций
func_call   ::= "eu, " name {args | variable}
args        ::= '('name [, args]')'
name        ::= [а-я, А-Я]

4,5 итерация:
code        ::= {func}+$
func        ::= name {assignment | {args body}}
body        ::= '{'STR+'}'
STR         ::= cond | func_call | E | ret
cond        ::= body '('E')' {"if" | "while"}
assignment    ::= E "bolad"
E           - стандартный парсер выражений из дифф + лог. операции + вызов функций
func_call   ::= "eu, " name {args | assignment}
args        ::= '('name [, args]')'
name        ::= [а-я, А-Я]
ret         ::= "sygeide" E

4,75 итерация:
code        ::= {func}+$
func        ::= name {assignment | {args body}}
args        ::= '('name [, args]')'
body        ::= '{'STR+'}'
STR         ::= {cond | ret | E} |{"eu," variable}
cond        ::= body '('E')' {"if" | "while"}
assignment  ::= E "bolad"
E           - стандартный парсер выражений из дифф + лог. операции + вызов функций
func_call   ::= "eu, " name call_args
call_args   ::= '('{ | E [, E]*')'
name        ::= [а-я, А-Я]
ret         ::= "sygeide" E

5 итерация:
code        ::= {func}+$
func        ::= name {assignment | {args body}}
args        ::= '('name [, args]')'
body        ::= '{'STR+'}'
STR         ::= {cond | ret | E} |{"eu," {variable | scan}}
scan        ::= {"scan" '('name')'}
cond        ::= body '('E')' {"if" | "while"}
assignment  ::= E "bolad"
E           - стандартный парсер выражений из дифф + лог. операции + вызов функций
func_call   ::= "eu, " name call_args
call_args   ::= '('{ | E [, E]*')'
name        ::= [а-я, А-Я]
ret         ::= "sygeide" E

*/

_INIT_LOG();

node_t *pars_STR(token_t *tkns, int *pos);
node_t *pars_body(token_t *tkns, int *pos);
node_t *pars_func(token_t *tkns, int *pos);
node_t *pars_cond(token_t *tkns, int *pos);
node_t *pars_func_call(token_t *tkns, int *pos);
node_t *pars_call_args(token_t *tkns, int *pos);
node_t *pars_args(token_t *tkns, int *pos);
node_t *pars_E(token_t *tkns, int *pos);
node_t *pars_sum(token_t *tkns, int *pos);
node_t *pars_mult(token_t *tkns, int *pos);
node_t *pars_power(token_t *tkns, int *pos);
node_t *pars_number(token_t *tkns, int *pos);
node_t *pars_variable(token_t *tkns, int *pos);
node_t *pars_assignment(token_t *tkns, int *pos);
node_t *pars_scan(token_t *tkns, int *pos);
node_t *pars_name(token_t *tkns, int *pos);

node_t *create_syntax_tree(token_t *tkns)
{
    assert(tkns);
    _OPEN_LOG("logs/parsing.log");
    LOG("-----------------------------------------------\n\n");
    LOG("> creating syntax tree:\n");

    int pos = 0;
    node_t *root = create_node(PROGRAMM, CONN, 0);
    node_t *node = NULL;
    
    do {
        node = pars_func(tkns, &pos);
        _ADD_B(root, node);
    } while (node && tkns[pos].data_type != $);

    if (tkns[pos].data_type != $)
    {
        LOG("[error]>>> $ not found <(%p)>\n", tkns);
        kill_tree(root, DONT_KILL_STRS);
        root = NULL;
    }

    _CLOSE_LOG();
    return root;
}

node_t *pars_func(token_t *tkns, int *pos)
{
    LOG("> creating function\n");

    node_t *func = pars_name(tkns, pos);
    if (!func)
        return NULL;

    node_t *args = pars_args(tkns, pos);
    if (!args)
    {
        LOG("> arguments were'nt parsed, checkin for the variable:\n");
        func->data_type = VAR;
        node_t *var = pars_assignment(tkns, pos);
        if (!var)
            return func;

        _ADD_B(var, func);
        LOG("> variable created\n");
        return var;
    }

    func->data_type = FUNC;
    node_t *body = pars_body(tkns, pos);
    _ADD_B(func, args);
    _ADD_B(func, body);

    LOG("> function created\n");
    return func;
}

node_t *pars_ret(token_t *tkns, int *pos)
{
    if (tkns[*pos].data_type != COMMAND || tkns[*pos].data.command != RET)
    {
        LOG("> return not found\n");
        return NULL;
    }
    TOK_SHIFT();

    node_t *expr = pars_E(tkns, pos);
    if (!expr)
        return NULL;
    
    return create_node(RET, OP, 1, expr);
}

node_t *pars_STR(token_t *tkns, int *pos)
{
    node_t *node = pars_ret(tkns, pos);
    if (node)
        return node;
    
    node = pars_E(tkns, pos);
    if (node)
        return node;
    
    node = pars_cond(tkns, pos);
    if (node)
        return node;

    if (tkns[*pos].data_type != COMMAND || (tkns[*pos].data.command != STR_END))
    {
        LOG("[error]>>> string start wasn't found, syntax error\n");
        printf("[error]>>> string start wasn't found, syntax error\n");
        return NULL;
    }
    TOK_SHIFT();
    LOG("> ZAP founded tok shifted\n");

    node = pars_scan(tkns, pos);
    if (node)
        return node;

    node = pars_variable(tkns, pos);
    if (node)
        return node;

    LOG(">>> string wasn't found, returning NULL\n");
    return NULL;
}

node_t *pars_scan(token_t *tkns, int *pos)
{
    LOG("> parsing scan\n");
    if (tkns[*pos].data_type != COMMAND || tkns[*pos].data.command != SCAN)
    {
        LOG("> scan wasn't found\n");
        return NULL;
    }
    TOK_SHIFT();

    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != OP_BR)
    {
        LOG("[error]>>> syntax error: opening bracket wasn't found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: opening bracket wasn't found\n");
        //kill_tree(body, DONT_KILL_STRS);
        return NULL;
    }
    TOK_SHIFT();

    node_t *arg_name = pars_name(tkns, pos);
    if (!arg_name)
        return NULL;

    arg_name->data_type = VAR;

    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_BR)
    {
        LOG("[error]>>> syntax error: closing bracket wasn't found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: closing bracket wasn't found\n");
        kill_tree(arg_name, DONT_KILL_STRS);
        return NULL;
    }
    TOK_SHIFT();

    LOG("> scan parsed, returning branch\n");
    return create_node(SCAN, OP, 1, arg_name);
}

node_t *pars_body(token_t *tkns, int *pos)
{
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != OP_F_BR)
    {
        LOG("> opening figure bracket not found\n");
        return NULL;
    }
    LOG("> opening figure bracket found\n");
    TOK_SHIFT();

    node_t *node = NULL;
    node_t *body = create_node(BODY, CONN, 0);
    do
    {
        node = pars_STR(tkns, pos);
        if (!node)
        {
            kill_tree(body, DONT_KILL_STRS);
            return NULL;
        }
        
        _ADD_B(body, node);
    } while (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_F_BR);

    if (!body->branch_number)
    {
        LOG("> body don't have any strings, returning NULL\n");
        kill_tree(body, DONT_KILL_STRS);
        return NULL;
    }
    
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_F_BR)
    {
        LOG("[error]>>> closing figure bracket not found\n");
        printf("[error]>>> closing figure bracket not found\n");
        kill_tree(body, DONT_KILL_STRS);
        return NULL;
    }
    LOG("> closing figure bracket found\n");
    TOK_SHIFT();

    LOG("body created with: %d branches\n", body->branch_number);
    return body;
}

node_t *pars_cond(token_t *tkns, int *pos)
{
    LOG("> parsing condition\n");
    node_t *body = pars_body(tkns, pos);
    if (!body)
        return NULL;

    LOG("> body in the cond was parsed\n");

    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != OP_BR)
    {
        LOG("[error]>>> syntax error: opening bracket wasn't found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: opening bracket wasn't found\n");
        kill_tree(body, DONT_KILL_STRS);
        return NULL;
    }
    TOK_SHIFT();

    node_t *expr = pars_E(tkns, pos);
    if (!expr)
    {
        kill_tree(body, DONT_KILL_STRS);
        return NULL;
    }
    
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_BR)
    {
        LOG("[error]>>> syntax error: closing bracket wasn't found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: closing bracket wasn't found\n");
        kill_tree(body, DONT_KILL_STRS);
        kill_tree(expr, DONT_KILL_STRS);
        return NULL;
    }
    TOK_SHIFT();

    if (tkns[*pos].data_type != OP || !(tkns[*pos].data.command == IF || tkns[*pos].data.command == WHILE))
    {
        LOG("[error]>>> syntax error: if/while operator wasn't found... <(%p)>\n", tkns);
        printf("[error]>>> syntax error: if/while operator wasn't found\n");
        kill_tree(body, DONT_KILL_STRS);
        kill_tree(expr, DONT_KILL_STRS);
        return NULL;
    }
    node_t *node = create_node(tkns[*pos].data.command, OP, 2, body, expr);
    TOK_SHIFT(); 
    
    return node;
}

node_t *pars_variable(token_t *tkns, int *pos)
{
    LOG("> creating an variable:\n");

    node_t *name = pars_name(tkns, pos);
    if (!name)
        return NULL;
    name->data_type = VAR;

    if (tkns[*pos].data_type == COMMAND && tkns[*pos].data.command == VAR_END)
    {
        TOK_SHIFT();
        return name;
    }

    node_t *node = pars_assignment(tkns, pos);
    if (!node)
    {
        kill_tree(name, DONT_KILL_STRS);
        return NULL;
    }
    _ADD_B(node, name);

    return node;
}

node_t *pars_assignment(token_t *tkns, int *pos)
{
    LOG("> creating expression and operator:\n");
    node_t *expr = pars_E(tkns, pos);
    if (!expr)
        return NULL;
    
    if (tkns[*pos].data_type == OP && tkns[*pos].data.command == E)
    {
        LOG("> variable with assignment found\n");
        TOK_SHIFT();
        return create_node((unsigned char)E, OP, 1, expr);
    }

    LOG("[error]>>> syntax error, assign operator wasn't found <(%p)>\n", tkns);
    printf("[error]>>> syntax error, assign operator wasn't found\n");
    kill_tree(expr, DONT_KILL_STRS);
    return NULL;
}

node_t *pars_func_call(token_t *tkns, int *pos)
{
    LOG("> creating function call\n");

    node_t *name = pars_name(tkns, pos);
    if (!name)
        return NULL;
    name->data_type = VAR;

    node_t *node = pars_call_args(tkns, pos);
    if (!node)
        return name;

    name->data_type = FUNC;
    _ADD_B(name, node);

    LOG("> pars_func_call completed\n");
    return name;
}

node_t *pars_call_args(token_t *tkns, int *pos)
{
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != OP_BR)
    {
        LOG("> opening bracket not found, variable it is\n");
        return NULL;
    }
    LOG("> opening bracket was found on %p\n", tkns);
    TOK_SHIFT();

    node_t *expr = pars_E(tkns, pos);
    if (!expr)
        return NULL;

    node_t *args = create_node(ARGS, CONN, 1, expr);
    while (tkns[*pos].data_type == OP && tkns[*pos].data.command == ZAP)
    {
        TOK_SHIFT();
        expr = pars_E(tkns, pos);
        if (!expr)
        {
            kill_tree(args, DONT_KILL_STRS);
            return NULL;
        }
        _ADD_B(args, expr);
    }

    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_BR)
    {
        LOG("[error]>>> syntax error: closing bracket not found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: closing bracket not found\n");
        kill_tree(args, DONT_KILL_STRS);
        return NULL;
    }
    TOK_SHIFT();

    return args;
}

node_t *pars_args(token_t *tkns, int *pos)
{
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != OP_BR)
    {
        LOG("> opening bracket not found\n");
        return NULL;
    }
    TOK_SHIFT();
    LOG("> opening bracket was found on %p\n", tkns);

    node_t *node = create_node((unsigned char)ARGS, CONN, 0);
    do
    {
        node_t *arg = pars_name(tkns, pos);
        if (!arg)
        {
            LOG(">>> arguments not found\n");
            break;
        }
        arg->data_type = VAR;
        _ADD_B(node, arg);

        if (tkns[*pos].data_type == OP && tkns[*pos].data.command == ZAP)
        {
            LOG("> ZAP was found\n");
            TOK_SHIFT();
        }
        else
        {
            LOG("> ZAP not found\n");
            break;
        }
    } while (1);
    
    if (tkns[*pos].data_type != OP || tkns[*pos].data.command != CL_BR)
    {
        LOG("[error]>>> syntax error: closing bracket not found <(%p)>\n", tkns);
        printf("[error]>>> syntax error: closing bracket not found\n");
        kill_tree(node, DONT_KILL_STRS);
        return NULL;
    }
    LOG("> closing bracket was found\n");
    TOK_SHIFT();
    
    return node;
}

node_t *pars_name(token_t *tkns, int *pos)
{
    node_t *node = NULL;

    if (tkns[*pos].data_type == YET_TO_DET)
    {
        LOG("> creating a string with name:\n");
        node = create_node(tkns[*pos].data.string, 0, 0);
        TOK_SHIFT();
    }
    return node;
}

//--------------------------------------------------------------Expression parser---------------------------------------------------------------
node_t *pars_E(token_t *tkns, int *pos)
{
    node_t *node = pars_sum(tkns, pos);
    if (!node)
        return NULL;
    
    while (tkns[*pos].data_type == COMMAND && (LOG_E <= tkns[*pos].data.command && tkns[*pos].data.command <= LOG_BE))
    {
        unsigned char command = tkns[*pos].data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 1, node);
        _ADD_B(node, pars_sum(tkns, pos));
    }

    node_t *expr = create_node(EXPR, CONN, 1, node);

    return expr;
}

node_t *pars_sum(token_t *tkns, int *pos)
{
    node_t *node = pars_mult(tkns, pos);
    if (!node)
        return NULL;

    while (tkns[*pos].data_type == COMMAND && (tkns[*pos].data.command == PLUS || tkns[*pos].data.command == MINUS))
    {
        unsigned char command = tkns[*pos].data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 1, node);
        _ADD_B(node, pars_mult(tkns, pos));
    }

    return node;
}

node_t *pars_mult(token_t *tkns, int *pos)
{
    node_t *node = pars_power(tkns, pos);
    if (!node)
        return NULL;

    while (tkns[*pos].data_type == COMMAND && (tkns[*pos].data.command == STAR || tkns[*pos].data.command == SLASH))
    {
        unsigned char command = tkns[*pos].data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 1, node);
        _ADD_B(node, pars_power(tkns, pos));
    }

    return node;
}

node_t *pars_power(token_t *tkns, int *pos)
{
    node_t *node = pars_number(tkns, pos);
    if (!node)
        return NULL;

    while (tkns[*pos].data_type == COMMAND && tkns[*pos].data.command == POW)
    {
        unsigned char command = tkns[*pos].data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 1, node);
        _ADD_B(node, pars_number(tkns, pos));
    }

    return node;
}

node_t *pars_number(token_t *tkns, int *pos)
{
    node_t *node = NULL;

    if (tkns[*pos].data_type == COMMAND && tkns[*pos].data.command == OP_BR)
    {
        TOK_SHIFT();
        node = pars_E(tkns, pos);
        
        if (tkns[*pos].data.command != CL_BR)
        {
            LOG("[error]>>> closing bracket wasn't found, character is: %#04x <(%p)>\n", tkns[*pos].data.command, tkns);
            printf("[error]>>> closing bracket wasn't found\n");
            kill_tree(node, DONT_KILL_STRS); 
            return NULL;
        }
        else
        {
            TOK_SHIFT();
        }
    }
    else if (tkns[*pos].data_type == NUMBER)
    {
        LOG("> number found\n");
        node = create_node(tkns[*pos].data.number, NUM, 0);
        TOK_SHIFT();
    }
    else if (tkns[*pos].data_type == YET_TO_DET)
    {
        LOG("> some string founded\n");
        node = pars_func_call(tkns, pos);
    }
    else
        LOG("> parser couldn't parse a number\n");

    return node;
}