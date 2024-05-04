#include "include/read_tree.h"

_INIT_LOG();

node_t *read_node(const char *buff, int *error);
data_t read_data(const char *buff, int *pos, int data_type, int *error);

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

    node_t *root = read_node(buff, &error);
    *root_ptr = root;

    _CLOSE_LOG();
    free(buff);

    return error;
}

node_t *read_node(const char *buff, int *error)
{
    assert(buff);

    static int pos = 0;
    int n = 0;
    int data_type = 0;

    if (buff[pos] != '{')
    {
        LOG("[error]>>> opening bracket wasn't found\n");
        *error = OP_BR_NOT_FOUND_ERR;
        return NULL;
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
            *error = CL_BR_NOT_FOUND_ERR;
        }
        pos++;
        LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);
        
        return NULL;
    }

    int sc = sscanf(buff + pos, "#%d#%n", &data_type, &n);
    LOG("> variables scanned is %d\n", sc);
    if (!n)
    {
        LOG("[error]>>> tree file structure error, returning NULL\n");
        *error = TREE_STRUCTURE_ERR;
        return NULL;
    }
    pos += n;
    n = 0;
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

    data_t data = read_data(buff, &pos, data_type, error);

    node_t *node = create_node(data, data_type, 0);
    _ADD_B(node, read_node(buff, error));
    _ADD_B(node, read_node(buff, error));

    if (buff[pos] != '}')
    {
        LOG("[error]>>> closing bracket wasn't found in the nil node\n");
        *error = CL_BR_NOT_FOUND_ERR;
        kill_tree(node);
        return NULL;
    }
    pos++;
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", pos, buff + pos);

    return node;
}

data_t read_data(const char *buff, int *pos, int data_type, int *error)
{
    data_t data = {};
    int n = 0;

    switch (data_type)
    {

    case NUM:
        sscanf(buff + *pos, "#%lf#%n", &data.number, &n);
        if (!n)
            break;

        *pos += n;
        break;

    case VAR:
    case FUNC:
    {
        char *string = (char *)calloc(100, sizeof(char));
        sscanf(buff + *pos, "#%[^#]#%n", string, &n);
        if (!n)
        {
            free(string);
            break;
        }
        
        data.string = string;
        *pos += n;
        break;
    }

    case OP:
    case CONN:
    {
        int cmd = 0;
        sscanf(buff + *pos, "#%d#%n", &cmd, &n);
        if (!n)
            break;

        data.command = (unsigned char)cmd;
        *pos += n;
        break;
    }

    default:
        LOG("[error]>>> fatal error, operation type was not recognised\n");
        *error = OP_TYPE_NOT_REC_ERR;
        break;

    }

    if (!n)
    {
        LOG("[error]>>> error while reading data, returning NULL\n");
        *error = DATA_READ_ERR;
        return data;
    }
    LOG("> pos shifted, current pos is %d, the rest is: %s\n", *pos, buff + *pos);

    return data;
}