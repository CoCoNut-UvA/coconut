#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-ast.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

void gen_trav_user(Config *config, FILE *fp, Traversal *trav) {
    out("#include \"generated/trav.h\"\n");
    out("\n");
    char *travlwr = strlwr(trav->id);
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);
        out_start_func("Node *%s_%s(Node *arg_node, Info *arg_info)", travlwr,
                       nodelwr);
        for (int i = 0; i < array_size(node->children); i++) {
            Child *child = array_get(node->children, i);
            char *childupr = strupr(child->id);
            out_field("arg_node = traverse(%s_%s(arg_node), arg_info)", nodeupr,
                      childupr);
            free(childupr);
        }
        out_field("return arg_node");
        out_end_func();
        free(nodelwr);
        free(nodeupr);
    }
    free(travlwr);
}