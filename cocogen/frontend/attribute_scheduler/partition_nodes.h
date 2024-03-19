#pragma once
#include "ccn/ccn.h"
#include "palm/hash_table.h"
#include "frontend/attribute_scheduler/graph.h"

htable_st *partition_nodes(graph_st *graph, node_st *node);
bool repartition_nodes(graph_st *graph, node_st *node, node_st *st, htable_st *partition_tables);
