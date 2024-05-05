#include "../../general/graph_creator/create_graph.h"
#include "include/read_tree.h"
#include "include/bin_2_n.h"
#include "include/translator.h"

_INIT_LOG();

int main(int argc, char const *argv[])
{
    if (argc != 3 || !argv[argc - 2] || !argv[argc - 1])
    {
        printf("[error]>>> not enough arguments in programm call\n");
        return ERR;
    }
    _CLEAR_LOGS();
    _OPEN_LOG("logs/main.log");

    LOG("> reading tree\n");
    node_t *root = NULL;
    int error = read_tree(&root, argv[argc - 1]);
    if (!root || error)
    {
        LOG("[error]>>> error in tree reading\n");
        kill_tree(root, KILL_STRINGS);
        _CLOSE_LOG();
        return error;
    }

    LOG("> converting tree to n-ary\n");
    root = convert_tree(root);
    
    LOG("> creating graph\n");
    error = create_gparh_code(root);
    if (error)
    {
        LOG("[error]>>> error in graphcode creation\n");
        kill_tree(root, KILL_STRINGS);
        _CLOSE_LOG();
        return error;
    }
    
    error = calc_consts(root);
    if (error)
        LOG("[error]>>> error in constants calculation\n");

    LOG("> starting translation:\n");
    error = translator(root);

    kill_tree(root, KILL_STRINGS);
    _CLOSE_LOG();
    return error;
}
