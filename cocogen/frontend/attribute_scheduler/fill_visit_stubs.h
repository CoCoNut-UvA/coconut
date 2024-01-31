#pragma once
#include "ccn/ccn.h"
#include "frontend/attribute_scheduler/collect_visits.h"
#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"
#include <stdbool.h>

struct visit_stub {
    node_st *visit_sequence_visit;
    struct visit_stub *next;
};

bool add_visit_stub(htable_st *visit_stubs_htable, struct visit *visit,
                    node_st *visit_sequence_visit);
void free_visit_stubs(struct visit_stub *root);
void fill_visit_stubs(node_st *node, htable_st *visit_mdata_htable,
                      htable_st *visit_stubs_htable);
