/**
 * @file
 *
 * generate visit sequences
 *
 *
 */
#include <assert.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "frontend/attribute_scheduler/generate_visit_sequences.h"
#include "frontend/attribute_scheduler/graph.h"
#include "frontend/attribute_scheduler/queue.h"
#include "frontend/symboltable.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

static const int htable_size = 200;

struct dependency {
    struct GRnode *dependency;
    struct dependency *next;
};

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

static inline size_t lookup_partition(htable_st *partition_table, node_st *st,
                                      struct GRnode *node) {
    node_st *ref;
    if (NODE_TYPE(node->node) == NT_CHILD) {
        ref = STlookup(st, CHILD_TYPE_REFERENCE(node->node));
    } else {
        ref = node->node;
    }

    assert(NODE_TYPE(ref) == NT_INODE || NODE_TYPE(ref) == NT_INODESET);

    htable_st *subtable = (htable_st *)HTlookup(partition_table, ref);
    if (subtable == NULL) {
        return 0;
    }
    size_t part = (size_t)HTlookup(subtable, node->attribute);
    if (part == 0) {
        CTI(CTI_ERROR, true, "Could not find partition of attribute %s.%s",
            ID_ORIG(get_node_name(node->node)),
            ID_ORIG(ATTRIBUTE_NAME(node->attribute)));
        CCNerrorAction();
        DATA_SAV_GET()->errors += 1;
    }

    return part;
}

struct dependency *find_dependencies(graph_st *graph, queue_st *queue, struct GRnode *node) {
    struct dependency *deps = NULL;

    for (struct QUitem *dep_item = queue->top; dep_item; dep_item = dep_item->next) {
        struct GRnode *dep_node = dep_item->item;
        if (dep_node == node) {
            continue;
        }

        if (GRlookup_edge(graph, dep_node, node)) {
            struct dependency *dep = MEMmalloc(sizeof(struct dependency));
            dep->dependency = dep_node;
            dep->next = deps;
            deps = dep;
        }
    }

    return deps;
}

bool has_unscheduled_dependencies(htable_st *dependencies, htable_st *scheduled, struct GRnode *node) {
    for (struct dependency *dep = HTlookup(dependencies, node); dep; dep = dep->next) {
        if (HTlookup(scheduled, dep) == NULL) {
            return true;
        }
    }
    return false;
}

node_st *generate_visit_sequences(graph_st *graph, node_st *node, node_st *st, htable_st *partition_tables) {
    queue_st **queues = NULL;
    size_t queues_length = 0;
    for (struct GRnode *gnode = graph->nodes; gnode != NULL; gnode = gnode->next) {
        size_t partition = lookup_partition(partition_tables, st, gnode);

        if (partition >= queues_length) {
            queues = MEMrealloc(queues, (partition + 1) * sizeof(queue_st));
            for (size_t i = queues_length; i <= partition; ++i) {
                queues[i] = QUcreate();
            }
            queues_length = partition + 1;
        }

        QUinsert(queues[partition], gnode);
    }

    htable_st *scheduled = HTnew_Ptr(htable_size);
    htable_st *dependencies = HTnew_Ptr(htable_size);

    node_st *visit_head = NULL;
    node_st *visit_tail = NULL;

    for (size_t partition = 0; partition < queues_length; ++partition) {
        node_st *sequence_head = NULL;
        node_st *sequence_tail = NULL;

        queue_st *queue = queues[partition];
        struct GRnode *gnode;
        /* Store all inter-partition dependencies in dependencies */
        for (struct QUitem *item = queue->top; item; item = item->next) {
            gnode = item->item;
            struct dependency *deps = find_dependencies(graph, queue, gnode);
            HTinsert(dependencies, gnode, deps);
        }

        while ((gnode = QUpop(queue)) != NULL) {
            assert(!HTlookup(scheduled, gnode));

            if (has_unscheduled_dependencies(dependencies, scheduled, gnode)) {
                QUinsert(queue, gnode);
                continue;
            }

            HTinsert(scheduled, gnode, (void *)1);
        }

        (void) sequence_head;
        (void) sequence_tail;
    }

    HTdelete(scheduled);

    (void) node;
    (void) visit_tail;
    return visit_head;
}
