#include "include/commands.h"

enum COMMANDS
{
    E,
    LOG_E,
    LOG_NE,
    LOG_A,
    LOG_AE,
    LOG_B,
    LOG_BE,

    RET,


    STR_END,
    OP_BR,
    CL_BR,
    OP_F_BR,
    CL_F_BR,
};

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



};