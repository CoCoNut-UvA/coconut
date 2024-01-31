/**
 * @file
 *
 * generate visit
 *
 *
 */
#include <assert.h>

#include "frontend/attribute_scheduler/common.h"
#include "frontend/attribute_scheduler/collect_visits.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/memory.h"

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

struct visits *collect_visits(graph_st *graph, node_st *node, node_st *st,
                              htable_st *partition_tables) {
    struct visits *visits = MEMmalloc(sizeof(struct visits));
    visits->visits = NULL;
    visits->length = 0;
    for (struct GRnode *gnode = graph->nodes; gnode != NULL;
         gnode = gnode->next) {
        size_t partition = lookup_partition(partition_tables, st, gnode);
        size_t visit_index = partition / 2 + partition % 2 - 1;
        if (visit_index >= visits->length) {
            visits->visits = MEMrealloc(
                visits->visits, (visit_index + 1) * sizeof(struct visit));
            for (size_t i = visits->length; i <= visit_index; ++i) {
                visits->visits[i] = MEMmalloc(sizeof(struct visit));
                visits->visits[i]->index = i;
                visits->visits[i]->inputs = NULL;
                visits->visits[i]->inputs_length = 0;
                visits->visits[i]->outputs = NULL;
                visits->visits[i]->outputs_length = 0;
            }
            visits->length = visit_index + 1;
        }

        struct visit *visit = visits->visits[visit_index];

        if (gnode->node != node) {
            continue;
        }

        if (ATTRIBUTE_IS_INHERITED(gnode->attribute)) {
            visit->inputs =
                MEMrealloc(visit->inputs,
                           (visit->inputs_length + 1) * sizeof(struct GRnode));
            visit->inputs[visit->inputs_length] = gnode;
            visit->inputs_length = visit->inputs_length + 1;
        } else {
            assert(ATTRIBUTE_IS_SYNTHESIZED(gnode->attribute));

            visit->outputs =
                MEMrealloc(visit->outputs,
                           (visit->outputs_length + 1) * sizeof(struct GRnode));
            visit->outputs[visit->outputs_length] = gnode;
            visit->outputs_length = visit->outputs_length + 1;
        }
    }

    return visits;
}
