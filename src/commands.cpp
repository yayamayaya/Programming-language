#include "include/commands.h"

command_t commands[COMMANDS_NUMBER] = 
{
    {E, "bolad", 5},
    {VAR_END, "brat", 4},
    {ZAP, "men", 3},

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
    {STR_END, "eu,", 3},

    {OP_BR, "pavlodar", 8},
    {CL_BR, "kalasy", 6},
    {OP_F_BR, "kraba", 5},
    {CL_F_BR, "nakin'", 6},

    {IF, "eitkezinba", 11},
    {WHILE, "shenber", 7},
    {RET, "sygeide", 7},
    {PRINT, "korsetsh", 8},
};
    //{INCL, "include", 7},