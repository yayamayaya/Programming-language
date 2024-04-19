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
code        ::= {variable | func}+0
func        ::= name '('args')' '{'body'}'
body        ::= STR+
STR         ::= cond | variable | func_call
cond        ::= '{']variable['}' '('E')' {"if" | "while"}
variable    ::= "eu, " name [E "="]
E           - стандартный парсер выражений из дифф + лог. операции + вызов функций
func_call   ::= name '{'args'}'
args        ::= name [, args]
name        ::= [а-я, А-Я]
*/

node_t *pars_STR();
node_t *pars_body();
node_t *pars_cond();
node_t *pars_E();
node_t *pars_sum();
node_t *pars_mult();
node_t *pars_power();
node_t *pars_number();
node_t *pars_variable();
node_t *pars_name();

token_t *tkns = NULL;

/*node_t *pars_fv();


node_t *create_syntax_tree(token_t *token_arr)
{
    tkns = token_arr;
    node_t *root = create_node((unsigned char)0, BODY, 0);

    while (tkns->data_type != $)
    {
        add_branch(root, pars_fv());
    }
}

node_t *pars_fv()
{
    node_t *name_node = pars_name();

    if (tkns->data_type == COMMAND && tkns->data_type == OP_BR)
    {
        name_node->data_type = FUNC;
        pars_f();
    }
    
}

node_t *pars_args()
{
    if (tkns->data_type == COMMAND && tkns->data_type == OP_BR)
    {
        TOK_SHIFT();
        node_t *args = create_node((unsigned char)0, ARGS, 0);
        if (tkns->data_type == YET_TO_DET)
            do 
            {
                TOK_SHIFT();
                add_branch(args, pars_name());
            } while (tkns->data_type == COMMAND && tkns->data.command == ZAP);

        if (tkns->data_type == COMMAND && tkns->data_type == CL_BR)
        {
            return args;
        }  
    }
    
    return NULL;
}*/

node_t *create_syntax_tree(token_t *token_arr)
{
    tkns = token_arr;
    LOG("-----------------------------------------------\n\n");
    LOG("> creating syntax tree:\n");

    node_t *root = pars_body();
    if (tkns->data_type != $)
    {
        LOG("syntax error, $ wasn't found%40s\n", "[error]");
        if (root)
            kill_tree(root);
        return NULL;
    }

    return root;
}

node_t *pars_STR()
{
    node_t *node = pars_cond();
    if (node)
        return node;
    
    node = pars_variable();
    if (node)
        return node;

    LOG(">>> string wasn't found, returning NULL\n");
    return NULL;
}

node_t *pars_body()
{
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
    
    LOG("body created with: %d branches\n", body->branch_number);
    return body;
}

node_t *pars_cond()
{
    node_t *node = NULL;

    if (tkns->data_type == OP && tkns->data.command == OP_F_BR)
    {
        node = create_node((unsigned char)0, OP, 0);

        LOG("> opening figure bracket was found\n");
        TOK_SHIFT();
        while (tkns->data_type != OP || tkns->data.command != CL_F_BR)  
            _ADD_B(node, pars_body());

        TOK_SHIFT();
        LOG("> the insights of figure brackets were parsed, closing figure bracket was found\n");
    }
    else
    {
        LOG("> figure bracket wasn't found, returning NULL\n");
        return NULL;
    }

    if (tkns->data_type == OP && tkns->data.command == OP_BR)
    {
        LOG("> opening bracket was found\n");
        TOK_SHIFT();
        _ADD_B(node, pars_E());
        if (tkns->data_type == OP && tkns->data.command == CL_BR)
        {
            TOK_SHIFT();
        }
        else
        {
            LOG(">>> syntax error: closing bracket wasn't found%40s\n", "[error]");
            kill_tree(node);
            return NULL;
        }
    }
    else
    {
        LOG(">>> syntax error: opening bracket wasn't found%40s\n", "[error]");
        kill_tree(node);
        return NULL;
    }

    if (tkns->data_type != OP || !(tkns->data.command == IF || tkns->data.command == WHILE))
    {
        LOG(">>> syntax error: if/while operator wasn't found...%40s\n", "[error]");
        kill_tree(node);
        return NULL;
    }
    node->data.command = tkns->data.command;
    TOK_SHIFT(); 
    
    return node;
}

node_t *pars_variable()
{
    LOG("> creating an variable:\n");
    if ((tkns->data_type != COMMAND) || (tkns->data.command != STR_END))
    {
        LOG("string start wasn't found, returning NULL\n");
        return NULL;
    }
    TOK_SHIFT();

    node_t *node        = pars_name();
    if (!node)
        return NULL;

    node->data_type = VAR;   

    if (tkns->data_type == OP && tkns->data.command == VAR_END)
    {
        LOG("a variable without assignment was found\n");
        TOK_SHIFT();
        return node;
    }
    
    LOG("> creating expression and operator:\n");
    node_t *expr = pars_E();

    if (tkns->data_type == OP && tkns->data.command == E)
    {
        LOG("> variable with assignment found\n");
        TOK_SHIFT();
        return create_node((unsigned char)E, OP, 2, node, expr);
    }

    LOG("syntax error, assign operator wasn't found%40s\n", "[error]");
    kill_tree(node);
    kill_tree(expr);
    return NULL;
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