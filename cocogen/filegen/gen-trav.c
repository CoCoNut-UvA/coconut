#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"
#include "lib/smap.h"

static int indent = 0;

// Map from node name to index in reachability matrix
static smap_t *node_index = NULL;

// Node reachability matrix
static bool **node_reachability = NULL;

static bool **pass_nodes = NULL;

#define ERROR_HEADER "traversal-driver"

static void compute_reachable_nodes(Config *config) {
    if (node_reachability && pass_nodes) {
        return;
    }
    node_index = smap_init(32);

    size_t num_nodes = array_size(config->nodes);
    size_t num_nodesets = array_size(config->nodesets);

    // Add nodes to node_index map
    for (int i = 0; i < num_nodes; i++) {
        Node *node = array_get(config->nodes, i);
        int *index = mem_alloc(sizeof(int));
        *index = i;
        smap_insert(node_index, node->id, index);
    }

    for (int i = 0; i < num_nodesets; i++) {
        Nodeset *nodeset = array_get(config->nodesets, i);
        int *index = mem_alloc(sizeof(int));
        *index = i + num_nodes;
        smap_insert(node_index, nodeset->id, index);
    }

    // Allocate node reachability matrix

    size_t num_total = num_nodes + num_nodesets;

    node_reachability = mem_alloc(sizeof(bool *) * num_total);

    for (int i = 0; i < num_total; i++) {
        node_reachability[i] = mem_alloc(sizeof(bool) * num_total);
        memset(node_reachability[i], 0, sizeof(bool) * num_total);
    }

    // Initialise reachability matrix with adjacency matrix

    for (int i = 0; i < num_nodes; i++) {
        Node *node = array_get(config->nodes, i);

        for (int j = 0; j < array_size(node->children); j++) {
            Child *child = array_get(node->children, j);

            int *index = smap_retrieve(node_index, child->type);
            node_reachability[*index][i] = true;
        }
    }

    for (int i = 0; i < num_nodesets; i++) {
        Nodeset *nodeset = array_get(config->nodesets, i);

        for (int j = 0; j < array_size(nodeset->nodes); j++) {
            Node *node = array_get(nodeset->nodes, j);
            int *index = smap_retrieve(node_index, node->id);
            node_reachability[*index][num_nodes + i] = true;
        }
    }

    // Compute reachability of nodes using the Floyd-Warshall algorithm
    for (int k = 0; k < num_total; k++) {
        for (int i = 0; i < num_total; i++) {
            for (int j = 0; j < num_total; j++) {

                if (node_reachability[k][i] && node_reachability[j][k])
                    node_reachability[j][i] = true;
            }
        }
    }

    // Fill pass_nodes table

    size_t num_traversals = array_size(config->traversals);

    pass_nodes = mem_alloc(sizeof(bool *) * num_traversals);

    for (int i = 0; i < num_traversals; i++) {
        Traversal *trav = array_get(config->traversals, i);

        pass_nodes[i] = mem_alloc(sizeof(bool) * num_total);

        // Traversal handles all nodes
        if (trav->nodes == NULL) {
            for (int j = 0; j < num_total; j++) {
                pass_nodes[i][j] = true;
            }
        } else {
            memset(pass_nodes[i], 0, sizeof(bool) * num_total);

            for (int j = 0; j < array_size(trav->nodes); j++) {
                Node *trav_node = array_get(trav->nodes, j);
                int *index = smap_retrieve(node_index, trav_node->id);

                // List of nodes from where the handled node can be reached
                bool *reach_nodes = node_reachability[*index];

                // Add the handled node itself to the list of nodes that need
                // to be traversed
                pass_nodes[i][*index] = true;

                // Add the nodes in reach_node
                for (int k = 0; k < num_total; k++) {
                    if (reach_nodes[k])
                        pass_nodes[i][k] = true;
                }
            }
        }
    }
}

void gen_trav_header(Config *config, FILE *fp) {
    compute_reachable_nodes(config);
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("#include \"generated/ast.h\"\n");
    out("#include \"generated/free.h\"\n");
    out("#include \"lib/array.h\"\n");
    out("\n");
    out_comment("Traversal functions");
    out_field("Node *traverse(Node *arg_node)");
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *" TRAV_PREFIX "%s(Node *arg_node)", nodelwr);
        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            if (child->node != NULL) {
                continue;
            }
            char *childlwr = strlwr(child->id);

            out_field("Node *" TRAV_PREFIX "%s_%s(Node *arg_node)", nodelwr,
                      childlwr);
            free(childlwr);
        }
        free(nodelwr);
    }
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        char *travupr = strupr(trav->id);
        out_comment("Traversal %s", trav->id);
        out_field("Node *%s_start(Node* syntaxtree)", travlwr);
        for (int i = 0; i < array_size(trav->nodes); i++) {
            Node *node = array_get(trav->nodes, i);
            char *nodelwr = strlwr(node->id);
            out_field("Node *%s_%s(Node *arg_node)", travlwr, nodelwr);
            free(nodelwr);
        }
        out("\n");
        free(travlwr);
        free(travupr);
    }
    out("#endif /* _CCN_TRAV_H_ */\n");
}

