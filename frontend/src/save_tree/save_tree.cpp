#include "../include/save_tree.h"

void write_tree(FILE *file, node_t *root);

int save_tree(const char *file_name, node_t *root)
{
    assert(file_name);
    assert(root);

    FILE *tree_file = fopen(file_name, "wb");
    if (!tree_file)
    {
        printf("[error]>>> couldn't open save file\n");
        return 1;
    }
    setbuf(tree_file, NULL);
    
    write_tree(tree_file, root);

    fclose(tree_file);
    return 0;
}

void write_tree(FILE *file, node_t *node)
{
    assert(file);
    if (!node)
    {
        fprintf(file, "{nil}");
        return;
    }
    
    fprintf(file, "{");
    fprintf(file, "#%d#", node->data_type);

    switch (node->data_type)
    {
    case VAR:
    case FUNC:
        fprintf(file, "#%s#", node->data.string);
        break;
    
    case NUM:
        fprintf(file, "#%lf#", node->data.number);
        break;

    case OP:
    case CONN:
        fprintf(file, "#%d#", node->data.command);
        break;

    default:
        printf("[error]>>> fatal error happened during tree saving\n");
        break;
    }

    if (node->branch_number > 2)
    {
        for (int i = 0; i < node->branch_number; i++)
        {
            write_tree(file, node->branches[i]);
            if (i != node->branch_number - 1)
                fprintf(file, "{#%d##%d#", node->data_type, node->data.command);
        }
        fprintf(file, "{nil}");
        for (int i = 0; i < node->branch_number; i++)
            fprintf(file, "}");
        
        return;
    }
    else if (node->branch_number == 2)
    {
        write_tree(file, node->branches[L]);
        write_tree(file, node->branches[R]);
    }
    else if (node->branch_number == 1)
    {
        write_tree(file, node->branches[L]);
        write_tree(file, NULL);
    }
    else if (!node->branch_number)
    {
        write_tree(file, NULL);
        write_tree(file, NULL);
    }

    fprintf(file, "}");

    return;
}