#include "frontend/attribute_scheduler/partition_nodes.h"
#include "frontend/attribute_scheduler/queue.h"
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

    return partition_table;
}
