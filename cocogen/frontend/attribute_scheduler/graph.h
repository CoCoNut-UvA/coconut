#pragma once
#include "ccn/ccn.h"
#include <stdbool.h>

struct GRnode {
    node_st *node;
    node_st *attribute;
    struct GRnode *next;
};

struct GRnode_list {
    struct GRnode *node;
    struct GRnode_list *next;
};

enum GRedge_type {
    GRnormal,
    GRinduced,
    GRintravisit,
    GRintravisit_induced
};

struct GRedge {
    struct GRnode *first;
    struct GRnode *second;
    enum GRedge_type type;
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

enum GRerror_type {
    GR_error_none,
    GR_error_cycle,
    GR_error_cycle_induced,
    GR_error_cycle_intravisit,
    GR_error_cycle_intravisit_induced
};

struct GRerror {
    enum GRerror_type type;
    union {
        struct {
            struct GRnode *n1;
            struct GRnode *n2;
        } cycle;
    } data;
};

typedef struct GRgraph graph_st;

graph_st *GRnew();
void GRdelete(graph_st *graph);
graph_st *GRcopy(graph_st *graph);
bool GRnode_equal(struct GRnode *n1, struct GRnode *n2);
struct GRnode *GRadd_node(graph_st *graph, node_st *node, node_st *attribute);
struct GRnode *GRlookup_node(graph_st *graph, node_st *node,
                             node_st *attribute);
struct GRnode_list *GRget_inter_node_dependencies(struct GRgraph *graph,
                                                  struct GRnode *node);
struct GRnode_list *GRget_node_dependencies(struct GRgraph *graph,
                                            struct GRnode *node);
struct GRerror GRadd_edge(graph_st *graph, struct GRnode *from,
                          struct GRnode *to, enum GRedge_type type);
struct GRedge *GRlookup_edge(graph_st *graph, struct GRnode *from,
                             struct GRnode *to, bool intravisit);
void GRadd_new_intra_node_dependency(struct GRedge_list **added_edges,
                                     struct GRnode *n1, struct GRnode *n2);
struct GRerror GRclose_transitivity(graph_st *graph,
                                    struct GRedge_list **added_edges);
