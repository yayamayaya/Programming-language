#ifndef COMMANDS
#define COMMANDS

#define COMMANDS_NUMBER 24

enum DATA_TYPES
{
    NUM         = 1,
    OP          = 2,
    VAR         = 3,
    FUNC        = 4,
    LINKER      = 10,
    MAIN        = 15,

    //GRPS
    VAR_OP      = 0x40,
    AR_OP       = 0x20,
};

enum COMMANDS
{
    PLUS    = 0x21,
    MINUS   = 0x22,
    STAR    = 0x23,
    SLASH   = 0x24,
    POW     = 0x25,
    LOG_E   = 0x26,
    LOG_NE  = 0x27,
    LOG_A   = 0x28,
    LOG_AE  = 0x29,
    LOG_B   = 0x2A,
    LOG_BE  = 0x2B,    

    ASSIGN  = 0x41,
    VAR_END = 0x42,

    OP_BR   = 0x01,
    CL_BR   = 0x02,
    OP_F_BR = 0x03,
    CL_F_BR = 0x04,        
    STR_END = 0x07,
    ZAP     = 0x08,

    IF      = 0x11,
    WHILE   = 0x12,
    RET     = 0x13,
    PRINT   = 0x14,
    SCAN    = 0x15,

    BODY    = 0x0B,
    EXPR    = 0x0E,
    ARGS    = 0x0A,
    PROGRAMM = 0x0C,
};

typedef struct
{
    unsigned char cmd;
    const char *cmd_name;
    int cmd_length;
} command_t;

extern command_t commands[COMMANDS_NUMBER];

#define _CHECK_FOR_MAIN(arg)                \
    if (!strcmp(arg, "Elbasy"))             \
    {                                       \
        LOG("> main was found\n");          \
        (*main_flag)++;                     \
        kill_tree(func, DONT_KILL_STRS);    \
        func = create_node(0, MAIN, 0);   \
    }
    

#endif