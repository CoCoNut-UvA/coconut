/**
 * @file
 *
 * generate visit
 *
 *
 */
#include <assert.h>

#include "frontend/attribute_scheduler/collect_visits.h"
#include "frontend/attribute_scheduler/common.h"
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
        CTI(CTI_ERROR, true, "Could not find partition of attribute %s.%s",
            ID_ORIG(get_node_name(node->node)),
            ID_ORIG(ATTRIBUTE_NAME(node->attribute)));
        CCNerrorAction();
        DATA_SAV_GET()->errors += 1;
        return 0;
    }
    size_t part = (size_t)HTlookup(subtable,
                                   ID_LWR(ATTRIBUTE_NAME(node->attribute)));
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
        if (gnode->node != node) {
            continue;
        }
        size_t partition = lookup_partition(partition_tables, st, gnode);
        size_t visit_index;
        if (partition == 0) {
            visit_index = 0; // Finish action for error checking
        } else {
            visit_index = partition / 2 - 1;
        }
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

static bool node_has_visit(node_st *node, node_st *st,
                           htable_st *visits_htable) {
    for (node_st *child = INODE_ICHILDREN(node); child != NULL;
         child = CHILD_NEXT(child)) {
        node_st *child_ref = get_node_type(child, st);
        struct visits *visits = HTlookup(visits_htable, child_ref);
        assert(visits != NULL);
        if (visits->length > 0) {
            return true;
        }
    }

    return false;
}

static bool nodeset_has_visit(node_st *expr, node_st *st,
                              htable_st *visits_htable) {
    if (expr == NULL) {
        return NULL;
    }

    assert(NODE_TYPE(expr) == NT_SETLITERAL);

    node_st *node = STlookup(st, SETLITERAL_REFERENCE(expr));
    struct visits *visits = HTlookup(visits_htable, node);
    if (visits->length > 0) {
        return true;
    }

    return nodeset_has_visit(SETLITERAL_LEFT(expr), st, visits_htable) ||
           nodeset_has_visit(SETLITERAL_RIGHT(expr), st, visits_htable);
}

static inline void add_empty_visit(struct visits *visits) {
    visits->visits = MEMrealloc(visits->visits, 1 * sizeof(struct visit));
    visits->visits[0] = MEMmalloc(sizeof(struct visit));
    visits->visits[0]->index = 0;
    visits->visits[0]->inputs = NULL;
    visits->visits[0]->inputs_length = 0;
    visits->visits[0]->outputs = NULL;
    visits->visits[0]->outputs_length = 0;
    visits->length = 1;
}

/* We want every visit to be reachable from the root node. So if a child node
   has any visit, while the current node doesn't, we add an empty visit with
   the purpose of calling the child visits.
*/
void generate_empty_visits(node_st *nodes, node_st *nodesets, node_st *st,
                           htable_st *visits_htable) {
    size_t changes;

    do {
        changes = 0;

        for (node_st *node = nodes; node != NULL; node = INODE_NEXT(node)) {
            struct visits *visits = HTlookup(visits_htable, node);
            if (visits == NULL || visits->length > 0) {
                continue;
            }

            if (node_has_visit(node, st, visits_htable)) {
                add_empty_visit(visits);
                changes += 1;
            }
        }

        for (node_st *nodeset = nodesets; nodeset;
             nodeset = INODESET_NEXT(nodeset)) {
            struct visits *visits = HTlookup(visits_htable, nodeset);
            if (visits == NULL || visits->length > 0) {
                continue;
            }

            if (nodeset_has_visit(INODESET_EXPR(nodeset), st, visits_htable)) {
                add_empty_visit(visits);
                changes += 1;
            }
        }
    } while (changes > 0); // Keep adding visits until there are no more changes
}
