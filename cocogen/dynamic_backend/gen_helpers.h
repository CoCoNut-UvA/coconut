#pragma once

#include "ccngen/ast.h"

char *DGHpassFuncName(node_st *pass);
char *DGHattributeField(node_st *attr);
char *DGHattributeAccess(node_st *node, node_st *attr, char *arg);
