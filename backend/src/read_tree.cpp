#include "include/read_tree.h"

_INIT_LOG();

int read_node(node_t **node, const char *buff);
int read_data(data_t *data, const char *buff, int *pos, int data_type);

int read_tree(node_t **root_ptr, const char *tree_file_name)
{
    assert(tree_file_name);
    _OPEN_LOG("logs/tree_reading.log");
    LOG("> reading a tree from file\n");
    
    char *buff = NULL;
    int file_size = 0;
    int error = file_read(&buff, &file_size, tree_file_name);
    if (error)
    {
        LOG("[error]>>> file reading error\n");
        free(buff);
        _CLOSE_LOG();
        return error;
    }

    error = read_node(root_ptr, buff);

    _CLOSE_LOG();
    free(buff);

    return error;
}

int read_node(node_t **node, const char *buff)
{
    assert(buff);

    static int pos = 0;
    int n = 0;
    int data_type = 0;

    LOG("> reading node:\n");

    if (buff[pos] != '{')
    {
        LOG("[error]>>> opening bracket wasn't found\n");
        return OP_BR_NOT_FOUND_ERR;
    }
    pos++;
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

    sscanf(buff + pos, "nil%n", &n);
    if (n)
    {
        LOG("> NULL branch was found, returning NULL\n");
        pos += n;
        n = 0;
        LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

        if (buff[pos] != '}')
        {
            LOG("[error]>>> closing bracket wasn't found in the nil node\n");
            return CL_BR_NOT_FOUND_ERR;
        }
        pos++;
        LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);
        
        return NO_ERR;
    }

    int sc = sscanf(buff + pos, "#%d#%n", &data_type, &n);
    LOG("> variables scanned is %d\n", sc);
    if (!n)
    {
        LOG("[error]>>> tree file structure error, returning NULL\n");
        return TREE_STRUCTURE_ERR;
    }
    pos += n;
    n = 0;
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

    data_t data = {};
    int error = read_data(&data, buff, &pos, data_type);
    if (error)
    {
        if (data_type == VAR || data_type == FUNC)
            free((void *)data.string);
        
        return error;
    }

    node_t *l = NULL;
    node_t *r = NULL;

    error = read_node(&l, buff);
    if (error)
        return error;
    error = read_node(&r, buff);
    if (error)
        return error;

    node_t *new_node = create_node(data, data_type, 0);
    _ADD_B(new_node, l);
    _ADD_B(new_node, r);
    
    if (buff[pos] != '}')
    {
        LOG("[error]>>> closing bracket wasn't found in the nil node\n");
        kill_tree(new_node, KILL_STRINGS);
        return CL_BR_NOT_FOUND_ERR;
    }
    pos++;
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

    *node = new_node;

    return NO_ERR;
}

int read_data(data_t *data_ptr, const char *buff, int *pos, int data_type)
{
    data_t data = {};
    int n = 0;

    LOG("> reading data\n");
    switch (data_type)
    {

    case NUM:
        LOG("> scanning number\n");
        sscanf(buff + *pos, "#%lf#%n", &data.number, &n);
        if (!n)
            break;

        LOG("> number %.2lf scanned\n", data.number);
        *pos += n;
        break;

    case VAR:
    case FUNC:
    {
        LOG("> scanning a string\n");
        char *string = (char *)calloc(100, sizeof(char));
        sscanf(buff + *pos, "#%[^#]#%n", string, &n);
        if (!n)
        {
            free(string);
            break;
        }
        
        data.string = string;
        *pos += n;

        LOG("> string \"%s\" scanned\n", data.string);
        break;
    }

    case MAIN:
    {
        int cmd = 0;
        sscanf(buff + *pos, "#%d#%n", &cmd, &n);
        if (!n)
            break;

        char *string = (char *)calloc(5, sizeof(char));
        sprintf(string, "main");
        data.string = string;
        *pos += n;

        LOG("> main scanned\n");
        break;
    }

    case OP:
    case LINKER:
    {
        LOG("scanning a command\n");
        int cmd = 0;
        sscanf(buff + *pos, "#%d#%n", &cmd, &n);
        if (!n)
            break;

        data.command = (unsigned char)cmd;
        *pos += n;
        LOG("> command %d scanned\n", data.command);
        break;
    }

    default:
        LOG("[error]>>> fatal error, operation type was not recognised\n");
        return OP_TYPE_NOT_REC_ERR;

    }

    if (!n)
    {
        LOG("[error]>>> error while reading data, returning NULL\n");
        return DATA_READ_ERR;
    }
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", *pos, buff + *pos);

    *data_ptr = data;

    return NO_ERR;
}