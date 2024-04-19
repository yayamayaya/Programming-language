#include "../include/lang.h"

/*
Язык имеет следующую грамматику:
code            ::= {variable | func}+0
func            ::= name args '{'STR+'}'
variable        ::= name [var_op E]
STR             ::= {variable | return | E | IF | ELSE | FOR | WHILE} "эу"
IF              ::= "if"'('{{{[variable] [var-op] [E]}}}')'['{'STR*'}']
ELSE            ::= "else"
WHILE           ::= "while"'('variable var_op [E]')'['{'STR*'}']
FOR             ::= "for" '('variable 'эу' {{{variable logical E}}} 'эу' E ')'
args            ::= '('{name,}*name')'
return          ::= "return" E
name            ::= ['а'-'я']+
var_op          ::= "="

где E - это стандартный парсинг выражений, с добавление вызовов функий в конце и логическими операторами
E               ::= M{[+, -]M}*
M               ::= P{[*, /]P}*
P               ::= U{^U}*
U               ::= ['1'-'9']+ | variable

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
*/

node_t *pars_STR();
node_t *pars_body();
node_t *pars_func();
node_t *pars_cond();
node_t *pars_func_call();
node_t *pars_args();
node_t *pars_E();
node_t *pars_sum();
node_t *pars_mult();
node_t *pars_power();
node_t *pars_number();
node_t *pars_variable();
node_t *pars_name();

token_t *tkns = NULL;

node_t *create_syntax_tree(token_t *token_arr)
{
    tkns = token_arr;
    LOG("-----------------------------------------------\n\n");
    LOG("> creating syntax tree:\n");

    node_t *root = create_node(PROGRAMM, CONN, 0);
    node_t *node = NULL;
    
    do {
        node = pars_func();
        _ADD_B(root, node);
    } while (node && tkns->data_type != $);

    if (tkns->data_type != $)
    {
        LOG("$ not found%40s\n", "[error]");
        kill_tree(root);
        root = NULL;
    }

    return root;
}

node_t *pars_func()
{
    LOG("> creating function\n");

    node_t *func = pars_name();
    if (!func)
        return NULL;

    node_t *args = pars_args();
    if (!args)
    {
        LOG("> arguments were'nt parsed, checkin for the variable:\n");
        func->data_type = VAR;
        node_t *var = pars_variable();
        if (!var)
            return func;

        _ADD_B(var, func);
        LOG("> variable created\n");
        return var;
    }

    func->data_type = FUNC;
    node_t *body = pars_body();
    _ADD_B(func, args);
    _ADD_B(func, body);

    LOG("> function created\n");
    return func;
}

node_t *pars_STR()
{
    node_t *node = pars_cond();
    if (node)
        return node;
    
    node = pars_func_call();
    if (node)
        return node;

    LOG(">>> string wasn't found, returning NULL\n");
    return NULL;
}

node_t *pars_body()
{
    if (tkns->data_type != OP || tkns->data.command != OP_F_BR)
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
        node = pars_STR();
        _ADD_B(body, node);
    } while (node && tkns->data_type != $);

    if (!body->branch_number)
    {
        LOG("> body don't have any strings, returning NULL\n");
        kill_tree(body);
        return NULL;
    }
    
    if (tkns->data_type != OP || tkns->data.command != CL_F_BR)
    {
        LOG("closing figure bracket not found\n");
        return NULL;
    }
    LOG("> closing figure bracket found\n");
    TOK_SHIFT();

    LOG("body created with: %d branches\n", body->branch_number);
    return body;
}

node_t *pars_cond()
{
    LOG("> parsing condition\n");
    node_t *body = pars_body();
    if (!body)
        return NULL;

    LOG("> body in the cond was parsed\n");

    if (tkns->data_type != OP || tkns->data.command != OP_BR)
    {
        LOG(">>> syntax error: opening bracket wasn't found%40s\n", "[error]");
        kill_tree(body);
        return NULL;
    }
    TOK_SHIFT();

    node_t *expr = pars_E();
    if (!expr)
    {
        kill_tree(body);
        return NULL;
    }
    
    if (tkns->data_type != OP || tkns->data.command != CL_BR)
    {
        LOG(">>> syntax error: closing bracket wasn't found%40s\n", "[error]");
        kill_tree(body);
        kill_tree(expr);
        return NULL;
    }
    TOK_SHIFT();

    if (tkns->data_type != OP || !(tkns->data.command == IF || tkns->data.command == WHILE))
    {
        LOG(">>> syntax error: if/while operator wasn't found...%40s\n", "[error]");
        kill_tree(body);
        kill_tree(expr);
        return NULL;
    }
    node_t *node = create_node(tkns->data.command, OP, 2, body, expr);
    TOK_SHIFT(); 
    
    return node;
}

