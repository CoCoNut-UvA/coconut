#pragma once

#include <stdio.h>
#include "ccngen/ast.h"

void FMTprintIndentLevel(FILE *fp, int indent);
char *FMTattributeDefaultVal(enum attribute_type);
char *FMTattributeTypeToString(enum attribute_type);
