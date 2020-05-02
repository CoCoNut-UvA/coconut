#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "ast/to-string.h"
#include "lib/imap.h"
#include "lib/memory.h"
#include "lib/smap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Map from node name to index in reachability matrix
static smap_t *node_index = NULL;

// Node reachability matrix
static bool **node_reachability = NULL;

static bool **traversal_node_handles = NULL;

#define ERROR_HEADER "traversal-driver"

static void compute_reachable_nodes(Config *config) {
    if (node_reachability && traversal_node_handles) {
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

    // Fill traversal_node_handles table

    size_t num_traversals = array_size(config->traversals);

    traversal_node_handles = mem_alloc(sizeof(bool *) * num_traversals);

    for (int i = 0; i < num_traversals; i++) {
        Traversal *traversal = array_get(config->traversals, i);

        traversal_node_handles[i] = mem_alloc(sizeof(bool) * num_total);

        // Traversal handles all nodes
        if (traversal->nodes == NULL) {
            for (int j = 0; j < num_total; j++) {
                traversal_node_handles[i][j] = true;
            }
        } else {
            memset(traversal_node_handles[i], 0, sizeof(bool) * num_total);

            for (int j = 0; j < array_size(traversal->nodes); j++) {
                Node *node = array_get(traversal->nodes, j);
                int *index = smap_retrieve(node_index, node->id);

                // List of nodes from where the handled node can be reached
                bool *reach_nodes = node_reachability[*index];

                // Add the handled node itself to the list of nodes that need
                // to be traversed
                traversal_node_handles[i][*index] = true;

                // Add the nodes in reach_node
                for (int k = 0; k < num_total; k++) {
                    if (reach_nodes[k])
                        traversal_node_handles[i][k] = true;
                }
            }
        }
    }
}

static void generate_replace_node(Node *node, FILE *fp, bool header) {
    // Generate replace functions
    out("void " REPLACE_NODE_FORMAT "(struct %s *node)", node->id, node->id);
    if (header) {
        out(";\n");
    } else {
        out(" {\n");
        out("    if (node_replacement == NULL) {\n");
        out("        node_replacement_type = " NT_FORMAT ";\n", node->id);
        out("        node_replacement = node;\n");
        out("    } else {\n");
        out("        print_user_error(\"" ERROR_HEADER
            "\", \"" REPLACE_NODE_FORMAT ": "
            "Not making a node replacement, since another replacement "
            "function "
            "was already called previously.\");\n",
            node->id);
        out("    }\n");
        out("}\n");
    }
}

static void generate_start_node(Config *config, FILE *fp, bool header,
                                Node *node) {
    // Generate start functions
    out("void " TRAV_START_FORMAT "(struct %s *node, TraversalType trav)",
        node->id, node->id);
    if (header) {
        out(";\n");
    } else {
        out(" {\n");
        out("    // Inside of the struct Info* is unknown, thus hide "
            "under void.\n");
        out("    void* info;\n");
        out("\n");
        out("    // Set the new traversal as current traversal.\n");
        out("    " TRAV_PREFIX "push(trav);\n");
        out("\n");
        out("    switch(trav) {\n");
        for (int j = 0; j < array_size(config->traversals); ++j) {
            Traversal *trav = (Traversal *)array_get(config->traversals, j);
            out("    case " TRAV_FORMAT ":\n", trav->id);
            out("        info = %s_createinfo();\n", trav->id);
            out("        _" TRAV_PREFIX "%s(node, info);\n", node->id);
            out("        %s_freeinfo(info);\n", trav->id);
            out("        break;\n");
        }
        out("    }\n");
        out("    " TRAV_PREFIX "pop();\n");
        out("}\n");
    }
}

static void generate_node_child_node(Node *node, Child *child, FILE *fp) {
    out("    _" TRAV_PREFIX "%s(node->%s, info);\n", child->type, child->id);

    out("    if (node_replacement != NULL) {\n");
    out("        if (node_replacement_type == " NT_FORMAT ") {\n",
        child->type);
    out("            node->%s = node_replacement;\n", child->id);
    out("        } else {\n");
    out("            print_user_error(\"" ERROR_HEADER
        "\",  \"Replacement node for %s->%s is not of "
        "type "
        "%s.\");\n",
        node->id, child->id, child->type);
    out("        }\n");
    out("    }\n");
}

static void generate_node_child_nodeset(Node *node, Child *child, FILE *fp) {
    out("    if (!node->%s) return;\n", child->id);
    out("    switch (node->%s->type) {\n", child->id);

    Nodeset *nodeset = child->nodeset;

    for (int i = 0; i < array_size(nodeset->nodes); ++i) {
        Node *cnode = (Node *)array_get(nodeset->nodes, i);
        out("    case " NS_FORMAT ":\n", nodeset->id, cnode->id);
        out("        _" TRAV_PREFIX "%s(node->%s->value.val_%s, info);\n",
            cnode->id, child->id, cnode->id);
        out("        break;\n");
    }

    out("    }\n\n");

    out("    if (node_replacement != NULL) {\n");

    out("        switch (node_replacement_type) {\n");
    for (int i = 0; i < array_size(nodeset->nodes); ++i) {
        Node *cnode = (Node *)array_get(nodeset->nodes, i);
        out("        case " NT_FORMAT ":\n", cnode->id);
        out("            node->%s->value.val_%s = node_replacement;\n",
            child->id, cnode->id);
        out("            node->%s->type = " NS_FORMAT ";\n", child->id,
            child->type, cnode->id);
        out("            break;\n");
    }

    out("        default:\n");
    out("            print_user_error(\"" ERROR_HEADER
        "\", \"Replacement node for %s->%s is not a "
        "node type of nodeset %s.\");\n",
        node->id, child->id, child->type);
    out("            break;\n");
    out("        }\n");

    out("    }\n");
}

static void generate_trav_node(Node *node, FILE *fp, Config *config,
                               bool header) {

    if (!header) {
        out("void _" TRAV_PREFIX "%s(struct %s *node, struct Info *info) {\n",
            node->id, node->id);
        out("   if (!node) return;\n");
        out("   switch (" TRAV_PREFIX "current()) {\n");
        for (int i = 0; i < array_size(config->traversals); i++) {
            Traversal *t = array_get(config->traversals, i);

            bool handles_node = t->nodes == NULL;
            for (int j = 0; j < array_size(t->nodes); j++) {
                char *node_name = array_get(t->nodes, j);
                if (strcmp(node->id, node_name) == 0) {
                    handles_node = true;
                    break;
                }
            }

            out("   case " TRAV_FORMAT ":\n", t->id);

            if (handles_node) {
                out("       " TRAVERSAL_HANDLER_FORMAT "(node, info);\n",
                    t->id, node->id);
            } else {

                for (int j = 0; j < array_size(node->children); j++) {
                    Child *c = array_get(node->children, j);

                    int *index = smap_retrieve(node_index, c->type);
                    bool handles_child = traversal_node_handles[i][*index];

                    if (handles_child)
                        out("       " TRAV_PREFIX "%s_%s(node, info);\n",
                            node->id, c->id);
                }
            }
            out("       break;\n");
        }

        out("   default:\n");
        for (int i = 0; i < array_size(node->children); i++) {
            Child *c = array_get(node->children, i);
            out("       " TRAV_PREFIX "%s_%s(node, info);\n", node->id, c->id);
        }
        out("       break;\n");
        out("   }\n");
        out("}\n\n");
    }

    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("void " TRAV_PREFIX "%s_%s(struct %s *node, struct Info *info)",
            node->id, child->id, node->id);

        if (header) {
            out(";\n");
        } else {
            out(" {\n");
            out("    if (!node) return;\n");
            out("    void *orig_node_replacement = node_replacement;\n");
            out("    node_replacement = NULL;\n");

            if (child->node != NULL) {
                // Child is a node
                generate_node_child_node(node, child, fp);
            } else if (child->nodeset != NULL) {
                // Child is a nodeset
                generate_node_child_nodeset(node, child, fp);
            } else {
                // Should not have passed the context analysis.
                assert(0);
            }
            out("    node_replacement = orig_node_replacement;\n");

            out("}\n\n");
        }
    }
}

