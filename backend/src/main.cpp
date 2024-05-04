#include "../../general/graph_creator/create_graph.h"
#include "include/read_tree.h"
#include "include/bin_2_n.h"

_INIT_LOG();

int main(int argc, char const *argv[])
{
    if (argc != 3 || !argv[argc - 2] || !argv[argc - 1])
    {
        printf("[error]>>> not enough arguments in programm call\n");
        return ERR;
    }
    _OPEN_LOG("logs/main.log");

    LOG("> reading tree\n");
    node_t *root = NULL;
    int error = read_tree(&root, argv[argc - 1]);
    if (!root || error)
    {
        LOG("[error]>>> error in tree reading\n");
        kill_tree(root);
        _CLOSE_LOG();
        return error;
    }

    LOG("> converting tree to n-ary\n");
    root = convert_tree(root);
    
    LOG("> creating graph\n");
    error = create_gparh_code(root);

    kill_tree(root);
    _CLOSE_LOG();
    return error;
}
