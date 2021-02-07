/**
 * Reachability is based on: https://github.com/MaicoTimmerman/CoCoNut/blob/e8b03f14a283372ec561b61a59a76ff2d7fdffc6/src/cocogen/gen-trav-functions.c
 * and is licensed under the MIT license.
 *
 *
 * We create a 2D matrix with [traversals][nodes] and traverse trough all
 * traversals and then per traversal we go trough the target nodes and
 * denote them in the reachability matrix as targetted(HANDLED_BY_USER).
 * Then we go trough all nodes and check if that nodes has a child or that
 * the child of that node has a child, and so on, that is targetted by the
 * current traversal. If so, we denote it in the reachability matrix as
 * HANDLED_BY_TRAV, because the traversal needs to traverse further.
 * If a node is not handler by a user or traversal, we can skip traversing it
 * in the specific traversal.
 *
 * NOTE/TODO:
 * This can probably be optimised with a bitvector for the nodes and children
 * allowing instant lookup if a node has a certain child.
 */

#include <stddef.h>
#include <palm/memory.h>
#include <memory.h>
#include "assert.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

#include "frontend/reachability.h"
#include "frontend/symboltable.h"

int **reachability_matrix = NULL;
int *child_visited = NULL;

static bool child_is_reachable = false;
static bool is_traversal_nodes = false;
static node_st *ste;
static int trav_index = 0;
static node_st *ast;

node_st *RCBast(node_st *node)
{
    reachability_matrix = MEMmalloc(sizeof(int *) * AST_NUM_TRAVERSALS(node) + 1);
    child_visited = MEMmalloc(sizeof(int) * AST_NUM_NODES(node) + 1);
    ast = node;
    ste = AST_STABLE(node);
    TRAVopt(AST_ITRAVERSALS(node));

    MEMfree(child_visited);
    return node;
}

node_st *RCBitraversal(node_st *node)
{
    trav_index = ITRAVERSAL_INDEX(node);
    reachability_matrix[trav_index] = MEMmalloc(sizeof(int) * AST_NUM_NODES(ast));
    memset(reachability_matrix[trav_index], 0, sizeof(int) *AST_NUM_NODES(ast));

    if (!ITRAVERSAL_INODES(node)) {
        const int num_nodes = AST_NUM_NODES(ast);
        for (int i = 0; i < num_nodes; i++) {
            reachability_matrix[trav_index][i] = RCB_NODE_HANDLED_BY_USER;
        }
    } else {
        is_traversal_nodes = true;
        TRAVdo(ITRAVERSAL_INODES(node));
        is_traversal_nodes = false;
        child_is_reachable = false;
        TRAVdo(AST_INODES(ast));
    }

    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *RCBinode(node_st *node)
{
    if (!reachability_matrix[trav_index][INODE_INDEX(node)]) {
        child_is_reachable = false;
        // To track if we visited children already to prevent infinite recursion.
        memset(child_visited, 0, sizeof(int) * AST_NUM_NODES(ast));
        TRAVopt(INODE_ICHILDREN(node));
        if (child_is_reachable) {
            reachability_matrix[trav_index][INODE_INDEX(node)] = RCB_NODE_HANDLED_BY_TRAV;
        }
    }
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *RCBinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *RCBchild(node_st *node)
{
    node_st *inode = STlookup(ste, CHILD_TYPE_REFERENCE(node));
    assert(NODE_TYPE(inode) == NT_INODE || NODE_TYPE(inode) == NT_INODESET);
    if (NODE_TYPE(inode) == NT_INODESET) {
        TRAVdo(inode);
    } else {
        if (!child_visited[INODE_INDEX(inode)]) {
            child_visited[INODE_INDEX(inode)] = 1;
            const int reach = reachability_matrix[trav_index][INODE_INDEX(inode)];
            if (reach && reach != RCB_NODE_NOT_HANDLED) {
                child_is_reachable = true;
            } else {
                TRAVopt(INODE_ICHILDREN(inode));
            }
        }
    }
    // Stop the search if we already found that a child is reachable.
    if (!child_is_reachable) {
        TRAVopt(CHILD_NEXT(node));
    }
    return node;
}

node_st *RCBsetliteral(node_st *node)
{
    node_st *inode = STlookup(ste, SETLITERAL_REFERENCE(node));
    assert(NODE_TYPE(inode) == NT_INODE);
    if (is_traversal_nodes) {
        reachability_matrix[trav_index][INODE_INDEX(inode)] = RCB_NODE_HANDLED_BY_USER;
    } else {
        if (!child_visited[INODE_INDEX(inode)]) {
            child_visited[INODE_INDEX(inode)] = 1;
            const int reach = reachability_matrix[trav_index][INODE_INDEX(inode)];
            if (reach && reach != RCB_NODE_NOT_HANDLED) {
                child_is_reachable = true;
            } else {
                TRAVopt(INODE_ICHILDREN(inode));
            }
        }
    }

    TRAVopt(SETLITERAL_LEFT(node));
    TRAVopt(SETLITERAL_RIGHT(node));
    return node;
}

