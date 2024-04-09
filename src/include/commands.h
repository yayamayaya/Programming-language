#ifndef COMMANDS
#define COMMANDS

#define COMMANDS_NUMBER 13

typedef struct
{
    unsigned char cmd;
    const char *cmd_name;
    int cmd_length;
} command_t;

extern command_t commands[COMMANDS_NUMBER];


#endif