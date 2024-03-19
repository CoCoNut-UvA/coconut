#include <assert.h>

#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"
#include "palm/memory.h"

// forward declaration
static struct GRedge *add_edge_internal(struct GRgraph *graph,
                                        struct GRnode *n1, struct GRnode *n2,
                                        enum GRedge_type type);

static const int hashtable_size = 200;

struct GRgraph_internal {
    htable_st *attribute_map;
    htable_st *edge_map;
    struct GRnode *last_node;
    struct GRedge_list *last_edge;
};

static size_t hash_attribute(void *ptr) {
    struct GRnode *key = (struct GRnode *)ptr;

    size_t hkey_node = (((size_t)key->node >> 5) & 0x1f);
    size_t hkey_attribute = (((size_t)key->attribute >> 5) & 0x1f);

    return hkey_node + hkey_attribute;
}

bool GRnode_equal(struct GRnode *n1, struct GRnode *n2) {
    return n1->node == n2->node && n1->attribute == n2->attribute;
}

static bool hash_attribute_equal(void *ptr1, void *ptr2) {
    return GRnode_equal((struct GRnode *)ptr1, (struct GRnode *)ptr2);
}

static struct GRedge *HTlookup_edge(htable_st *edge_map, struct GRnode *n1,
                                    struct GRnode *n2) {
    htable_st *inner_table = (htable_st *)HTlookup(edge_map, n1);
    if (inner_table == NULL) {
        return NULL;
    }

    return (struct GRedge *)HTlookup(inner_table, n2);
}

static void HTinsert_edge(htable_st *edge_map, struct GRedge *edge) {
    htable_st *inner_table = (htable_st *)HTlookup(edge_map, edge->first);
    if (inner_table == NULL) {
        inner_table = HTnew_Ptr(hashtable_size);
        HTinsert(edge_map, edge->first, inner_table);
    }

    if (HTlookup(inner_table, edge->second) != NULL) {
        return;
    }

    HTinsert(inner_table, edge->second, edge);
}

struct GRgraph *GRnew() {
    // Allocate graph_internal in the same block of memory
    struct GRgraph *graph =
        MEMmalloc(sizeof(struct GRgraph) + sizeof(struct GRgraph_internal));
    // Set graph->internal to memory after graph struct
    graph->internal = (struct GRgraph_internal *)(graph + 1);
    graph->nodes = NULL;
    graph->edges = NULL;

    // Initialize hashmaps
    graph->internal->attribute_map =
        HTnew(hashtable_size, hash_attribute, hash_attribute_equal);
    graph->internal->edge_map = HTnew_Ptr(hashtable_size);

    // Other internal members
    graph->internal->last_node = NULL;
    graph->internal->last_edge = NULL;

    return graph;
}

struct GRgraph *GRcopy(struct GRgraph *graph) {
    struct GRgraph *copy = GRnew();
    htable_st *node_mapping = HTnew_Ptr(hashtable_size);

    for (struct GRnode *node = graph->nodes; node != NULL; node = node->next) {
        struct GRnode *new = GRadd_node(copy, node->node, node->attribute);
        HTinsert(node_mapping, node, new);
    }

    for (struct GRedge_list *entry = graph->edges; entry != NULL;
         entry = entry->next) {
        struct GRedge *edge = entry->edge;
        add_edge_internal(copy, HTlookup(node_mapping, edge->first),
                          HTlookup(node_mapping, edge->second), edge->type);
    }

    HTdelete(node_mapping);

    return copy;
}

static void *delete_subtables(void *htable) {
    HTdelete((htable_st *)htable);
    return NULL;
}

void GRdelete(struct GRgraph *graph) {
    HTdelete(graph->internal->attribute_map);
    HTmap(graph->internal->edge_map, delete_subtables);
    HTdelete(graph->internal->edge_map);

    struct GRnode *next_node = NULL;
    for (struct GRnode *n = graph->nodes; n != NULL; n = next_node) {
        next_node = n->next;
        MEMfree(n);
    }

    struct GRedge_list *next_edge = NULL;
    for (struct GRedge_list *e = graph->edges; e != NULL; e = next_edge) {
        next_edge = e->next;
        MEMfree(e->edge);
        MEMfree(e);
    }

    // graph->internal is part of graph
    MEMfree(graph);
}

struct GRnode *GRadd_node(struct GRgraph *graph, node_st *node,
                          node_st *attribute) {
    struct GRnode *graph_node = MEMmalloc(sizeof(struct GRnode));
    graph_node->node = node;
    graph_node->attribute = attribute;
    graph_node->next = NULL;

    assert(HTlookup(graph->internal->attribute_map, graph_node) == NULL);

    HTinsert(graph->internal->attribute_map, graph_node, graph_node);

    if (graph->nodes == NULL) { // first node
        graph->nodes = graph_node;
        graph->internal->last_node = graph_node;
    } else {
        graph->internal->last_node->next = graph_node;
        graph->internal->last_node = graph_node;
    }

    return graph_node;
}

struct GRnode *GRlookup_node(struct GRgraph *graph, node_st *node,
                             node_st *attribute) {
    struct GRnode lookup_node = {.node = node, .attribute = attribute};
    return (struct GRnode *)HTlookup(graph->internal->attribute_map,
                                     &lookup_node);
}

