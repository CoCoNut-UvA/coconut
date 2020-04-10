#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-ast.h"
#include "filegen/gen-util.h"

void generate_init_function(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    out("Node *node_init_%s(", nodelwr);
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
    free(nodelwr);
}

void generate_child_struct(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    out("// Node %s Children\n", node->id);
    out("struct CHILDREN_%s {\n", nodeupr);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("    Node *%s;\n", child->id);
    }
    out("};\n\n");
    free(nodeupr);
}

void generate_attribute_struct(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    out("// Node %s Attributes\n", node->id);
    out("struct ATTRIBUTES_%s {\n", nodeupr);
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *type_str = get_attr_str(config, attr);
        out("    %s %s;\n", type_str, attr->id);
    }
    out("};\n\n");
    free(nodeupr);
}

void generate_children_union(Config *config, FILE *fp) {
    out("// Children\n");
    out("union CHILDREN {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        char *nodeupr = strupr(node->id);
        char *nodelwr = strlwr(node->id);
        out("    struct CHILDREN_%s *N_%s;\n", nodeupr, nodelwr);
        free(nodeupr);
        free(nodelwr);
    }
    out("};\n\n");
}

void generate_attributes_union(Config *config, FILE *fp) {
    out("// Attributes\n");
    out("union ATTRIBUTES {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        char *nodeupr = strupr(node->id);
        char *nodelwr = strlwr(node->id);
        out("    struct ATTRIBUTES_%s *N_%s;\n", nodeupr, nodelwr);
        free(nodeupr);
        free(nodelwr);
    }
    out("};\n\n");
}

void generate_macros(Config *config, FILE *fp, Node *node) {
    out("// Macros for Node %s\n", node->id);
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out("#define %s_%s(n) ((n)->children.N_%s->%s)\n", nodeupr, childupr,
            nodelwr, child->id);
        free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out("#define %s_%s(n) ((n)->attribs.N_%s->%s)\n", nodeupr, attrupr,
            nodelwr, attr->id);
        free(attrupr);
    }
    free(nodeupr);
    free(nodelwr);
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
        out("// Constructor for Node %s\n", node->id);
        out("extern ");
        generate_init_function(config, fp, node);
        out(";\n\n");
    }
    out("#endif /* _CCN_AST_H_ */\n");
}

void generate_members(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out("    %s_%s(node) = %s;\n", nodeupr, childupr, child->id);
        free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out("    %s_%s(node) = %s;\n", nodeupr, attrupr, attr->id);
        free(attrupr);
    }
    free(nodeupr);
}

void generate_node_constructor(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    generate_init_function(config, fp, node);
    out("{\n");
    out("    Node *node = node_init_empty();\n");
    out("    node->children.N_%s = mem_alloc(sizeof(struct CHILDREN_%s));\n",
        nodelwr, nodeupr);
    out("    node->attribs.N_%s = mem_alloc(sizeof(struct ATTRIBUTES_%s));\n",
        nodelwr, nodeupr);
    out("    NODE_TYPE(node) = N_%s;\n", nodelwr);
    generate_members(config, fp, node);
    // TODO: Checks here or in another file?
    out("}\n\n");
    free(nodeupr);
    free(nodelwr);
}

void gen_ast_src(Config *config, FILE *fp) {
    out("#include <stdbool.h>\n");
    // TODO: Does the library need to be in the core directory?
    out("#include \"lib/memory.h\"\n");
    out("#include \"generated/ast.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("// Constructor for Node %s\n", node->id);
        generate_node_constructor(config, fp, node);
    }
}