node_t *pars_variable()
{
    LOG("> creating an variable:\n");

    //node->data_type = VAR;   

    if (tkns->data_type == OP && tkns->data.command == VAR_END)
    {
        LOG("a variable without assignment was found\n");
        TOK_SHIFT();
        return NULL;
    }
    
    LOG("> creating expression and operator:\n");
    node_t *expr = pars_E();

    if (tkns->data_type == OP && tkns->data.command == E)
    {
        LOG("> variable with assignment found\n");
        TOK_SHIFT();
        return create_node((unsigned char)E, OP, 1, expr);
    }

    LOG("syntax error, assign operator wasn't found%40s\n", "[error]");
    kill_tree(expr);
    return NULL;
}

node_t *pars_func_call()
{
    if ((tkns->data_type != COMMAND) || (tkns->data.command != STR_END))
    {
        LOG("string start wasn't found, returning NULL\n");
        return NULL;
    }
    TOK_SHIFT();
    LOG("> creating function call\n");

    node_t *func = pars_name();
    if (!func)
        return NULL;

    node_t *args = pars_args();
    if (!args)
    {
        func->data_type = VAR;
        node_t *var = pars_variable();
        if (!var)
            return func;

        _ADD_B(var, func);
        LOG("> variable created\n");
        return var;
    }

    func->data_type = FUNC;
    _ADD_B(func, args);

    LOG("> function call created\n");
    return func;
}

node_t *pars_args()
{
    if (tkns->data_type != OP || tkns->data.command != OP_BR)
    {
        LOG("> opening bracket not found\n");
        return NULL;
    }
    TOK_SHIFT();

    node_t *node = create_node((unsigned char)ARGS, CONN, 0);
    do
    {
        node_t *arg = pars_name();
        if (!arg)
        {
            LOG(">>> arguments not found\n");
            break;
        }
        arg->data_type = VAR;
        _ADD_B(node, arg);

        if (tkns->data_type == OP && tkns->data.command == ZAP)
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
    
    if (tkns->data_type != OP || tkns->data.command != CL_BR)
    {
        LOG(">>> syntax error: closing bracket not found%40s\n", "[error]");
        kill_tree(node);
        return NULL;
    }
    LOG("> closing bracket was found\n");
    TOK_SHIFT();
    
    return node;
}

node_t *pars_name()
{
    node_t *node = NULL;

    if (tkns->data_type == YET_TO_DET)
    {
        LOG("> creating a string with name:\n");
        node = create_node(tkns->data.string, 0, 0);
        TOK_SHIFT();
    }
    return node;
}

//--------------------------------------------------------------Expression parser---------------------------------------------------------------
node_t *pars_E()
{
    node_t *node = pars_sum();

    while (tkns->data_type == COMMAND && (LOG_E <= tkns->data.command && tkns->data.command <= LOG_BE))
    {
        unsigned char command = tkns->data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 2, node, pars_sum());
    }

    node_t *expr = create_node(EXPR, CONN, 1, node);

    return expr;
}

node_t *pars_sum()
{
    node_t *node = pars_mult();

    while (tkns->data_type == COMMAND && (tkns->data.command == PLUS || tkns->data.command == MINUS))
    {
        unsigned char command = tkns->data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 2, node, pars_mult());
    }

    return node;
}

node_t *pars_mult()
{
    node_t *node = pars_power();

    while (tkns->data_type == COMMAND && (tkns->data.command == STAR || tkns->data.command == SLASH))
    {
        unsigned char command = tkns->data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 2, node, pars_power());
    }

    return node;
}

node_t *pars_power()
{
    node_t *node = pars_number();

    while (tkns->data_type == COMMAND && tkns->data.command == POW)
    {
        unsigned char command = tkns->data.command;
        TOK_SHIFT();
        node = create_node(command, OP, 2, node, pars_number());
    }

    return node;
}

node_t *pars_number()
{
    node_t *node = NULL;

    /*if (token_arr[*pos].data_type == command)
        node = write_command(token_arr, pos);
    else */
    if (tkns->data_type == COMMAND && tkns->data.command == OP_BR)
    {
        TOK_SHIFT();
        node = pars_E();
        if (tkns->data.command != CL_BR)
        {
            LOG(">>> closing bracket wasn't found, character is: %#04x%40s\n", tkns->data.command, "[error]");                    
            return NULL;
        }
        else
        {
            TOK_SHIFT();
        }
    }
    else if (tkns->data_type == NUMBER)
    {
        LOG("> number found\n");
        node = create_node(tkns->data.number, NUM, 0);
        TOK_SHIFT();
    }
    else if (tkns->data_type == YET_TO_DET)
    {
        LOG("> some string founded\n");
        node = pars_name();
        node->data_type = VAR;
    }
    else
        LOG(">>> syntax error, parser couldn't parse a number%40s\n", "[error]");

    return node;
}