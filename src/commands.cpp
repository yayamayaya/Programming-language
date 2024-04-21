#include "include/commands.h"

command_t commands[COMMANDS_NUMBER] = 
{
    {E, "=", 1},
    {VAR_END, "brat", 4},
    {ZAP, ",", 1},

    {PLUS, "+", 1},
    {MINUS, "-", 1},
    {STAR, "*", 1},
    {SLASH, "/", 1},
    {POW, "^", 1},
    {LOG_E, "loge", 4},
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

    {IF, "if", 2},
    {WHILE, "while", 5},
    {RET, "sygeide", 7},
    {PRINT, "print", 5},
};
    //{INCL, "include", 7},