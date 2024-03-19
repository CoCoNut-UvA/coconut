/**
 * @file
 *
 * Traversal: scheduleAttributeVisits
 * UID      : SAV
 *
 *
 */

// TODO: split synthesized/inherited attributes in different categories (add
// enum to GRnode and use in hash?)

#include <assert.h>
#include <stdio.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "dot/gen_dot_ag.h"
#include "frontend/attribute_scheduler/collect_visits.h"
#include "frontend/attribute_scheduler/common.h"
#include "frontend/attribute_scheduler/fill_visit_stubs.h"
#include "frontend/attribute_scheduler/generate_visits.h"
#include "frontend/attribute_scheduler/graph.h"
#include "frontend/attribute_scheduler/partition_nodes.h"
#include "frontend/attribute_scheduler/queue.h"
#include "frontend/symboltable.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

static const int htable_size = 200;

struct node_list {
    node_st *node;
    struct node_list *next;
};

struct graph_list {
    graph_st *graph;
    struct graph_list *next;
};

static void *free_visits_map(void *arg) {
    free_visits((struct visits *)arg);
    return NULL;
}

static void *free_stubs_map(void *arg) {
    free_visit_stubs((struct visit_stub *)arg);
    return NULL;
}

static void *delete_nodes(void *node) {
    MEMfree((struct GRnode *)node);
    return NULL;
}

static void *delete_tables(void *table) {
    HTdelete((htable_st *)table);
    return NULL;
}

static void *delete_graphs(void *graphs) {
    struct graph_list *next;
    for (struct graph_list *graph = (struct graph_list *)graphs; graph != NULL;
         graph = next) {
        GRdelete(graph->graph);
        next = graph->next;
        MEMfree(graph);
    }
    return NULL;
}

static unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

