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

void gen_trav_function_declarations(Config *config, FILE *fp, Traversal *trav) {

    if (trav->nodes != NULL) {
        for (int i = 0; i < array_size(trav->nodes); i++) {
            char *node_id = array_get(trav->nodes, i);
            char *nodelwr = strlwr(node_id);
            out("Node *%s_%s(Node *arg_node, Info *arg_info);\n", nodelwr,
                nodelwr);
        }
    } else {
        for (int i = 0; i < array_size(config->nodes); i++) {
            Node *node = array_get(config->nodes, i);
            char *nodelwr = strlwr(node->id);
            out("Node *%s_%s(Node *arg_node, Info *arg_info);\n", nodelwr,
                nodelwr);
            free(nodelwr);
        }
    }
    out("\n");
}

/**
 * This this function goes over each node in the traversal and compares it to
 * the node given to the function. If the two nodes have an equal ID, then the
 * node is a valid node in the traversal.
 */
bool node_in_traversal(Config *config, FILE *fp, Traversal *trav, Node *node) {
    for (int i = 0; i < array_size(trav->nodes); i++) {
        char *travnode_id = array_get(trav->nodes, i);
        if (strcmp(travnode_id, node->id) == 0) {
            return true;
        }
    }
    return false;
}

void generate_trav_tables(Config *config, FILE *fp) {
    out("TravTables travtables = {\n");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("    // Traversal TRAV_%s for traversal %s\n", trav->prefix,
            trav->id);
        out("    {\n");
        if (trav->nodes != NULL) {
            for (int i = 0; i < array_size(config->nodes); i++) {
                Node *node = array_get(config->nodes, i);
                char *nodelwr = strlwr(node->id);
                if (node_in_traversal(config, fp, trav, node)) {
                    out("        &%s_%s,\n", travlwr, nodelwr);
                } else {
                    out("        &traverse_children,\n");
                }
                free(nodelwr);
            }
        } else {
            for (int i = 0; i < array_size(config->nodes); i++) {
                Node *node = array_get(config->nodes, i);
                char *nodelwr = strlwr(node->id);
                out("        &%s_%s,\n", travlwr, nodelwr);
                free(nodelwr);
            }
        }
        out("    },\n\n");
        free(travlwr);
    }
    out("}\n\n");
}

void gen_trav_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("\n");
    out("Node *traverse_children(Node *arg_node, Info *arg_info);\n\n");

    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_function_declarations(config, fp, trav);
    }

    generate_trav_tables(config, fp);

    out("#endif /* _CCN_TRAV_H_ */\n");
}

/**
 * General traversal function that traverses each child of a given node. This
 * method is probably not optimal, TODO: find a better method.
 */
void generate_traverse_children(Config *config, FILE *fp) {
    out("Node *traverse_children(Node *arg_node, Info *arg_info) {\n");
    out("    switch (NODE_TYPE(arg_node)) {\n");
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);
        out("    case NT_%s:\n", nodelwr);
        for (int i = 0; i < array_size(node->children); i++) {
            Child *child = array_get(node->children, i);
            char *childupr = strupr(child->id);
            out("        TRAV(%s_%s(arg_node), arg_info);\n", nodeupr,
                childupr);

            free(childupr);
        }
        out("        break;\n");
        free(nodelwr);
        free(nodeupr);
    }
    out("    default:\n");
    /// TODO: Do some error handling here
    out("        break;\n");
    out("    }\n");
    out("}\n\n");
}

void gen_trav_src(Config *config, FILE *fp) {
    out("#include \"generated/trav.h\"\n");
    out("\n");
    generate_traverse_children(config, fp);
}
