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
*/


node_t *pars_STR();
node_t *pars_body();
node_t *pars_if();
node_t *pars_E();
node_t *pars_mult();
node_t *pars_power();
node_t *pars_number();
node_t *pars_variable();
node_t *pars_var_op();
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
        kill_tree(root);
        return NULL;
    }

    return root;
}

node_t *pars_STR()
{
    node_t *node = pars_if();
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
        LOG("> body don't have any strings, returning NULL");
        kill_tree(body);
        return NULL;
    }
    
    LOG("body created with: %d branches\n", body->branch_number);
    return body;
}

node_t *pars_if()
{
    if (tkns->data_type != OP || tkns->data.command != IF)
    {
        LOG("> if operator wasn't found...\n");
        return NULL;
    }
    TOK_SHIFT(); 

    node_t *node = create_node((unsigned char)IF, OP, 0);
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

    if (tkns->data_type == OP && tkns->data.command == OP_F_BR)
    {
        LOG("> opening figure bracket was found\n");
        TOK_SHIFT();
        while (tkns->data_type != OP || tkns->data.command != CL_F_BR)  
            _ADD_B(node, pars_body());

        TOK_SHIFT();
        LOG("> the insights of figure brackets were parsed, closing figure braclet was found\n");
    }
    else
    {
        LOG("> figure bracket wasn't found, parsing a string\n");
        _ADD_B(node, pars_STR());
    }
    
    return node;
}

node_t *pars_variable()
{
    LOG("> creating an variable:\n");
    node_t *node        = pars_name();
    if (!node)
        return NULL;

    node->data_type = VAR;   

    LOG("> checking for the operator:\n");
    if (tkns->data_type == COMMAND)
    {
        node_t *op          = pars_var_op();        
        if (op)
        {
            LOG("operator found\n");
            _ADD_B(op, node);
            _ADD_B(op, pars_E());
            
            node = op;
            LOG("> variable was parsed with operator\n");
        }
    }

    if ((tkns->data_type != COMMAND) || (tkns->data.command != STR_END))
    {
        LOG(">>> syntax error%40s\n", "[error]");
        if (node)
        {
            kill_tree(node);
            return NULL;
        }
    }
    TOK_SHIFT();

    return node;
}

node_t *pars_var_op()
{
    LOG("> searching for variable command:\n");
    if (tkns->data.command & VAR_OP)
    {
        node_t *node = create_node(tkns->data.command, OP, 0);
        TOK_SHIFT();
        return node;
    }
    
    LOG("variable opertion wasn't found\n");
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