static unsigned int hash_ptr(void *x) {
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

static inline struct GRedge *
HTlookup_edge(htable_st *table, struct GRnode *first, struct GRnode *second) {
    struct GRedge lookup = {.first = first, .second = second};
    return HTlookup(table, &lookup);
}

static inline void HTinsert_edge(htable_st *table, struct GRedge *edge) {
    assert(HTlookup_edge(table, edge->first, edge->second) == NULL);
    HTinsert(table, edge, edge);
}

static inline struct GRnode *HTlookup_graph_node(node_st *node,
                                                 node_st *attribute) {
    struct GRnode lookup = {.node = node, .attribute = attribute};
    return HTlookup(DATA_SAV_GET()->graph_nodes, &lookup);
}

static inline struct GRnode *HTinsert_graph_node(node_st *node,
                                                 node_st *attribute) {
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

void SAVinit() {
    DATA_SAV_GET()->graph_nodes =
        HTnew(htable_size, hash_attribute, hash_attribute_equal);
    return;
}
void SAVfini() {
    HTmap(DATA_SAV_GET()->graph_nodes, delete_nodes);
    return;
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
    default:
        assert(false); // Error not handled
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

static inline void delete_edge_list(struct GRedge_list *edge_list) {
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
            HTinsert_graph_node(get_node_type(new_edge->edge->first->node,
                                              DATA_SAV_GET()->symboltable),
                                new_edge->edge->first->attribute);
        struct GRnode *new_n2 =
            HTinsert_graph_node(get_node_type(new_edge->edge->second->node,
                                              DATA_SAV_GET()->symboltable),
                                new_edge->edge->second->attribute);

        if (!HTlookup_edge(induced_edges_map, new_n1, new_n2)) {
            struct GRedge_list *insert_edge =
                MEMmalloc(sizeof(struct GRedge_list));
            insert_edge->edge = MEMmalloc(sizeof(struct GRedge));
            insert_edge->edge->first = new_n1;
            insert_edge->edge->second = new_n2;
            insert_edge->edge->type = GRinduced;
            insert_edge->next = *edge_list;
            *edge_list = insert_edge;
            HTinsert_edge(induced_edges_map, insert_edge->edge);
        }

        MEMfree(new_edge->edge);
        MEMfree(new_edge);
    }
}

static void add_nodeset_entries(node_st *current_node,
                                struct node_list **nodes) {
    if (current_node == NULL) {
        return;
    }

    struct node_list *node = MEMmalloc(sizeof(struct node_list));
    node->node = STlookup(DATA_SAV_GET()->symboltable,
                          SETLITERAL_REFERENCE(current_node));
    assert(node->node != NULL && NODE_TYPE(node->node) == NT_INODE);
    node->next = *nodes;
    *nodes = node;

    add_nodeset_entries(SETLITERAL_LEFT(current_node), nodes);
    add_nodeset_entries(SETLITERAL_RIGHT(current_node), nodes);
}

static inline void delete_nodelist(struct node_list *node_list) {
    struct node_list *next;
    for (struct node_list *node = node_list; node != NULL; node = next) {
        next = node->next;
        MEMfree(node);
    }
}

static inline node_st *get_equivalent_attribute(node_st *attr, node_st *node) {
    node_st *equiv_attr = NULL;

    for (node_st *cand = INODE_IATTRIBUTES(node); cand != NULL;
         cand = ATTRIBUTE_NEXT(cand)) {
        if (STReq(ID_LWR(ATTRIBUTE_NAME(attr)), ID_LWR(ATTRIBUTE_NAME(cand)))) {
            equiv_attr = cand;
        }
    }

    assert(equiv_attr != NULL);
    return equiv_attr;
}

static struct graph_list *generate_graph_nodeset(node_st *nodeset,
                                                 struct GRedge_list **edges) {
    struct graph_list *graphs = NULL;
    struct node_list *nodes = NULL;
    add_nodeset_entries(INODESET_EXPR(nodeset), &nodes);

    // For each member node, add edges between nodeset attributes and node
    // attributes
    for (struct node_list *entry = nodes; entry != NULL; entry = entry->next) {
        graph_st *graph = GRnew();
        add_attributes(graph, nodeset, INODESET_IATTRIBUTES(nodeset));
        add_attributes(graph, entry->node, INODESET_IATTRIBUTES(nodeset));

        for (node_st *attr = INODESET_IATTRIBUTES(nodeset); attr != NULL;
             attr = ATTRIBUTE_NEXT(attr)) {
            if (!ATTRIBUTE_IS_INHERITED(attr) &&
                !ATTRIBUTE_IS_SYNTHESIZED(attr)) {
                continue;
            }

            struct GRnode *n1 = GRlookup_node(graph, nodeset, attr);
            if (n1 == NULL) {
                n1 = GRadd_node(graph, nodeset, attr);
            }
            node_st *node_attr = get_equivalent_attribute(attr, entry->node);
            struct GRnode *n2 = GRlookup_node(graph, entry->node, node_attr);
            if (n2 == NULL) {
                n2 = GRadd_node(graph, entry->node, node_attr);
            }

            if (ATTRIBUTE_IS_INHERITED(attr)) {
                handle_graph_error(GRadd_edge(graph, n1, n2, false));
            }

            if (ATTRIBUTE_IS_SYNTHESIZED(attr)) {
                handle_graph_error(GRadd_edge(graph, n2, n1, false));
            }
        }

        handle_graph_error(GRclose_transitivity(graph, edges));

        struct graph_list *graph_entry = MEMmalloc(sizeof(struct graph_list));
        graph_entry->graph = graph;
        graph_entry->next = graphs;
        graphs = graph_entry;
    }

    return graphs;
}

static graph_st *generate_graph_node(node_st *node,
                                     struct GRedge_list **edges) {
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
            if (n1->node == n2->node) {
                GRadd_new_intra_node_dependency(edges, n1, n2);
            }
        }
    }
    handle_graph_error(GRclose_transitivity(graph, edges));

    return graph;
}

void add_induced_edges(graph_st *graph, struct GRedge_list *edges,
                       struct GRedge_list **new_edges) {
    for (struct GRedge_list *entry = edges; entry != NULL;
         entry = entry->next) {
        node_st *tnode = entry->edge->first->node;
        for (struct GRnode *node = graph->nodes; node != NULL;
             node = node->next) {
            node_st *node_type =
                get_node_type(node->node, DATA_SAV_GET()->symboltable);
            if (tnode == node_type) {
                struct GRnode *from = GRlookup_node(
                    graph, node->node, entry->edge->first->attribute);
                struct GRnode *to = GRlookup_node(
                    graph, node->node, entry->edge->second->attribute);
                if (from == NULL || to == NULL ||
                    GRlookup_edge(graph, from, to, false)) {
                    continue;
                }
                handle_graph_error(GRadd_edge(graph, from, to, true));
            }
        }
    }

    handle_graph_error(GRclose_transitivity(graph, new_edges));
}

