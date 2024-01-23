#pragma once
#include <stdio.h>
#include "ccn/ccn.h"
#include "frontend/attribute_scheduler/collect_visits.h"
#include "frontend/attribute_scheduler/graph.h"
#include "palm/hash_table.h"

void free_visits(struct visits *visits);
node_st *generate_visits(graph_st *graph, node_st *node, node_st *st,
                         htable_st *visits_htable,
                         htable_st *visit_mdata_htable,
                         htable_st *visit_stubs_htable, FILE *log_file);
