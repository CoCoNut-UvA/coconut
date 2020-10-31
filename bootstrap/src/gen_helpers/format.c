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

char *FMTattributeDefaultVal(enum attribute_type type)
{
    switch (type) {
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

char *FMTattributeTypeToString(enum attribute_type type)
{
    switch (type) {
    case AT_iint:
        return "int";
    case AT_link_or_enum:
        assert(false);
    case AT_link:
        return "node_st";
    case AT_istring:
        return "char *";
    }
    assert(false);
    return "IMPOSSIBLE VALUE";
}

