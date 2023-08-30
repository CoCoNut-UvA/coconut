#pragma once
#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"

typedef struct GDag GDag_st;

GDag_st *GDag_dot_init(char *filename);
void GDag_dot_add_graph(GDag_st *dot, node_st *st, graph_st *graph, char *name,
                        htable_st *partition_table);
void GDag_dot_finish(GDag_st *dot);
