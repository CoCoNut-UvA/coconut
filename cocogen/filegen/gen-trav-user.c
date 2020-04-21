#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-ast.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

void gen_trav_user(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    out("#include \"generated/trav.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    out_typedef_struct("INFO");
    out_typedef_struct_end("Info");

    out_start_func("Info *%s_create_info()", travlwr);
    out_field("Info *info = mem_alloc(sizeof(Info))");
    out_field("return info");
    out_end_func();

    out_start_func("Node *%s_start(Node* syntaxtree)", travlwr);
    out_field("Info *arg_info = %s_create_info()", travlwr);
    out_field("trav_push(" TRAV_ENUM_PREFIX "%s)", travlwr);
    out_field("syntaxtree = traverse(syntaxtree, arg_info)");
    out_field("trav_pop()");
    /// TODO: free info
    out_end_func();

    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);
        out_start_func("Node *%s_%s(Node *arg_node, Info *arg_info)", travlwr,
                       nodelwr);
        for (int i = 0; i < array_size(node->children); i++) {
            Child *child = array_get(node->children, i);
            char *childupr = strupr(child->id);
            out_field("%s_%s(arg_node) = traverse(%s_%s(arg_node), arg_info)",
                      nodeupr, childupr, nodeupr, childupr);
            free(childupr);
        }
        out_field("return arg_node");
        out_end_func();
        free(nodelwr);
        free(nodeupr);
    }
    free(travlwr);
}