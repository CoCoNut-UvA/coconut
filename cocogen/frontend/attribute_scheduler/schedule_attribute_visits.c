/**
 * @file
 *
 * Traversal: scheduleAttributeVisits
 * UID      : SAV
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "frontend/attribute_scheduler/graph.h"
#include "frontend/attribute_scheduler/queue.h"
#include "frontend/symboltable.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include <assert.h>

static const int htable_size = 200;

unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

unsigned int hash_ptr(void *x) {
    // prevent compiler warnings
    return hash((unsigned int)((size_t)x));
}

static size_t hash_attribute(void *ptr) {
    struct GRnode *key = (struct GRnode *)ptr;
    return hash_ptr(key->node) ^ hash_ptr(key->attribute);
}

static bool hash_attribute_equal(void *ptr1, void *ptr2) {
    return GRnode_equal((struct GRnode *)ptr1, (struct GRnode *)ptr2);
}

static size_t hash_edge(void *ptr) {
    struct GRedge *key = (struct GRedge *)ptr;

    unsigned int hkey_n1 =
        hash_ptr(key->first->node) ^ hash_ptr(key->first->attribute);
    unsigned int hkey_n2 =
        hash_ptr(key->second->node) ^ hash_ptr(key->second->attribute);

    return hash(hkey_n1) ^ hkey_n2;
}

static bool hash_edge_equal(void *ptr1, void *ptr2) {
    struct GRedge *edge1 = (struct GRedge *)ptr1;
    struct GRedge *edge2 = (struct GRedge *)ptr2;
    return GRnode_equal(edge1->first, edge2->first) &&
           GRnode_equal(edge1->second, edge2->second);
}

static struct GRedge *HTlookup_edge(htable_st *table, struct GRnode *first,
                                    struct GRnode *second) {
    struct GRedge lookup = {.first = first, .second = second};
    return HTlookup(table, &lookup);
}

static void HTinsert_edge(htable_st *table, struct GRedge *edge) {
    assert(HTlookup_edge(table, edge->first, edge->second) == NULL);
    HTinsert(table, edge, edge);
}

static struct GRnode *HTlookup_graph_node(node_st *node, node_st *attribute) {
    struct GRnode lookup = {.node = node, .attribute = attribute};
    return HTlookup(DATA_SAV_GET()->graph_nodes, &lookup);
}

static struct GRnode *HTinsert_graph_node(node_st *node, node_st *attribute) {
    struct GRnode *found = HTlookup_graph_node(node, attribute);
    if (found) {
        return found;
    }

    struct GRnode *new_node = MEMmalloc(sizeof(struct GRnode));
    new_node->next = NULL;
    new_node->node = node;
    new_node->attribute = attribute;
    HTinsert(DATA_SAV_GET()->graph_nodes, new_node, new_node);
    return new_node;
}

static void *delete_nodes(void *node) {
    MEMfree((struct GRnode *)node);
    return NULL;
}

static void *delete_tables(void *table) {
    HTdelete((htable_st *)table);
    return NULL;
}

static void *delete_graphs(void *graph) {
    GRdelete((graph_st *)graph);
    return NULL;
}

void SAVinit() {
    DATA_SAV_GET()->graph_nodes =
        HTnew(htable_size, hash_attribute, hash_attribute_equal);
    return;
}
void SAVfini() {
    HTmap(DATA_SAV_GET()->graph_nodes, delete_nodes);
    return;
}

static inline node_st *get_node_attributes(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE) {
        return INODE_IATTRIBUTES(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        return INODESET_IATTRIBUTES(node);
    }
}

static inline node_st *get_node_name(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE) {
        return INODE_NAME(node);
    } else if (NODE_TYPE(node) == NT_CHILD) {
        return CHILD_NAME(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        return INODESET_NAME(node);
    }
}

static inline node_st *get_node_type(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE || NODE_TYPE(node) == NT_INODESET) {
        return node;
    } else {
        assert(NODE_TYPE(node) == NT_CHILD);
        node_st *ref =
            STlookup(DATA_SAV_GET()->symboltable, CHILD_TYPE_REFERENCE(node));
        assert(ref != NULL);
        return ref;
    }
}

static inline void handle_graph_error(struct GRerror error) {
    switch (error.type) {
    case GR_error_none:
        return;
    case GR_error_cycle:
        CTI(CTI_ERROR, true,
            "Loop found in dependencies between %s.%s and %s.%s\n",
            ID_ORIG(get_node_name(error.data.cycle.n1->node)),
            ID_ORIG(ATTRIBUTE_NAME(error.data.cycle.n1->attribute)),
            ID_ORIG(get_node_name(error.data.cycle.n2->node)),
            ID_ORIG(ATTRIBUTE_NAME(error.data.cycle.n2->attribute)));
        break;
    case GR_error_cycle_induced:
        CTI(CTI_ERROR, true,
            "Loop found in induced dependency between %s.%s and %s.%s\n",
            ID_ORIG(get_node_name(error.data.cycle.n1->node)),
            ID_ORIG(ATTRIBUTE_NAME(error.data.cycle.n1->attribute)),
            ID_ORIG(get_node_name(error.data.cycle.n2->node)),
            ID_ORIG(ATTRIBUTE_NAME(error.data.cycle.n2->attribute)));
        break;
    }

    CCNerrorAction();
    DATA_SAV_GET()->errors += 1;
}

static inline void add_attributes(graph_st *graph, node_st *node,
                                  node_st *attributes) {
    for (node_st *attr = attributes; attr != NULL;
         attr = ATTRIBUTE_NEXT(attr)) {
        if (ATTRIBUTE_IS_INHERITED(attr) || ATTRIBUTE_IS_SYNTHESIZED(attr)) {
            GRadd_node(graph, node, attr);
        }
    }
}

static void delete_edge_list(struct GRedge_list *edge_list) {
    struct GRedge_list *next_edge;
    for (struct GRedge_list *edge = edge_list; edge != NULL; edge = next_edge) {
        next_edge = edge->next;
        MEMfree(edge->edge);
        MEMfree(edge);
    }
}

// Adds converted edges from new edges to the edge list, while also deleting
// all elements from new edges
static void combine_edgelists(struct GRedge_list **edge_list,
                              struct GRedge_list *new_edges,
                              htable_st *induced_edges_map) {
    struct GRedge_list *next_new_edge;
    for (struct GRedge_list *new_edge = new_edges; new_edge != NULL;
         new_edge = next_new_edge) {
        next_new_edge = new_edge->next;
        struct GRnode *new_n1 =
            HTinsert_graph_node(get_node_type(new_edge->edge->first->node),
                                new_edge->edge->first->attribute);
        struct GRnode *new_n2 =
            HTinsert_graph_node(get_node_type(new_edge->edge->second->node),
                                new_edge->edge->second->attribute);

        if (!HTlookup_edge(induced_edges_map, new_n1, new_n2)) {
            struct GRedge_list *insert_edge =
                MEMmalloc(sizeof(struct GRedge_list));
            insert_edge->edge = MEMmalloc(sizeof(struct GRedge));
            insert_edge->edge->first = new_n1;
            insert_edge->edge->second = new_n2;
            insert_edge->edge->induced = true;
            insert_edge->next = *edge_list;
            *edge_list = insert_edge;
            HTinsert_edge(induced_edges_map, insert_edge->edge);
        }

        MEMfree(new_edge->edge);
        MEMfree(new_edge);
    }
}

static graph_st *generate_graph(node_st *node, struct GRedge_list **edges) {
    graph_st *graph = GRnew();

    add_attributes(graph, node, INODE_IATTRIBUTES(node));

    for (node_st *child = INODE_ICHILDREN(node); child != NULL;
         child = CHILD_NEXT(child)) {
        node_st *type =
            STlookup(DATA_SAV_GET()->symboltable, CHILD_TYPE_REFERENCE(child));
        add_attributes(graph, child, get_node_attributes(type));
    }

    for (node_st *equation = INODE_IEQUATIONS(node); equation != NULL;
         equation = EQUATION_NEXT(equation)) {
        node_st *rule = EQUATION_RULE(equation);
        node_st *rule_node;
        if (ATTRIBUTE_REFERENCE_INODE(rule) == NULL) {
            rule_node = node;
        } else {
            rule_node = ATTRIBUTE_REFERENCE_CHILD_REFERENCE(rule);
        }
        struct GRnode *n2 = GRlookup_node(graph, rule_node,
                                          ATTRIBUTE_REFERENCE_REFERENCE(rule));
        assert(n2 != NULL);
        for (node_st *arg = EQUATION_IARGS(equation); arg != NULL;
             arg = EQUATION_DEPENDENCY_NEXT(arg)) {
            node_st *dep = EQUATION_DEPENDENCY_IATTRIBUTE(arg);
            node_st *dep_node;
            node_st *dep_attr = ATTRIBUTE_REFERENCE_REFERENCE(dep);
            if (!ATTRIBUTE_IS_INHERITED(dep_attr) &&
                !ATTRIBUTE_IS_SYNTHESIZED(dep_attr)) {
                continue;
            }
            if (ATTRIBUTE_REFERENCE_INODE(dep) == NULL) {
                dep_node = node;
            } else {
                dep_node = ATTRIBUTE_REFERENCE_CHILD_REFERENCE(dep);
            }
            struct GRnode *n1 = GRlookup_node(graph, dep_node, dep_attr);
            assert(n1 != NULL);
            handle_graph_error(GRadd_edge(graph, n1, n2, false));
        }
    }
    handle_graph_error(GRclose_transitivity(graph, edges));

    return graph;
}

void add_induced_edges(graph_st *graph, struct GRedge_list *edges,
                       struct GRedge_list **new_edges) {
    // TODO: Add induced dependencies to graph nodes
}

static htable_st *partition_nodes(graph_st *graph) {
    htable_st *partition_table = HTnew_Ptr(htable_size);
    queue_st *work_queue = QUcreate();
    struct GRnode *node;
    for (node = graph->nodes; node != NULL; node = node->next) {
        QUinsert(work_queue, node);
    }

    while ((node = QUpop(work_queue))) {
        // TODO: partition
    }

    return partition_table;
}

/**
 * @fn SAVast
 */
