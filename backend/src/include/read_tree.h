#include <stdio.h>
#include <stdlib.h>
#include "../../../general/n_tree_func/n_node.h"
#include "../../../../common/logging/log.h"
#include "../../../../common/file_reading/src/file_reading.h"
#include "../../../general/commands/commands.h"

#ifndef TREE_READ
#define TREE_READ

/*struct tree_file
{
    char *buff;

};*/

enum TREE_READ_ERRS
{
    OP_BR_NOT_FOUND_ERR = 1616,
    CL_BR_NOT_FOUND_ERR,
    TREE_STRUCTURE_ERR,
    OP_TYPE_NOT_REC_ERR,
    DATA_READ_ERR,

};

int read_tree(node_t **root_ptr, const char *tree_file_name);

#endif