void generate_trav_header(Config *config, FILE *fp) {
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        out("#include \"generated/trav-%s.h\"\n", node->id);
    }
}

void generate_trav_node_header(Config *config, FILE *fp, Node *node) {
    compute_reachable_nodes(config);
    out("#pragma once\n");
    out("#include \"generated/trav-core.h\"\n");

    out("struct Info;\n");

    generate_trav_node(node, fp, config, true);
    generate_start_node(config, fp, true, node);
    generate_replace_node(node, fp, true);
}

void generate_trav_node_definitions(Config *config, FILE *fp, Node *node) {
    compute_reachable_nodes(config);

    out("#include <stdio.h>\n");
    out("#include \"lib/print.h\"\n");
    out("#include \"generated/trav-%s.h\"\n", node->id);
    out("// generated/trav-core.h is included by my header.\n");

    out("extern " NT_ENUM_NAME " node_replacement_type;\n");
    out("extern void *node_replacement;\n");
    out("\n");

    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);

        if (child->node) {
            out("void _" TRAV_PREFIX
                "%s(struct %s *node, struct Info *info);\n",
                child->type, child->type);
        } else if (child->nodeset) {
            for (int j = 0; j < array_size(child->nodeset->nodes); j++) {
                Node *nodechild = array_get(child->nodeset->nodes, j);
                out("void _" TRAV_PREFIX
                    "%s(struct %s *node, struct Info *info);\n",
                    nodechild->id, nodechild->id);
            }
        }
    }
    out("\n");

    generate_trav_node(node, fp, config, false);
    generate_start_node(config, fp, false, node);
    generate_replace_node(node, fp, false);
}
