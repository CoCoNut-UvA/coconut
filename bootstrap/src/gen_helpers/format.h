#pragma once

#include <stdio.h>
#include "ccngen/ast.h"

void FMTprintIndentLevel(FILE *fp, int indent);
char *FMTattributeDefaultVal(node_st *node);
char *FMTattributeTypeToString(node_st *attr);