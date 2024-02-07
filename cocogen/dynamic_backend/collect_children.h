#pragma once
#include "ccn/ccn.h"
#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "palm/hash_table.h"

enum child_htable { CH_NULL = 0, CH_FALSE = 1, CH_TRUE = 2 };

struct child_list {
    node_st *child;
    struct child_list *next;
};

void destroy_child_list(struct child_list *list);

struct child_list *collect_children_equation_args(htable_st *htable,
                                                  node_st *node,
                                                  node_st *equation);
