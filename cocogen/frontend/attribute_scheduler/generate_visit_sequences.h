#pragma once
#include "ccn/ccn.h"
#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"

node_st *generate_visit_sequences(graph_st *graph, node_st *node, node_st *st, htable_st *partition_table);
