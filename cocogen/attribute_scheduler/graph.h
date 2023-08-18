#pragma once
#include <stdbool.h>
#include "ccn/ccn.h"

struct GRnode {
    node_st *node;
    node_st *attribute;
    struct GRnode *next;
};

struct GRedge {
    struct GRnode *first;
    struct GRnode *second;
    bool induced;
};

struct GRedge_list {
    struct GRedge *edge;
    struct GRedge_list *next;
};

struct GRgraph_internal;

struct GRgraph {
    struct GRnode *nodes;
    struct GRedge_list *edges;
    struct GRgraph_internal *internal;
};

typedef struct GRgraph graph_st;

graph_st *GRnew();
void GRdelete(graph_st *graph);
