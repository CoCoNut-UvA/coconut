#pragma once
#include <assert.h>
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "frontend/symboltable.h"

#ifndef MIN
#  define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#  define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

static inline node_st *get_node_attributes(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE) {
        return INODE_IATTRIBUTES(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        return INODESET_IATTRIBUTES(node);
    }
}

static inline node_st *get_node_name(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE) {
        return INODE_NAME(node);
    } else if (NODE_TYPE(node) == NT_CHILD) {
        return CHILD_NAME(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        return INODESET_NAME(node);
    }
}

static inline node_st *get_node_type(node_st *node, node_st *st) {
    if (NODE_TYPE(node) == NT_INODE || NODE_TYPE(node) == NT_INODESET) {
        return node;
    } else {
        assert(NODE_TYPE(node) == NT_CHILD);
        node_st *ref = STlookup(st, CHILD_TYPE_REFERENCE(node));
        assert(ref != NULL);
        return ref;
    }
}
