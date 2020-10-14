#include "gen_helpers/format.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

void FMTprintIndentLevel(FILE *fp, int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "    ");
    }
}

char *FMTattributeDefaultVal(node_st *attr)
{
    assert(NODE_TYPE(attr) == NT_ATTRIBUTE);
    switch (ATTRIBUTE_TYPE(attr)) {
    case AT_iint:
        return "0";
    case AT_link:
    case AT_istring:
        return "NULL";
    case AT_link_or_enum:
        return "0";
    }
    assert(false);
}

char *FMTattributeTypeToString(node_st *attr)
{
    assert(NODE_TYPE(attr) == NT_ATTRIBUTE);
    switch (ATTRIBUTE_TYPE(attr)) {
    case AT_iint:
        return "int";
    case AT_link_or_enum:
        return ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(attr));
    case AT_link:
        return "node_st";
    case AT_istring:
        return "char *";
    }
    assert(false);
    return "IMPOSSIBLE VALUE";
}