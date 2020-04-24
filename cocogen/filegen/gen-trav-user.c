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
    char *travupr = strupr(trav->id);
    out("#include \"core/trav_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    out_typedef_struct("TRAV_DATA");
    out_comment("Define your data here");
    out_typedef_struct_end("TraversalData");
    out("\n");
    out_start_func("TraversalData *%s_init_data()", travlwr);
    out_field("TraversalData *data = mem_alloc(sizeof(TraversalData))");
    out_comment("Initialise your data here");
    out_field("return data");
    out_end_func();
    out("\n");
    out_start_func("void %s_free_data(TraversalData *data)", travlwr);
    out_comment("Free your data here");
    out_field("mem_free(data)");
    out_end_func();
    out("\n");
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);

        // Traversal Function
        out_start_func("Node *%s_%s(Node *arg_node)", travlwr, nodelwr);
        for (int i = 0; i < array_size(node->children); i++) {
            Child *child = array_get(node->children, i);
            char *childupr = strupr(child->id);
            out_field("%s_%s(arg_node) = traverse(%s_%s(arg_node))", nodeupr,
                      childupr, nodeupr, childupr);
            free(childupr);
        }
        out_field("return arg_node");
        out_end_func();

        free(nodelwr);
        free(nodeupr);
    }
    free(travlwr);
    free(travupr);
}