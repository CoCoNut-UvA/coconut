#include "filegen/util.h"
#include "ast/ast.h"
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
        if (smap_retrieve(map, child->type) == NULL) {
            out("typedef struct %s %s;\n", child->type, child->type);
            smap_insert(map, child->type, child);
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

        if (smap_retrieve(map, attr->id) == NULL) {
            switch (attr->type) {
            case AT_link:
                out("struct %s;\n", attr->type_id);
                break;
            default:
                break;
            }
            smap_insert(map, attr->id, attr);
        }
    }

    smap_free(map);

    if (using_enum)
        out("#include \"generated/enum.h\"\n");
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
