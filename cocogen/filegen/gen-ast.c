#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-ast.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

void gen_init_function(Config *config, FILE *fp, Node *node) {
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

void gen_struct(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    out_comment("Node %s Attributes", node->id);
    out_struct("DATA_%s", nodeupr);
    out_comment("Children");
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out_field("Node *%s", child->id);
    }
    out_comment("Attributes");
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *type_str = get_attr_str(config, attr);
        out_field("%s %s", type_str, attr->id);
    }
    out_struct_end();
    free(nodeupr);
}

void gen_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("DATA");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        char *nodeupr = strupr(node->id);
        char *nodelwr = strlwr(node->id);
        out_field("struct DATA_%s *N_%s", nodeupr, nodelwr);
        free(nodeupr);
        free(nodelwr);
    }
    out_struct_end();
}

void gen_macros(Config *config, FILE *fp, Node *node) {
    out_comment("Macros for Node %s", node->id);
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out("#define %s_%s(n) ((n)->data->N_%s->%s)\n", nodeupr, childupr,
            nodelwr, child->id);
        free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out("#define %s_%s(n) ((n)->data->N_%s->%s)\n", nodeupr, attrupr,
            nodelwr, attr->id);
        free(attrupr);
    }
    free(nodeupr);
    free(nodelwr);
}

void gen_ast_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_AST_H_\n");
    out("#define _CCN_AST_H_\n\n");
    out("#include <stdbool.h>\n");
    out("\n");
    out("#include \"core/ast_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        gen_struct(config, fp, node);
    }
    gen_union(config, fp);
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        gen_macros(config, fp, node);
        out_comment("Constructor for Node %s", node->id);
        out("extern ");
        gen_init_function(config, fp, node);
        out(";\n\n");
    }
    out("#endif /* _CCN_AST_H_ */\n");
}

void gen_members(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(node) = %s", nodeupr, childupr, child->id);
        free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out_field("%s_%s(node) = %s", nodeupr, attrupr, attr->id);
        free(attrupr);
    }
    free(nodeupr);
}

void gen_node_constructor(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    gen_init_function(config, fp, node);
    out_start_func_field();
    out_field("Node *node = node_init()");
    out_field("node->data->N_%s = mem_alloc(sizeof(struct DATA_%s))", nodelwr,
              nodeupr);
    out_field("NODE_TYPE(node) = " NT_ENUM_PREFIX "%s", nodelwr);
    gen_members(config, fp, node);
    // TODO: Checks here or in another file?
    out_end_func();
    free(nodeupr);
    free(nodelwr);
}

void gen_ast_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"ast.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("// Constructor for Node %s\n", node->id);
        gen_node_constructor(config, fp, node);
    }
}