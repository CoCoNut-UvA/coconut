#pragma once
#include "ccn/ccn.h"
#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"

struct visit {
    size_t index;
    struct GRnode **inputs;
    size_t inputs_length;
    struct GRnode **outputs;
    size_t outputs_length;
};

struct visits {
    struct visit **visits;
    size_t length;
};

struct visits *collect_visits(graph_st *graph, node_st *node, node_st *st,
                              htable_st *partition_tables);
void generate_empty_visits(node_st *inodes, node_st *nodesets, node_st *st, htable_st *visits_htable);
