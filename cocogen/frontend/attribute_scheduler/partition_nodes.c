#include <assert.h>
#include "ccn/ccn.h"
#include "frontend/attribute_scheduler/common.h"
#include "frontend/attribute_scheduler/partition_nodes.h"
#include "frontend/attribute_scheduler/queue.h"
#include "frontend/symboltable.h"
#include "palm/memory.h"

static const int htable_size = 200;

htable_st *partition_nodes(graph_st *graph, node_st *tnode) {
    htable_st *partition_table = HTnew_String(htable_size);
    queue_st *work_queue = QUcreate();
    struct GRnode *node;
    for (node = graph->nodes; node != NULL; node = node->next) {
        if (node->node == tnode) {
            QUinsert(work_queue, node);
        }
    }

    while ((node = QUpop(work_queue))) {
        char *attribute_id = ID_LWR(ATTRIBUTE_NAME(node->attribute));
        bool synthesized = ATTRIBUTE_IS_SYNTHESIZED(node->attribute);
        if (HTlookup(partition_table,
                     attribute_id)) { // Attribute already assigned
            continue;
        }

        struct GRnode_list *deps = GRget_inter_node_dependencies(graph, node);
        if (deps == NULL) {
            HTinsert(partition_table, attribute_id,
                     (void *)(synthesized ? 3UL : 2UL));
            continue;
        }

        // Search highest partition index of dependencies
        size_t max_partition = 0;
        bool assignable = true;
        struct GRnode_list *next;
        for (struct GRnode_list *entry = deps; entry != NULL; entry = next) {
            struct GRnode *dep = entry->node;
            next = entry->next;
            entry = MEMfree(entry); // No longer needed
            size_t dep_partition =
                (size_t)HTlookup(partition_table,
                                 ID_LWR(ATTRIBUTE_NAME(dep->attribute)));

            if (dep_partition == 0) {
                // We need to wait for this dependency before we can assign this
                // node
                assignable = false;
            } else {
                max_partition = dep_partition > max_partition ? dep_partition
                                                              : max_partition;
            }
        }

        if (!assignable) { // Wait for other nodes to complete
            QUinsert(work_queue, node);
            continue;
        }

        bool dep_synthesized = max_partition % 2 == 1;
        if (synthesized == dep_synthesized) {
            HTinsert(partition_table, attribute_id, (void *)max_partition);
        } else {
            HTinsert(partition_table, attribute_id,
                     (void *)(max_partition + 1));
        }
    }

    QUdelete(work_queue);

    return partition_table;
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
    size_t part = (size_t)HTlookup(subtable,
                                   ID_LWR(ATTRIBUTE_NAME(node->attribute)));
    if (part == 0) {
        fprintf(stderr, "NO PARTITION %s.%s: %p.%p\n", ID_ORIG(get_node_name(node->node)), ID_ORIG(ATTRIBUTE_NAME(node->attribute)), (void *)node->node, (void *)node->attribute);
        for (htable_iter_st *iter = HTiterate(subtable); iter != NULL; iter = HTiterateNext(iter)) {
            node_st *attr = HTiterKey(iter);
            size_t part_attr = (size_t) HTiterValue(iter);
            fprintf(stderr, "Candidate %s.%s: %p.%p (%lu)\n", ID_ORIG(get_node_name(ref)), ID_ORIG(ATTRIBUTE_NAME(attr)), (void *)ref, (void *)attr, part_attr);
        }
        fflush(stderr);
    }

    return part;
}

static inline void insert_partition(htable_st *partition_table, node_st *st,
                                      struct GRnode *node, size_t partition) {
    node_st *ref;
    if (NODE_TYPE(node->node) == NT_CHILD) {
        ref = STlookup(st, CHILD_TYPE_REFERENCE(node->node));
    } else {
        ref = node->node;
    }

    assert(NODE_TYPE(ref) == NT_INODE || NODE_TYPE(ref) == NT_INODESET);

    htable_st *subtable = (htable_st *)HTlookup(partition_table, ref);
    assert(subtable != NULL);

    HTinsert(subtable, ID_LWR(ATTRIBUTE_NAME(node->attribute)), (void *)partition);
}

bool repartition_nodes(graph_st *graph, node_st *tnode, node_st *st, htable_st *partition_tables) {
    queue_st *work_queue = QUcreate();
    struct GRnode *node;
    bool changes = false;

    for (node = graph->nodes; node != NULL; node = node->next) {
        if (node->node == tnode) {
            QUinsert(work_queue, node);
        }
    }

    while ((node = QUpop(work_queue))) {
        bool synthesized = ATTRIBUTE_IS_SYNTHESIZED(node->attribute);

        struct GRnode_list *deps = GRget_node_dependencies(graph, node);
        if (deps == NULL) {
            continue;
        }

        size_t partition = lookup_partition(partition_tables, st, node);
        size_t new_partition = partition;
        struct GRnode_list *next;
        for (struct GRnode_list *entry = deps; entry != NULL; entry = next) {
            struct GRnode *dep = entry->node;
            next = entry->next;
            entry = MEMfree(entry); // No longer needed
            size_t dep_partition = lookup_partition(partition_tables, st, dep);

            if (dep_partition > new_partition) {
                bool dep_synthesized = dep_partition % 2 == 1;
                if (synthesized == dep_synthesized) {
                    new_partition = dep_partition;
                } else {
                    new_partition = dep_partition + 1;
                }
            }
        }

        if (new_partition != partition) {
            insert_partition(partition_tables, st, node, new_partition);
            changes = true;
        }
    }

    QUdelete(work_queue);

    return changes;
}
