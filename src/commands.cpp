#include "include/commands.h"

command_t commands[COMMANDS_NUMBER] = 
{
    {E, "=", 1},
    {LOG_E, "==", 2},
    {LOG_NE, "!=", 2},
    {LOG_A, ">", 1},
    {LOG_AE, ">=", 2},
    {LOG_B, "<", 1},
    {LOG_BE, "<=", 2},
    {STR_END, "eu", 2},
    {OP_BR, "(", 1},
    {CL_BR, ")", 1},
    {OP_F_BR, "{", 1},
    {CL_F_BR, "}", 1},
    {RET, "sygeide", 7},
    {PLUS, "+", 1},
    {MINUS, "-", 1},
    {STAR, "*", 1},
    {SLASH, "/", 1},
    {POW, "^", 1},
    {ZAP, ",", 1},
    {PTR_VAL, "Pavlodar,brat", 13},
    {INCL, "include", 7},
    {IF, "if", 2},



};