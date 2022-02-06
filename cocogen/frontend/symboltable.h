#pragma once

#include "ccngen/ast.h"

node_st *STlookup(node_st *ste, node_st *node);
node_st *STadd(node_st *ste, node_st *key, node_st *val);
