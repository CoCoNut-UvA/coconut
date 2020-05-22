#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/util.h"
#include "lib/print.h"
#include "lib/smap.h"

void generate_node_header_includes(Config *config, FILE *fp, Node *node) {
    bool using_bool = false;
    bool using_int = false;
    bool using_enum = false;

    out("\n");

    // Create a map of child node types, so that there aren't duplicate
    // includes.
    smap_t *map = smap_init(32);

    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        if (smap_retrieve(map, child->type->orig) == NULL) {
            out("typedef struct %s %s;\n", child->type->orig,
                child->type->orig);
            smap_insert(map, child->type->orig, child);
        }
    }

    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        switch (attr->type) {
        case AT_bool:
            using_bool = true;
            break;
        case AT_int8:
        case AT_int16:
        case AT_int32:
        case AT_int64:
        case AT_uint8:
        case AT_uint16:
        case AT_uint32:
        case AT_uint64:
            using_int = true;
            break;
        case AT_enum:
            using_enum = true;
            break;
        default:
            break;
        }

        if (smap_retrieve(map, attr->id->orig) == NULL) {
            switch (attr->type) {
            case AT_link:
                out("struct %s;\n", attr->type_id->orig);
                break;
            default:
                break;
            }
            smap_insert(map, attr->id->orig, attr);
        }
    }

    smap_free(map);

    if (using_enum)
        out("#include \"enum.h\"\n");
    if (using_int)
        out("#include <stdint.h>\n");
    if (using_bool)
        out("#include <stdbool.h>\n");
}

void print_indent_level(int indent_level, FILE *fp) {
    for (int i = 0; i < indent_level; i++) {
        fprintf(fp, "    ");
    }
}

/**
 * This this function goes over each node in the traversal and compares it to
 * the node given to the function. If the two nodes have an equal ID, then the
 * node is a valid node in the traversal.
 */
bool is_traversal_node(Traversal *trav, Node *node) {
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *travnode = array_get(trav->nodes, i);
        if (strcmp(travnode->id->orig, node->id->orig) == 0) {
            return true;
        }
    }
    return false;
}

char *attr_default_value(Config *config, FILE *fp, enum AttrType type) {
    switch (type) {
    case AT_int:
    case AT_int8:
    case AT_int16:
    case AT_int32:
    case AT_int64:
    case AT_enum:
    case AT_uint:
    case AT_uint8:
    case AT_uint16:
    case AT_uint32:
    case AT_uint64:
        return "0";
        break;
    case AT_float:
    case AT_double:
        return "0.0";
        break;
    case AT_bool:
        return "false";
        break;
    case AT_string:
    case AT_link:
        return "NULL";
        break;
    case AT_link_or_enum:
        print_user_error("attr_default_value",
                         "attrtype is somehow not converted to link or enum");
        return "NULL";
        break;
    }
}