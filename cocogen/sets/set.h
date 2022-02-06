#pragma once

#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"


node_st *SETIDinsert(node_st *literal, node_st *id);
bool SETIDcontains(node_st *literal, node_st *id);
node_st *SETIDunion(node_st *set1, node_st *set2);
node_st *SETIDintersect(node_st *dst, node_st *src);
node_st *SETIDdifference(node_st *left, node_st *right);
