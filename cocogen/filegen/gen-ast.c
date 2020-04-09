#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-ast.h"

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

void generate_child_struct(Config *config, FILE *fp, Node *node) {
    out("// Node %s Children\n", node->id);
    out("struct CHILDREN_%s {\n", node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("    node *%s;\n", child->id);
    }
    out("};\n\n");
}

void generate_attribute_struct(Config *config, FILE *fp, Node *node) {
    out("// Node %s Attributes\n", node->id);
    out("struct ATTRIBUTES_%s {\n", node->id);
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *type_str = attrtype_to_str(attr->type);
        if (type_str) {
            out("    %s %s;\n", type_str, attr->id);
        } else if (attr->type == AT_enum) {
            out("    %s %s;\n", attr->type_id, attr->id);
        } else if (attr->type == AT_link_or_enum) {
            // This is terrible but this is the frontend's fault
            bool isnode = false;
            for (int i = 0; i < array_size(config->nodes); ++i) {
                Node *node = (Node *)array_get(config->nodes, i);
                if (strcmp(attr->type_id, node->id) == 0) {
                    isnode = true;
                    break;
                }
            }
            if (isnode) {
                out("    node *%s;\n", attr->id);
            } else {
                out("    %s %s;\n", attr->type_id, attr->id);
            }
        }
    }
    out("};\n\n");
}

void generate_children_union(Config *config, FILE *fp) {
    out("// Children\n");
    out("union CHILDREN {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("    struct CHILDREN_%s *%s;\n", node->id, node->id);
    }
    out("};\n\n");
}

void generate_attributes_union(Config *config, FILE *fp) {
    out("// Attributes\n");
    out("union ATTRIBUTES {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("    struct ATTRIBUTES_%s *%s;\n", node->id, node->id);
    }
    out("};\n\n");
}

void gen_ast_header(Config *config, FILE *fp) {
    out("#include \"<stdbool.h>\"\n");
    out("#include \"core/ast_core.h\"\n");
    out("\n");
    out("typedef struct NODE node;\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        generate_child_struct(config, fp, node);
    }
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        generate_attribute_struct(config, fp, node);
    }
    generate_children_union(config, fp);
    generate_attributes_union(config, fp);
}

void gen_ast_src(Config *config, FILE *fp) {}