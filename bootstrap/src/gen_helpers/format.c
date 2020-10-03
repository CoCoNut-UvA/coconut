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

char *FMTattributeDefaultVal(node_st *node)
{
    assert(NODE_TYPE(node) == NT_ATTRIBUTE);
    switch (ATTRIBUTE_TYPE(node)) {
    case AT_iint:
        return "0";
    case AT_link:
        return "NULL";
        break;
    case AT_link_or_enum:
        assert(false);
        return "NULL";
    }
    assert(false);
    return "IMPOSSIBLE VALUE";
}

char *FMTattributeTypeToString(node_st *attr)
{
    assert(NODE_TYPE(attr) == NT_ATTRIBUTE);
    switch (ATTRIBUTE_TYPE(attr)) {
    case AT_iint:
        return "int";
    case AT_link_or_enum:
        assert(false);
    case AT_link:
        return "node_st";
    }
    assert(false);
    return "IMPOSSIBLE VALUE";
}