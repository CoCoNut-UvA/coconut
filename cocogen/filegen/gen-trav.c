#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"
#include "lib/print.h"
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

    // Add nodes to node_index map
    for (int i = 0; i < num_nodes; i++) {
        Node *node = array_get(config->nodes, i);
        int *index = mem_alloc(sizeof(int));
        *index = i;
        smap_insert(node_index, node->id, index);
    }

    // Allocate node reachability matrix

    node_reachability = mem_alloc(sizeof(bool *) * num_nodes);

    for (int i = 0; i < num_nodes; i++) {
        node_reachability[i] = mem_alloc(sizeof(bool) * num_nodes);
        memset(node_reachability[i], 0, sizeof(bool) * num_nodes);
    }

    // Initialise reachability matrix with adjacency matrix

    for (int i = 0; i < num_nodes; i++) {
        Node *node = array_get(config->nodes, i);

        for (int j = 0; j < array_size(node->children); j++) {
            Child *child = array_get(node->children, j);
            if (!child->node) {
                for (int k = 0; k < array_size(child->nodeset->nodes); k++) {
                    Node *nodesetnode = array_get(child->nodeset->nodes, k);
                    int *index = smap_retrieve(node_index, nodesetnode->id);
                    node_reachability[*index][i] = true;
                }
            } else if (!child->nodeset) {
                int *index = smap_retrieve(node_index, child->type);
                node_reachability[*index][i] = true;
            } else {
                print_user_error(
                    "traversal-driver",
                    "Child is somehow neither a node or a nodeset.");
            }
        }
    }

    // Compute reachability of nodes using the Floyd-Warshall algorithm
    for (int k = 0; k < num_nodes; k++) {
        for (int i = 0; i < num_nodes; i++) {
            for (int j = 0; j < num_nodes; j++) {

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

        pass_nodes[i] = mem_alloc(sizeof(bool) * num_nodes);

        // Traversal handles all nodes
        if (trav->nodes == NULL) {
            for (int j = 0; j < num_nodes; j++) {
                pass_nodes[i][j] = true;
            }
        } else {
            memset(pass_nodes[i], 0, sizeof(bool) * num_nodes);

            for (int j = 0; j < array_size(trav->nodes); j++) {
                Node *trav_node = array_get(trav->nodes, j);
                int *index = smap_retrieve(node_index, trav_node->id);

                // List of nodes from where the handled node can be reached
                bool *reach_nodes = node_reachability[*index];

                // Add the handled node itself to the list of nodes that need
                // to be traversed
                pass_nodes[i][*index] = true;

                // Add the nodes in reach_node
                for (int k = 0; k < num_nodes; k++) {
                    if (reach_nodes[k])
                        pass_nodes[i][k] = true;
                }
            }
        }
    }
}

void gen_trav_struct(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    char *travupr = strupr(trav->id);
    out_comment("Traversal %s Attributes", trav->id);
    out_struct("TRAV_DATA_%s", travupr);
    for (int i = 0; i < array_size(trav->data); ++i) {
        TravData *td = (TravData *)array_get(trav->data, i);
        char *type_str = get_attr_str(config, td->type, td->type_id);
        if (td->type == AT_link_or_enum) {
            out_field("%s *%s", type_str, td->id);
        } else {
            out_field("%s %s", type_str, td->id);
        }
    }
    out_struct_end();
    free(travupr);
}

void gen_trav_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("TRAV_DATA");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(config->traversals, i);
        if (!trav->data) {
            continue;
        }
        char *travupr = strupr(trav->id);
        char *travlwr = strlwr(trav->id);
        out_field("struct TRAV_DATA_%s *TD_%s", travupr, travlwr);
        free(travupr);
        free(travlwr);
    }
    out_struct_end();
}

void gen_trav_macros(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    out_comment("Macros for Traversal %s", trav->id);
    char *travupr = strupr(trav->id);
    char *travlwr = strlwr(trav->id);
    for (int i = 0; i < array_size(trav->data); ++i) {
        TravData *td = (TravData *)array_get(trav->data, i);
        char *attrupr = strupr(td->id);
        out("#define %s_%s (trav_current()->travdata.TD_%s->%s)\n", travupr,
            attrupr, travlwr, td->id);
        free(attrupr);
    }
    out("\n");
    free(travupr);
    free(travlwr);
}

