#include "attribute_scheduler/graph.h"
#include "palm/hash_table.h"
#include "palm/memory.h"

static const int hashtable_size = 200;

struct GRgraph_internal {
    htable_st *attribute_map;
    htable_st *edge_map;
    struct GRnode *last_node;
    struct GRedge *last_edge;
};

static size_t hash_attribute(void *ptr) {
    struct GRnode *key = (struct GRnode *)ptr;

    size_t hkey_node = (((size_t)key->node >> 5) & 0x1f);
    size_t hkey_attribute = (((size_t)key->attribute >> 5) & 0x1f);

    return hkey_node + hkey_attribute;
}

static bool hash_attribute_equal(void *ptr1, void *ptr2) {
    struct GRnode *key1 = (struct GRnode *)ptr1;
    struct GRnode *key2 = (struct GRnode *)ptr2;
    return key1->node == key2->node && key1->attribute == key2->attribute;
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
}

void GRdelete(struct GRgraph *graph) {
    HTdelete(graph->internal->attribute_map);
    HTdelete(graph->internal->edge_map);
    // graph->internal is part of graph
    MEMfree(graph);
}

void GRadd_node(struct GRgraph *graph, node_st *node, node_st *attribute) {
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
}