// Create intravisit dependencies from I_i to S_i and from S_i to I_i+1:
// i = even | i -> i - 1 i = odd  | i -> i + 3
static inline void find_intravisit_dependencies_node(
    struct GRedge_list **dependencies, htable_st *partition_table,
    graph_st *graph, node_st *node, node_st *attr, size_t partition) {
    size_t target_partition =
        (partition % 2 == 0) ? (partition - 1) : (partition + 3);
    struct GRnode *n1 = GRlookup_node(graph, node, attr);
    assert(n1 != NULL);

    for (htable_iter_st *iter = HTiterate(partition_table); iter != NULL;
         iter = HTiterateNext(iter)) {
        node_st *cand = HTiterKey(iter);
        size_t cand_partition = (size_t)HTiterValue(iter);

        if (cand_partition == target_partition) {
            struct GRnode *n2 = GRlookup_node(graph, node, cand);
            assert(n2 != NULL);
            struct GRedge *dependency = MEMmalloc(sizeof(struct GRedge));
            dependency->first = n1;
            dependency->second = n2;
            dependency->type = GRintravisit;
            struct GRedge_list *entry = MEMmalloc(sizeof(struct GRedge_list));
            entry->edge = dependency;
            entry->next = *dependencies;
            *dependencies = entry;
        }
    }
}

static struct GRedge_list *
find_intravisit_dependencies(htable_st *graphs_htable,
                             htable_st *partition_tables) {
    struct GRedge_list *dependencies = NULL;
    for (htable_iter_st *iter = HTiterate(partition_tables); iter != NULL;
         iter = HTiterateNext(iter)) {
        node_st *node = HTiterKey(iter);
        struct graph_list *graphs = HTlookup(graphs_htable, node);
        if (graphs == NULL) {
            continue;
        }
        graph_st *graph = graphs->graph;
        htable_st *subtable = HTiterValue(iter);
        for (htable_iter_st *subiter = HTiterate(subtable); subiter != NULL;
             subiter = HTiterateNext(subiter)) {
            node_st *attr = HTiterKey(subiter);
            size_t partition = (size_t)HTiterValue(subiter);
            find_intravisit_dependencies_node(&dependencies, subtable, graph,
                                              node, attr, partition);
        }
    }

    return dependencies;
}

static void dump_dotfile(htable_st *graphs, char *filename,
                         htable_st *partition_tables) {
    GDag_st *dot = GDag_dot_init(filename);
    for (htable_iter_st *iter = HTiterate(graphs); iter != NULL;
         iter = HTiterateNext(iter)) {
        node_st *dot_name = get_node_name(HTiterKey(iter));
        struct graph_list *graph = HTiterValue(iter);
        bool multiple = graph != NULL && graph->next != NULL;
        int i = 1;
        for (graph = graph; graph != NULL; graph = graph->next) {
            char *name;
            if (multiple) {
                char *tmp = STRitoa(i);
                name = STRcatn(3, ID_ORIG(dot_name), "_", tmp);
                MEMfree(tmp);
            } else {
                name = ID_ORIG(dot_name);
            }

            GDag_dot_add_graph(dot, DATA_SAV_GET()->symboltable, graph->graph,
                               name, partition_tables);

            if (multiple) {
                MEMfree(name);
            }

            ++i;
        }
    }
    GDag_dot_finish(dot);
}

/**
 * @fn SAVast
 */
