#include "gen_helpers/format.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void FMTprintIndentLevel(FILE *fp, int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "    ");
    }
}

char *FMTattributeDefaultVal(enum attribute_type type)
{
    switch (type) {
    case AT_uint8:
    case AT_uint16:
    case AT_uint32:
    case AT_uint64:
    case AT_uint:
    case AT_int8:
    case AT_int16:
    case AT_int32:
    case AT_int64:
    case AT_int:
        return "0";
    case AT_float:
    case AT_double:
        return "0.0";
    case AT_bool:
        return "false";
    case AT_link:
    case AT_string:
        return "NULL";
    case AT_link_or_enum:
        return "0";
    case AT_user:
    default:
        err(EXIT_FAILURE, "AT_user is a special type and not handled by this function.");
    }
    assert(false);
    return NULL;
}

char *FMTattributeTypeToString(enum attribute_type type)
{
    switch (type) {
    case AT_int8:
        return "int8_t";
    case AT_int16:
        return "int16_t";
    case AT_int32:
        return "int32_t";
    case AT_int64:
        return "int64_t";
    case AT_int:
        return "int";
    case AT_uint8:
        return "uint8_t";
    case AT_uint16:
        return "uint16_t";
    case AT_uint32:
        return "uint32_t";
    case AT_uint64:
        return "uint64_t";
    case AT_uint:
        return "unsigned int";
    case AT_float:
        return "float";
    case AT_double:
        return "double";
    case AT_bool:
        return "bool";
    case AT_link:
        return "node_st";
    case AT_string:
        return "char *";
    case AT_link_or_enum:
        return "enum";
    case AT_user:
    default:
        err(EXIT_FAILURE, "AT_user is a special type and not handled by this function.");
    }
    assert(false);
    return NULL;
}

