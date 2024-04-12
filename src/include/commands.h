#ifndef COMMANDS
#define COMMANDS

#define COMMANDS_NUMBER 21

enum COMMANDS
{
    PLUS    = 0x21,
    MINUS   = 0x22,
    STAR    = 0x23,
    SLASH   = 0x24,
    POW     = 0x25,

    E       = 0x41,

    OP_BR   = 0x01,
    CL_BR   = 0x02,    
    STR_END = 0x03,

    BODY    = 0x0F,

    LOG_E,
    LOG_NE,
    LOG_A,
    LOG_AE,
    LOG_B,
    LOG_BE,
    

    RET,
    PTR_VAL,


    OP_F_BR,
    CL_F_BR,
    ZAP,
    INCL,
};

typedef struct
{
    unsigned char cmd;
    const char *cmd_name;
    int cmd_length;
} command_t;

extern command_t commands[COMMANDS_NUMBER];


#endif