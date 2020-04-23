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
    out("#include \"generated/globaldata.h\"\n");
    out("\n");
    out_field("extern void *globaldata[_TRAV_SIZE]");

    // Data struct skeleton
    out_struct("%s_DATA", travupr);
    out_struct_end();

    // Data constructor
    out_start_func("%sData *%s_init_data()", trav->id, travlwr);
    out_field("%sData *data = mem_alloc(sizeof(%sData))", trav->id, trav->id);
    out_field("return data");
    out_end_func();

    // Data destructor
    out_start_func("void %s_free_data(%sData *data)", travlwr, trav->id);
    out_field("mem_free(data)");
    out_end_func();

    // Start function
    out_start_func("Node *%s_start(Node* syntaxtree)", travlwr);
    // out_field("%sData *arg_data = %s_init_data()", trav->id, travlwr);
    out_field("trav_push(" TRAV_ENUM_PREFIX "%s)", travlwr);
    out_field("syntaxtree = traverse(syntaxtree)");
    out_field("trav_pop()");
    out_field("return syntaxtree");
    out_end_func();

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