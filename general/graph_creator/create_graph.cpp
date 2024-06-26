#include "create_graph.h"

_INIT_LOG();

int create_gparh_code(node_t *node) //передавать название фотографии через аргумент и объеденять через snprintf
{
    assert(node);
    _OPEN_LOG("logs/graph_creation.log");

    FILE *gcode = fopen("graph/graphcode.txt", "wb");
    if (!gcode)
    {
        LOG(">>> graphcode file couldn't open%40s\n", "[error]");
        _CLOSE_LOG();
        return ERR;
    }
    setbuf(gcode, NULL);

    fprintf(gcode, GPRAPH_CODE_START);
    go_through_tree(gcode, node);
    fprintf(gcode, "}\n");
    LOG(">> tree was read succes fully\n");

    fclose(gcode);
    print_png();

    _CLOSE_LOG();
    return NO_ERR;
}

void print_png()
{
    static int png_number = 0;
    char png_call[200] = {0};

    snprintf(png_call, sizeof(png_call), "%s%s", DOT_CALL, "graph/syntax_tree.png");

    png_number++;
    system(png_call);

    return;
}

void clear_all_png()
{
    system("rm -f syntax_tree.png");
}

int go_through_tree(FILE *gcode, node_t *node)
{
    assert(node);

    switch (node->data_type)
    {
    case NUM:
        fprintf(gcode, GRAPH_NUMBER_NODE);
        break;
    case OP:
        fprintf(gcode, GRAPH_OP_NODE);
        break;
    case VAR:
        fprintf(gcode, GRAPH_VAR_NODE);
        break;
    case LINKER:
        fprintf(gcode, GRAPH_CONN_NODE);
        break;
    case FUNC:
        fprintf(gcode, GRAPH_FUNC_NAME_NODE);
        break;
    case MAIN:
        fprintf(gcode, GRAPH_MAIN_NODE);
        break;

        
    default:
        LOG(">>> fatal error: couldn't create a graph code of that type%40s\n", "[error]");
        return ERR;
    }
    
    if (!node->branch_number)
    {
        LOG("> the end of the tree found\n");
        return NO_ERR;
    }
    
    for (int i = 0; i < node->branch_number; i++)
    {
        fprintf(gcode, "\tnode_num%p -> node_num%p;\n", node, node->branches[i]);
        if (node->branches[i])
            go_through_tree(gcode, node->branches[i]);
        LOG("> branch found with the pointer: %p\n", node->branches[i]);
    }
    
    return NO_ERR;
}