#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/genmacros.h"
#include "lib/print.h"
#include "lib/smap.h"

// Map from node name to index in reachability matrix
static smap_t *node_index;
static smap_t *trav_index;

// Node reachability matrix
static bool **node_reachability;

static bool **pass_nodes;

int get_node_index(char *id) {
    int *index = smap_retrieve(node_index, id);
    return *index;
}

int get_trav_index(char *id) {
    int *index = smap_retrieve(trav_index, id);
    return *index;
}

bool is_pass_node(Traversal *trav, Node *node) {
    int trav_index = get_trav_index(trav->id);
    int node_index = get_node_index(node->id);
    return pass_nodes[trav_index][node_index];
}

void compute_reachable_nodes(Config *config) {
    if (node_reachability && pass_nodes) {
        return;
    }
    node_index = smap_init(32);
    trav_index = smap_init(32);

    size_t num_nodes = array_size(config->nodes);
    size_t num_traversals = array_size(config->traversals);

    // Add nodes to node_index map
    for (int i = 0; i < num_nodes; i++) {
        Node *node = array_get(config->nodes, i);
        int *index = mem_alloc(sizeof(int));
        *index = i;
        smap_insert(node_index, node->id, index);
    }

    // Add traversals to trav_index map
    for (int i = 0; i < num_traversals; i++) {
        Traversal *trav = array_get(config->traversals, i);
        int *index = mem_alloc(sizeof(int));
        *index = i;
        smap_insert(trav_index, trav->id, index);
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
                    int index = get_node_index(nodesetnode->id);
                    node_reachability[index][i] = true;
                }
            } else if (!child->nodeset) {
                int index = get_node_index(child->type);
                node_reachability[index][i] = true;
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
                int index = get_node_index(trav_node->id);

                // List of nodes from where the handled node can be reached
                bool *reach_nodes = node_reachability[index];

                // Add the handled node itself to the list of nodes that need
                // to be traversed
                pass_nodes[i][index] = true;

                // Add the nodes in reach_node
                for (int k = 0; k < num_nodes; k++) {
                    if (reach_nodes[k])
                        pass_nodes[i][k] = true;
                }
            }
        }
    }
}