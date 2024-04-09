#include <stdio.h>
#include <stdlib.h>

/*
Язык имеет следующую грамматику:
code            ::= {variable | func}+$
variable        ::= name ['=' E]
func            ::= name args '{'STR+'}'
STR             ::= {variable | return | E | IF | ELSE | FOR | WHILE} "эу"
IF              ::= "if"'('{{{[variable] [logial] [E]}}}')'['{'STR*'}']
ELSE            ::= "else"
WHILE           ::= "while"'('variable [logial E]')'['{'STR*'}']
FOR             ::= "for" '('variable 'эу' {{{variable logical E}}} 'эу' E ')'
args            ::= '('{name}*')'
return          ::= "return" E
name            ::= ['а'-'я']+

где E - это стандартный парсинг выражений, с добавление вызовов функий в конце и логическими операторами
E               ::= M{[+, -]M}*
M               ::= P{[*, /]P}*
P               ::= U{^U}*
U               ::= ['1'-'9']+ | variable
*/


