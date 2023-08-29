#pragma once
#include "frontend/attribute_scheduler/graph.h"

typedef struct GDag GDag_st;

GDag_st *GDag_dot_init(char *filename);
void GDag_dot_add_graph(GDag_st *dot, node_st *st, graph_st *graph, char *name);
void GDag_dot_finish(GDag_st *dot);