struct GRnode_list *GRget_inter_node_dependencies(struct GRgraph *graph,
                                                  struct GRnode *node) {
    struct GRnode_list *dependencies = NULL;
    for (struct GRnode *dep = graph->nodes; dep != NULL; dep = dep->next) {
        if (dep == node || dep->node != node->node) {
            continue;
        }

        if (GRlookup_edge(graph, dep, node, false)) {
            struct GRnode_list *entry = MEMmalloc(sizeof(struct GRnode_list));
            entry->node = dep;
            entry->next = dependencies;
            dependencies = entry;
        }
    }

    return dependencies;
}

struct GRnode_list *GRget_node_dependencies(struct GRgraph *graph,
                                            struct GRnode *node) {
    struct GRnode_list *dependencies = NULL;
    for (struct GRnode *dep = graph->nodes; dep != NULL; dep = dep->next) {
        if (dep == node) {
            continue;
        }

        if (GRlookup_edge(graph, dep, node, false)) {
            struct GRnode_list *entry = MEMmalloc(sizeof(struct GRnode_list));
            entry->node = dep;
            entry->next = dependencies;
            dependencies = entry;
        }
    }

    return dependencies;
}

static struct GRedge *add_edge_internal(struct GRgraph *graph,
                                        struct GRnode *n1, struct GRnode *n2,
                                        enum GRedge_type type) {
    struct GRedge_list *graph_edge_entry =
        MEMmalloc(sizeof(struct GRedge_list));
    struct GRedge *graph_edge = MEMmalloc(sizeof(struct GRedge));
    graph_edge->first = n1;
    graph_edge->second = n2;
    graph_edge->type = type;
    assert(HTlookup_edge(graph->internal->edge_map, n1, n2) == NULL);
    HTinsert_edge(graph->internal->edge_map, graph_edge);

    graph_edge_entry->edge = graph_edge;
    graph_edge_entry->next = NULL;
    if (graph->edges == NULL) { // first node
        graph->edges = graph_edge_entry;
        graph->internal->last_edge = graph_edge_entry;
    } else {
        graph->internal->last_edge->next = graph_edge_entry;
        graph->internal->last_edge = graph_edge_entry;
    }
    return graph_edge;
}

static inline bool check_cycle(struct GRgraph *graph, struct GRnode *n1,
                               struct GRnode *n2, struct GRerror *error,
                               enum GRedge_type type) {
    if (HTlookup_edge(graph->internal->edge_map, n2, n1)) {
        switch (type) {
        case GRnormal:
            error->type = GR_error_cycle;
            break;
        case GRinduced:
            error->type = GR_error_cycle_induced;
            break;
        case GRintravisit:
            error->type = GR_error_cycle_intravisit;
            break;
        case GRintravisit_induced:
            error->type = GR_error_cycle_intravisit_induced;
            break;
        default:
            assert(false); // unhandled case
            break;
        }

        error->data.cycle.n1 = n1;
        error->data.cycle.n2 = n2;
        return true;
    }

    return false;
}

struct GRerror GRadd_edge(struct GRgraph *graph, struct GRnode *n1,
                          struct GRnode *n2, enum GRedge_type type) {
    struct GRerror error = {.type = GR_error_none};
    if (check_cycle(graph, n1, n2, &error, type)) {
        return error;
    }
    add_edge_internal(graph, n1, n2, type);
    return error;
}

struct GRedge *GRlookup_edge(graph_st *graph, struct GRnode *from,
                             struct GRnode *to, bool intravisit) {
    struct GRedge *edge = HTlookup_edge(graph->internal->edge_map, from, to);
    if (!edge) {
        return NULL;
    } else if (!intravisit && (edge->type == GRintravisit ||
                               edge->type == GRintravisit_induced)) {
        return NULL;
    } else {
        return edge;
    }
}

void GRadd_new_intra_node_dependency(struct GRedge_list **added_edges,
                                     struct GRnode *n1, struct GRnode *n2) {
    struct GRedge_list *entry = MEMmalloc(sizeof(struct GRedge_list));
    struct GRedge *edge = MEMmalloc(sizeof(struct GRedge));
    edge->first = n1;
    edge->second = n2;
    edge->type = GRinduced;
    entry->edge = edge;

    entry->next = *added_edges;
    *added_edges = entry;
}

struct GRerror GRclose_transitivity(struct GRgraph *graph,
                                    struct GRedge_list **added_edges) {
    struct GRerror error = {.type = GR_error_none};
    bool changed;
    do {
        changed = false;
        for (struct GRnode *i = graph->nodes; i != NULL; i = i->next) {
            for (struct GRnode *j = graph->nodes; j != NULL; j = j->next) {
                if (i == j || HTlookup_edge(graph->internal->edge_map, i, j)) {
                    continue;
                }

                for (struct GRnode *k = graph->nodes; k != NULL; k = k->next) {
                    if (i == k || j == k) {
                        continue;
                    }
                    if (HTlookup_edge(graph->internal->edge_map, i, k) &&
                        HTlookup_edge(graph->internal->edge_map, k, j)) {
                        add_edge_internal(graph, i, j, true);
                        // check for new induced depencies inside a node
                        if (i->node == j->node) {
                            GRadd_new_intra_node_dependency(added_edges, i, j);
                        }
                        changed = true;
                        if (check_cycle(graph, i, j, &error, true)) {
                            return error;
                        }

                        // Added induced edge, no longer need to search further
                        break;
                    }
                }
            }
        }
    } while (changed);
    return error;
}
