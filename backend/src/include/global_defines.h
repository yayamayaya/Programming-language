#ifndef GLOBAL_DEFS
#define GLOBAL_DEFS

#include "DSL_codegen.h"

#define _CHECK_NODE_NUM(num)                                                \
    if (node->branch_number != num)                                         \
    {                                                                       \
        LOG("[error]>>> number of nodes does not match needed\n");          \
        return ERR;                                                         \
    } do {} while (0)
    
#endif