#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lib/array.h"

#include "filegen/driver.h"
#include "filegen/gen-trav.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

void gen_trav_functions(Config *config, FILE *fp, Traversal *trav,
                        bool is_header) {
    out_comment("Traversal functions for %s", trav->id);
    char *travlwr = strlwr(trav->id);
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *%s_%s(Node *arg_node, Info *arg_info)", travlwr,
                  nodelwr);
        free(nodelwr);
    }
    out("\n");
    free(travlwr);
}

void gen_trav_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"../core/ast_core.h\"\n");
    out("#include \"../core/trav_core.h\"\n");
    out("\n");
    out("Node *traverse_children(Node *arg_node, Info *arg_info);\n\n");

    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_functions(config, fp, trav, true);
    }

    out("#endif /* _CCN_TRAV_H_ */\n");
}

/**
 * General traversal function that traverses each child of a given node. This
 * method is probably not optimal, TODO: find a better method.
 */
void gen_traverse_children(Config *config, FILE *fp) {
    out_start_func("Node *traverse_children(Node *arg_node, Info *arg_info)");
    out_begin_switch("NODE_TYPE(arg_node)");
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);
        out_begin_case("NT_%s", nodelwr);
        for (int i = 0; i < array_size(node->children); i++) {
            Child *child = array_get(node->children, i);
            char *childupr = strupr(child->id);
            out_field("TRAV(%s_%s(arg_node), arg_info)", nodeupr, childupr);
            free(childupr);
        }
        out_field("break");
        free(nodelwr);
        free(nodeupr);
        out_end_case();
    }
    out_begin_default_case();
    /// TODO: Do some error handling here
    out_field("break");
    out_end_case();
    out_end_switch();
    out_end_func();
}

void gen_trav_src(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"ast.h\"\n");
    out("#include \"trav.h\"\n");
    out("\n");
    gen_traverse_children(config, fp);
}
