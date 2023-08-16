#pragma once

#include "palm/ctinfo.h"
#include "ccngen/ast.h"

// Helper to convert an ID into an info object for error reporting.
void id_to_info(node_st *ID, struct ctinfo *info);