// Main Traversal Function
static void gen_trav_func(Config *config, FILE *fp) {
    out_start_func("Node *traverse(Node *arg_node)");
    out_begin_if("!arg_node");
    out_field("return arg_node");
    out_end_if();
    out_begin_switch("NODE_TYPE(arg_node)");
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_begin_case(NT_FORMAT, nodelwr);
        out_field("arg_node = " TRAV_PREFIX "%s(arg_node)", nodelwr);
        out_field("break");
        out_end_case();
        free(nodelwr);
    }
    out_end_switch();
    out_field("return arg_node");
    out_end_func();
}

// Traversal function for nodesets
static void gen_trav_nodeset(Config *config, FILE *fp, Node *node,
                             Child *child) {
    Nodeset *nodeset = child->nodeset;

    char *childlwr = strlwr(child->id);
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    char *childupr = strupr(child->id);
    char *nodesetlwr = strlwr(nodeset->id);

    out_start_func("Node *" TRAV_PREFIX "%s_%s(Node *arg_node)", nodelwr,
                   childlwr);
    out_begin_if("!arg_node");
    out_field("return arg_node");
    out_end_if();

    out_begin_if("!%s_%s(arg_node)", nodeupr, childupr);
    out_field("return arg_node");
    out_end_if();

    out_begin_switch("NODE_TYPE(%s_%s(arg_node))", nodeupr, childupr);
    for (int i = 0; i < array_size(nodeset->nodes); ++i) {
        Node *cnode = (Node *)array_get(nodeset->nodes, i);
        char *cnodelwr = strlwr(cnode->id);
        out_begin_case(NT_FORMAT, cnodelwr);
        out_field("arg_node = " TRAV_PREFIX "%s(%s_%s(arg_node))", cnodelwr,
                  nodeupr, childupr);
        out_field("break");
        out_end_case();
        free(cnodelwr);
    }
    out_end_switch();
    out_field("return arg_node");

    out_end_func();

    free(nodeupr);
    free(childupr);
    free(nodesetlwr);
    free(nodelwr);
    free(childlwr);
}

// Traversal function for nodes
static void gen_trav_node(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_start_func("Node *" TRAV_PREFIX "%s(Node *arg_node)", nodelwr);
    out_begin_if("!arg_node");
    out_field("return arg_node");
    out_end_if();
    out_begin_switch(TRAV_PREFIX "current()");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out_begin_case(TRAV_FORMAT, travlwr);
        if (is_traversal_node(config, trav, node)) {
            out_field("arg_node = " TRAVERSAL_HANDLER_FORMAT "(arg_node)",
                      travlwr, nodelwr);
        } else {
            for (int j = 0; j < array_size(node->children); j++) {
                Child *child = array_get(node->children, j);
                char *childlwr = strlwr(child->id);
                char *ctypelwr = strlwr(child->type);
                char *childupr = strupr(child->id);

                int *index = smap_retrieve(node_index, child->type);
                bool is_pass_node = pass_nodes[i][*index];
                if (is_pass_node) {
                    if (child->nodeset == NULL) {
                        out_field("arg_node = " TRAV_PREFIX
                                  "%s(%s_%s(arg_node))",
                                  ctypelwr, nodeupr, childupr);
                    } else {
                        out_field("arg_node = " TRAV_PREFIX "%s_%s(arg_node)",
                                  nodelwr, childlwr);
                    }
                }
                free(ctypelwr);
                free(childupr);
                free(childlwr);
            }
        }
        out_field("break");
        out_end_case();
        free(travlwr);
    }
    out_begin_case(TRAV_FORMAT, "free");
    out_field("arg_node = free_%s(arg_node)", nodelwr);
    out_field("break");
    out_end_case();
    out_begin_case(TRAV_FORMAT, "copy");
    out_field("arg_node = copy_%s(arg_node)", nodelwr);
    out_field("break");
    out_end_case();
    out_begin_default_case();
    out_field("break");
    out_end_case();
    out_end_switch();
    out_field("return arg_node");
    out_end_func();

    free(nodeupr);
    free(nodelwr);
}

void gen_trav_src(Config *config, FILE *fp) {
    compute_reachable_nodes(config);

    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"lib/array.h\"\n");
    out("#include \"generated/trav.h\"\n");
    out("#include \"generated/enum.h\"\n");
    out("#include \"core/copy_core.h\"\n");
    out("#include \"core/free_core.h\"\n");
    out("\n");
    gen_trav_func(config, fp);
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        gen_trav_node(config, fp, node);
        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            if (child->node != NULL) {
                continue;
            }
            gen_trav_nodeset(config, fp, node, child);
        }
    }
}
