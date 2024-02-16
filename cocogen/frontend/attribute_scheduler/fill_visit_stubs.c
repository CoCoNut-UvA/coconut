/**
 * @file
 *
 * fill visit stubs
 *
 *
 */
#include <assert.h>

#include "frontend/attribute_scheduler/collect_visits.h"
#include "frontend/attribute_scheduler/common.h"
#include "frontend/attribute_scheduler/fill_visit_stubs.h"
#include "palm/ctinfo.h"
#include "palm/dbug.h"
#include "palm/hash_table.h"
#include "palm/memory.h"

bool add_visit_stub(htable_st *visit_stubs_htable, struct visit *visit,
                    node_st *visit_sequence_visit) {
    struct visit_stub *stub_head = HTlookup(visit_stubs_htable, visit);
    struct visit_stub *stub = MEMmalloc(sizeof(struct visit_stub));
    stub->visit_sequence_visit = visit_sequence_visit;
    stub->next = stub_head;
    return HTinsert(visit_stubs_htable, visit, stub);
}

void free_visit_stubs(struct visit_stub *root) {
    while (root) {
        struct visit_stub *next = root->next;
        MEMfree(root);
        root = next;
    }
}

void fill_visit_stubs(node_st *node, htable_st *visit_mdata_htable,
                      htable_st *visit_stubs_htable) {
    for (node_st *visit = INODE_VISIT(node); visit; visit = VISIT_NEXT(visit)) {
        struct visit *visit_mdata = HTlookup(visit_mdata_htable, visit);
        assert(visit_mdata != NULL);
        DBUG("[debug] Adding visit %lu for node %s\n", visit_mdata->index,
             ID_LWR(get_node_name(node)));
        for (struct visit_stub *visit_stub =
                 HTlookup(visit_stubs_htable, visit_mdata);
             visit_stub; visit_stub = visit_stub->next) {
            VISIT_SEQUENCE_VISIT_VISIT(visit_stub->visit_sequence_visit) =
                visit;
        }
    }
}
