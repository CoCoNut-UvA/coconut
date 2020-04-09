#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-ast.h"
#include "filegen/gen-util.h"

void generate_init_function(Config *config, FILE *fp, Node *node) {
    out("Node *node_init_%s(", node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("Node *%s", child->id);
        if (i != array_size(node->children) - 1 ||
            array_size(node->attrs) > 0) {
            out(", ");
        }
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        out("%s %s", get_attr_str(config, attr), attr->id);
        if (i != array_size(node->attrs) - 1) {
            out(", ");
        }
    }
    out(")");
}

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
        char *type_str = get_attr_str(config, attr);
        out("    %s %s;\n", type_str, attr->id);
    }
    out("};\n\n");
}

void generate_children_union(Config *config, FILE *fp) {
    out("// Children\n");
    out("union CHILDREN {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("    struct CHILDREN_%s *N_%s;\n", node->id, node->id);
    }
    out("};\n\n");
}

void generate_attributes_union(Config *config, FILE *fp) {
    out("// Attributes\n");
    out("union ATTRIBUTES {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("    struct ATTRIBUTES_%s *N_%s;\n", node->id, node->id);
    }
    out("};\n\n");
}

void generate_macros(Config *config, FILE *fp, Node *node) {
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("#define %s_%s(n) ((n)->children.N_%s->%s\n", node->id, child->id,
            node->id, child->id);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        out("#define %s_%s(n) ((n)->attribs.N_%s->%s\n", node->id, attr->id,
            node->id, attr->id);
    }
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
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        generate_macros(config, fp, node);
        out("extern ");
        generate_init_function(config, fp, node);
        out(";\n\n");
    }
    out("#endif /* _CCN_AST_H_ */\n");
}

void generate_empty_node(Config *config, FILE *fp) {
    out("static Node *node_init_empty() {\n");
    out("    Node *node = (Node *)mem_alloc(sizeof(Node));\n");
    out("    return node;\n");
    out("}\n\n");
}

void generate_members(Config *config, FILE *fp, Node *node) {
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("    %s_%s(node) = %s;\n", node->id, child->id, child->id);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        out("    %s_%s(node) = %s;\n", node->id, attr->id, attr->id);
    }
}

void generate_node_constructor(Config *config, FILE *fp, Node *node) {
    generate_init_function(config, fp, node);
    out("{\n");
    out("    Node *node = node_init_empty();\n");
    out("    node->children.N_%s = mem_alloc(sizeof(struct CHILDREN_%s));\n",
        node->id, node->id);
    out("    node->attribs.N_%s = mem_alloc(sizeof(struct ATTRIBUTES_%s));\n",
        node->id, node->id);
    out("    NODE_TYPE(node) = N_%s;\n", node->id);
    generate_members(config, fp, node);
    // TODO: Checks here or in another file?
    out("}\n\n");
}

void gen_ast_src(Config *config, FILE *fp) {
    // TODO: Does the library need to be in the core directory?
    out("#include \"lib/memory.h\"\n");
    out("#include \"generated/ast.h\"\n");
    out("\n");
    generate_empty_node(config, fp);
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        generate_node_constructor(config, fp, node);
    }
}