node_st *SAVast(node_st *node) {
    htable_st *graphs_htable = HTnew_Ptr(htable_size);
    htable_st *partition_tables = HTnew_Ptr(htable_size);
    htable_st *induced_edges_map =
        HTnew(htable_size, hash_edge, hash_edge_equal);
    struct GRedge_list *induced_edges = NULL;
    DATA_SAV_GET()->symboltable = AST_STABLE(node);
    DATA_SAV_GET()->errors = 0;

    // Create node production graphs
    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        struct GRedge_list *edges = NULL;
        graph_st *graph = generate_graph_node(inode, &edges);
        struct graph_list *graph_entry = MEMmalloc(sizeof(struct graph_list));
        graph_entry->graph = graph;
        graph_entry->next = NULL;
        HTinsert(graphs_htable, inode, graph_entry);
        combine_edgelists(&induced_edges, edges, induced_edges_map);
    }

    // Create nodeset production graphs
    for (node_st *nodeset = AST_INODESETS(node); nodeset != NULL;
         nodeset = INODESET_NEXT(nodeset)) {
        struct GRedge_list *edges = NULL;
        struct graph_list *graphs = generate_graph_nodeset(nodeset, &edges);
        HTinsert(graphs_htable, nodeset, graphs);
        combine_edgelists(&induced_edges, edges, induced_edges_map);
    }

    dump_dotfile(graphs_htable, "pre_induce.dot", NULL);

    int counter = 1;

    // Add all induced dependencies to graphs
    while (induced_edges != NULL) {
        struct GRedge_list *current_edges = induced_edges;
        induced_edges = NULL;
        HTdelete(induced_edges_map);
        induced_edges_map = HTnew(htable_size, hash_edge, hash_edge_equal);
        for (htable_iter_st *iter = HTiterate(graphs_htable); iter != NULL;
             iter = HTiterateNext(iter)) {
            for (struct graph_list *graph = HTiterValue(iter); graph != NULL;
                 graph = graph->next) {
                struct GRedge_list *edges = NULL;
                add_induced_edges(graph->graph, current_edges, &edges);
                combine_edgelists(&induced_edges, edges, induced_edges_map);
            }
        }
        delete_edge_list(current_edges);

        // Write intermediate results to dot file
        char *filename = STRfmt("induce_%d.dot", counter);
        dump_dotfile(graphs_htable, filename, NULL);
        MEMfree(filename);
        ++counter;
    }

    HTdelete(induced_edges_map);

    if (DATA_SAV_GET()->errors > 0) {
        HTmap(graphs_htable, delete_graphs);
        HTdelete(graphs_htable);
        return node;
    }

    // Partition attributes in graphs
    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        struct graph_list *graph = HTlookup(graphs_htable, inode);
        htable_st *partition_table = partition_nodes(graph->graph, inode);
        HTinsert(partition_tables, inode, partition_table);
    }

    for (node_st *nodeset = AST_INODESETS(node); nodeset != NULL;
         nodeset = INODESET_NEXT(nodeset)) {
        struct graph_list *graph = HTlookup(graphs_htable, nodeset);
        if (graph == NULL) { // Empty nodeset
            continue;
        }
        htable_st *partition_table = partition_nodes(graph->graph, nodeset);
        HTinsert(partition_tables, nodeset, partition_table);
    }

    dump_dotfile(graphs_htable, "partitioned.dot", partition_tables);


    // struct GRedge_list *intra_visit_dependencies =
    //     find_intravisit_dependencies(graphs_htable, partition_tables);

    // // Add all induced intravisit dependencies to graphs
    // while (intra_visit_dependencies != NULL) {
    //     struct GRedge_list *current_edges = intra_visit_dependencies;
    //     intra_visit_dependencies = NULL;
    //     (void)current_edges;
    // }

    // TODO: Check for intravisit dependency loops. In that case execute
    // backtracking algorithm

    // Collect all visit metadata (inputs/outputs)
    htable_st *visits_htable = HTnew_Ptr(htable_size);
    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        struct graph_list *graph = HTlookup(graphs_htable, inode);
        struct visits *visits = collect_visits(
            graph->graph, inode, AST_STABLE(node), partition_tables);

        HTinsert(visits_htable, inode, visits);
    }

    for (node_st *nodeset = AST_INODESETS(node); nodeset != NULL;
         nodeset = INODESET_NEXT(nodeset)) {
        struct graph_list *graph = HTlookup(graphs_htable, nodeset);
        if (graph == NULL) { // Empty nodeset
            continue;
        }
        struct visits *visits = collect_visits(
            graph->graph, nodeset, AST_STABLE(node), partition_tables);

        HTinsert(visits_htable, nodeset, visits);
    }

    generate_empty_visits(AST_INODES(node), AST_INODESETS(node),
                          AST_STABLE(node), visits_htable);

    char *log_file_name = STRcat(globals.log_dir, "ag_schedule.log");
    FILE *log_file = fopen(log_file_name, "w");
    if (!log_file) {
        CTI(CTI_ERROR, false, "Could not open file: %s\n", log_file_name);
        CTIabortCompilation();
    }
    MEMfree(log_file_name);

    // Schedule visit sequences
    htable_st *visit_mdata_htable = HTnew_Ptr(htable_size);
    htable_st *visit_stubs_htable = HTnew_Ptr(htable_size);
    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        fprintf(log_file, "### Generating visits for node %s\n",
                ID_ORIG(INODE_NAME(inode)));
        struct graph_list *graph = HTlookup(graphs_htable, inode);

        node_st *visit_sequences = generate_visits(
            graph->graph, inode, AST_STABLE(node), visits_htable,
            visit_mdata_htable, visit_stubs_htable, log_file);
        INODE_VISIT(inode) = visit_sequences;
        fprintf(log_file, "\n\n");
    }

    fclose(log_file);

    // fill in visit stubs
    for (node_st *inode = AST_INODES(node); inode != NULL;
         inode = INODE_NEXT(inode)) {
        fill_visit_stubs(inode, visit_mdata_htable, visit_stubs_htable);
    }

    HTmap(visits_htable, free_visits_map);
    HTdelete(visits_htable);
    HTmap(graphs_htable, delete_graphs);
    HTdelete(graphs_htable);
    HTmap(partition_tables, delete_tables);
    HTdelete(partition_tables);
    HTdelete(visit_mdata_htable);
    HTmap(visit_stubs_htable, free_stubs_map);
    HTdelete(visit_stubs_htable);
    return node;
}
