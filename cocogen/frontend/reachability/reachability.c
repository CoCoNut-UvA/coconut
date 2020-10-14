
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "palm/memory.h"
#include "palm/dbug.h"
#include "palm/hash_table.h"

// Map from node name to index in reachability matrix
static htable_st *node_index = NULL;
static htable_st *trav_index = NULL;

// Node reachability matrix
static bool **node_reachability = NULL;
static bool **traversal_node_handles = NULL;


int RBgetNodeIndex(struct node *node)
{
    bool found = false;
    int *index = HTlookup(node_index, node->name->orig, &found);
    DBUG_ASSERT(found && index != NULL, "Could not find node index.");

    return *index;
}

int RBgetTravIndex(struct traversal *trav)
{
    bool found = false;
    int *index = HTlookup(trav_index, trav->name->orig, &found);
    DBUG_ASSERT(found, "Could not find trav index.");

    return *index;
}


bool RBisTraversalNode(struct traversal *trav, struct node *node)
{
    int trav_i = RBgetTravIndex(trav);
    int node_i = RBgetNodeIndex(node);
    printf("%d, %d\n", node_i, trav_i);
    return traversal_node_handles[trav_i][node_i];
}

bool RBshouldPassNode(struct traversal *trav, struct node *node)
{
    int trav_i = RBgetTravIndex(trav);
    int node_i = RBgetNodeIndex(node);
    return node_reachability[trav_i][node_i];
}

static
void CreateIndices(struct ast *ast) {
    size_t nodes = ASTnumOfNodes(ast);
    size_t travs = ASTnumOfTraversals(ast);

    node_index = HTnew_String(nodes);
    trav_index = HTnew_String(travs);

    int counter = 0;
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        int *index = MEMmalloc(sizeof(int));
        *index = counter;
        bool success = HTinsert(node_index, node->name->orig, index);
        DBUG_ASSERT(success, "Could not insert node index in node htable");
        counter++;
    }

    counter = 0;
    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        int *index = MEMmalloc(sizeof(int));
        *index = counter;
        bool found = HTinsert(trav_index, trav->name->orig, index);
        DBUG_ASSERT(found, "Could not insert trav index in trav htable");
        counter++;
    }
}


struct ast *RBdoTrav(struct ast *ast)
{
    if (node_reachability && traversal_node_handles) {
        return ast;
    }

    CreateIndices(ast);

    size_t num_traversals = ASTnumOfTraversals(ast);
    size_t num_nodes = ASTnumOfNodes(ast);

    node_reachability = MEMmalloc(sizeof(bool *) * num_nodes);

    for (size_t i = 0; i < num_nodes; i++) {
        node_reachability[i] = MEMmalloc(sizeof(bool) * num_nodes);
        memset(node_reachability[i], 0, sizeof(bool) * num_nodes);
    }

    int curr_node = 0;
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        int index = RBgetNodeIndex(node);

        struct child *child = NULL;
        SLIST_FOREACH(child, node->children, next) {
            if (child->type == CT_NODESET) {
                struct node_wrapper *child_node_wrapper = NULL;
                SLIST_FOREACH(child_node_wrapper, child->nodeset->nodes, next) {
                    struct node *child_node = child_node_wrapper->node;
                    int child_index = RBgetNodeIndex(child_node);
                    node_reachability[index][child_index];
                }

            } else if (child->type == CT_NODE) {
                int child_index = RBgetNodeIndex(child->node);
                node_reachability[index][child_index] = true;
            } else {
                DBUG_ASSERT(false, "Child type is NULL");
            }
        }
    }

        // Compute reachability of nodes using the Floyd-Warshall algorithm
    for (size_t k = 0; k < num_nodes; k++) {
        for (size_t i = 0; i < num_nodes; i++) {
            for (size_t j = 0; j < num_nodes; j++) {
                if (node_reachability[k][i] && node_reachability[j][k])
                    node_reachability[j][i] = true;
            }
        }
    }

    traversal_node_handles = MEMmalloc(sizeof(bool *) * num_traversals);

    struct traversal *trav = NULL;
    int curr_trav = 0;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        traversal_node_handles[curr_trav] = MEMmalloc(sizeof(bool) * num_nodes);
        if (trav->nodes == NULL) {
             for (int j = 0; j < num_nodes; j++) {
                traversal_node_handles[curr_trav][j] = true;
            }
        } else {
            memset(traversal_node_handles[curr_trav], 0, sizeof(bool) * num_nodes);
            struct node_wrapper *trav_node_wrapper = NULL;
            SLIST_FOREACH(trav_node_wrapper, trav->nodes, next) {
                struct node *trav_node = trav_node_wrapper->node;
                int index = RBgetNodeIndex(trav_node);

                // List of nodes from where the handled node can be reached
                bool *reach_nodes = node_reachability[index];

                traversal_node_handles[curr_trav][index] = true;

                for (size_t k = 0; k < num_nodes; k++) {
                    if (reach_nodes[k]) {
                        traversal_node_handles[curr_trav][k] = true;
                    }
                }
            }
        }
    }

    return ast;
}