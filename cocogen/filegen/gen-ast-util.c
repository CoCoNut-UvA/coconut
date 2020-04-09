#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/genmacros.h"

#include "filegen/gen-ast-util.h"

char *attrtype_to_str(enum AttrType type) {
    char *type_str = NULL;
    switch (type) {
    case AT_int:
        type_str = "int";
        break;
    case AT_uint:
        type_str = "uint";
        break;
    case AT_int8:
        type_str = "int8";
        break;
    case AT_int16:
        type_str = "int16";
        break;
    case AT_int32:
        type_str = "int32";
        break;
    case AT_int64:
        type_str = "int64";
        break;
    case AT_uint8:
        type_str = "uint8";
        break;
    case AT_uint16:
        type_str = "int8";
        break;
    case AT_uint32:
        type_str = "int32";
        break;
    case AT_uint64:
        type_str = "int64";
        break;
    case AT_float:
        type_str = "float";
        break;
    case AT_double:
        type_str = "double";
        break;
    case AT_bool:
        type_str = "bool";
        break;
    case AT_string:
        type_str = "char*";
        break;
    case AT_link_or_enum:
        break;
    case AT_link:
        type_str = "node*";
        break;
    case AT_enum:
        break;
    }

    return type_str;
}

bool type_is_link(Config *config, Attr *attr) {
    // This is terrible but this is the frontend's fault. It is necessary
    // because we want to address nodes dynamically. The current frontend
    // addresses links with their respective "static" struct declaration, which
    // is not what we want. Instead, the resulting type should become "node *".
    bool islink = false;
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        if (strcmp(attr->type_id, node->id) == 0) {
            islink = true;
            break;
        }
    }
    return islink;
}