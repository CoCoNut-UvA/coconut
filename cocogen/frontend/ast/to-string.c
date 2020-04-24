#include "ast/ast.h"
#include "lib/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_attr_type(Attr *attr) {
    char *link_type;

    switch (attr->type) {
    case AT_int:
        return "int";
    case AT_uint:
        return "unsigned int";
    case AT_int8:
        return "int8_t";
    case AT_int16:
        return "int16_t";
    case AT_int32:
        return "int32_t";
    case AT_int64:
        return "int64_t";
    case AT_uint8:
        return "uint8_t";
    case AT_uint16:
        return "uint16_t";
    case AT_uint32:
        return "uint32_t";
    case AT_uint64:
        return "uint64_t";
    case AT_float:
        return "float";
    case AT_double:
        return "double";
    case AT_bool:
        return "bool";
    case AT_string:
        return "char *";
    case AT_link_or_enum:
    case AT_enum:
        return attr->type_id;
    case AT_link:
        link_type = mem_alloc(strlen(attr->type_id) + 10);
        sprintf(link_type, "struct %s *", attr->type_id);
        return link_type;
    }
    return "";
}