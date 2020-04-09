#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-ast-util.h"
#include "filegen/gen-ast.h"

void generate_child_struct(Config *config, FILE *fp, Node *node) {
    out("// Node %s Children\n", node->id);
    out("struct CHILDREN_%s {\n", node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("    Node *%s;\n", child->id);
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
        } else if (attr->type == AT_link || (attr->type == AT_link_or_enum &&
                                             type_is_link(config, attr))) {
            out("    Node *%s;\n", attr->id);
        } else if (attr->type == AT_enum || attr->type == AT_link_or_enum) {
            out("    %s %s;\n", attr->type_id, attr->id);
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
    out("#ifndef _CCN_AST_H_\n");
    out("#define _CCN_AST_H_\n\n");
    out("#include \"<stdbool.h>\"\n");
    out("#include \"core/ast_core.h\"\n");
    out("\n");
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
    out("#endif /* _CCN_AST_H_ */\n");
}

void gen_ast_src(Config *config, FILE *fp) {}