void gen_trav_header(Config *config, FILE *fp) {
    compute_reachable_nodes(config);
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("\n");

    out_comment("Includes for user defined types");
    smap_t *include_names = smap_init(16);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        for (int i = 0; i < array_size(trav->data); i++) {
            TravData *td = array_get(trav->data, i);
            if (td->type == AT_link_or_enum) {
                char *include = td->value.include;
                if (!smap_retrieve(include_names, include)) {
                    smap_insert(include_names, include, td);
                    out("#include \"%s\"\n", include);
                }
            }
        }
    }
    smap_free(include_names);
    out("\n");

    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_struct(config, fp, trav);
    }
    gen_trav_union(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_macros(config, fp, trav);
    }
    out("\n");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        if (!node->children) {
            continue;
        }
        char *nodelwr = strlwr(node->id);
        out_field("Node *trav_%s(Node *arg_node)", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("#endif /* _CCN_TRAV_H_ */\n");
}

void gen_system_trav_array(Config *config, FILE *fp, char *trav) {
    out("{");
    out("&trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        out("&%s_%s, ", trav, nodelwr);
        free(nodelwr);
    }
    out("}, \n");
}

void gen_error_array(Config *config, FILE *fp) {
    out("{");
    out("&trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        out("&trav_error, ");
    }
    out("}, \n");
}

void gen_matrix(Config *config, FILE *fp) {
    out("const TravFunc trav_mat[_TRAV_SIZE][_NT_SIZE] = {");
    gen_error_array(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        out("{");
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("&trav_error, ");
        for (int j = 0; j < array_size(config->nodes); j++) {
            Node *node = array_get(config->nodes, j);
            char *nodelwr = strlwr(node->id);
            if (is_traversal_node(config, trav, node)) {
                out("&%s_%s, ", travlwr, nodelwr);
            } else {
                int *index = smap_retrieve(node_index, node->id);
                bool is_pass_node = pass_nodes[i][*index];
                if (is_pass_node) {
                    out("&trav_%s, ", nodelwr);
                } else {
                    out("&trav_noop, ");
                }
            }
            free(nodelwr);
        }
        free(travlwr);
        out("},\n");
    }
    gen_system_trav_array(config, fp, "free");
    gen_system_trav_array(config, fp, "copy");
    out("};\n\n");
}

void gen_travdata_arrays(Config *config, FILE *fp, char *version) {
    char *verlwr = strlwr(version);
    out("const %sFunc trav_data_%s_array[_TRAV_SIZE] = {", version, verlwr);
    out("&trav_%s_error, ", verlwr);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        if (trav->data) {
            out("&trav_%s_%s, ", verlwr, travlwr);
        } else {
            out("&trav_%s, ", verlwr);
        }
        free(travlwr);
    }
    out("&trav_%s, ", verlwr); // Free
    out("&trav_%s, ", verlwr); // Copy
    out("};\n\n");
    free(verlwr);
}

void gen_trav_node_func(Config *config, FILE *fp, Node *node) {
    if (!node->children) {
        return;
    }
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);

    out_start_func("Node *trav_%s(Node *arg_node)", nodelwr);
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

void lifetimes(Config *config, FILE *fp) {
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        for (int j = 0; j < array_size(node->lifetimes); j++) {
        }
    }
}

void gen_trav_src(Config *config, FILE *fp) {
    compute_reachable_nodes(config);

    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"core/copy_core.h\"\n");
    out("#include \"core/free_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("#include \"user/trav_%s.h\"\n", travlwr);
        free(travlwr);
    }
    out("\n");
    gen_matrix(config, fp);
    gen_travdata_arrays(config, fp, "Init");
    gen_travdata_arrays(config, fp, "Free");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        gen_trav_node_func(config, fp, node);
    }
}