node_st *SAVast(node_st *node) {
    htable_st *graphs = HTnew_Ptr(htable_size);
    htable_st *partition_tables = HTnew_Ptr(htable_size);
    htable_st *induced_edges_map =
        HTnew(htable_size, hash_edge, hash_edge_equal);
    struct GRedge_list *induced_edges = NULL;
    DATA_SAV_GET()->symboltable = AST_STABLE(node);
    DATA_SAV_GET()->errors = 0;

    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        struct GRedge_list *edges;
        graph_st *graph = generate_graph(inode, &edges);
        HTinsert(graphs, inode, graph);
        combine_edgelists(&induced_edges, edges, induced_edges_map);
    }

    // TODO: Also add nodesets

    while (induced_edges != NULL) {
        struct GRedge_list *current_edges = induced_edges;
        induced_edges = NULL;
        HTdelete(induced_edges_map);
        induced_edges_map = HTnew(htable_size, hash_edge, hash_edge_equal);
        for (htable_iter_st *iter = HTiterate(graphs); iter != NULL;
             iter = HTiterateNext(iter)) {
            struct GRedge_list *edges;
            add_induced_edges(HTiterValue(iter), current_edges, &edges);
            combine_edgelists(&induced_edges, edges, induced_edges_map);
        }
        delete_edge_list(current_edges);
    }

    HTdelete(induced_edges_map);

    if (DATA_SAV_GET()->errors > 0) {
        HTmap(graphs, delete_graphs);
        HTdelete(graphs);
        return node;
    }

    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        graph_st *graph = HTlookup(graphs, inode);
        htable_st *partition_table = partition_nodes(graph);
        HTinsert(partition_tables, inode, partition_table);
    }

    HTmap(graphs, delete_graphs);
    HTdelete(graphs);
    HTmap(partition_tables, delete_tables);
    HTdelete(partition_tables);
    return node;
}
