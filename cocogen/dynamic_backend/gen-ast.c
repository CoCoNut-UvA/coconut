#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/to-string.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

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
        if (attr->type == AT_link) {
            out("Node *%s", attr->id);
        } else {
            out("%s %s", str_attr_type(attr), attr->id);
        }
        if (i != array_size(node->attrs) - 1) {
            out(", ");
        }
    }
    out(")");
    mem_free(nodelwr);
}

void gen_node_struct(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    out_struct("NODE_DATA_%s", nodeupr);
    if (node->children) {
        out_union("NODE_CHILDREN_%s", nodeupr);
        out_struct("NODE_CHILDREN_%s_STRUCT", nodeupr);
        print_indent_level(indent, fp);
        out("Node ");
        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            out("*%s", child->id);
            if (i != array_size(node->children) - 1) {
                out(", ");
            } else {
                out(";\n");
            }
        }
        out_typedef_struct_end("%s_children_s", nodelwr);
        out_field("Node *%s_children_a[%ld]", nodelwr,
                  array_size(node->children));
        out_typedef_struct_end("%s_children", nodelwr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        if (attr->type == AT_link) {
            out_field("Node *%s", attr->id);
        } else {
            out_field("%s %s", str_attr_type(attr), attr->id);
        }
    }
    out_struct_end();
    mem_free(nodeupr);
    mem_free(nodelwr);
}

void gen_node_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("NODE_DATA");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        char *nodeupr = strupr(node->id);
        char *nodelwr = strlwr(node->id);
        out_field("struct NODE_DATA_%s *N_%s", nodeupr, nodelwr);
        mem_free(nodeupr);
        mem_free(nodelwr);
    }
    out_struct_end();
}

void gen_node_macros(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out("#define %s_%s(n) "
            "((n)->data.N_%s->%s_children.%s_children_s.%s)\n",
            nodeupr, childupr, nodelwr, nodelwr, nodelwr, child->id);
        mem_free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out("#define %s_%s(n) ((n)->data.N_%s->%s)\n", nodeupr, attrupr,
            nodelwr, attr->id);
        mem_free(attrupr);
    }
    out("\n");
    mem_free(nodeupr);
    mem_free(nodelwr);
}

void gen_ast_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_AST_H_\n");
    out("#define _CCN_AST_H_\n\n");
    out("#include <stdbool.h>\n");
    out("\n");
    out("#include \"include/core/types.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        gen_node_struct(config, fp, node);
    }
    gen_node_union(config, fp);
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        gen_init_function(config, fp, node);
        out(";\n");
    }
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        gen_node_macros(config, fp, node);
    }
    out("\n");
    out("#endif /* _CCN_AST_H_ */\n");
}

void gen_members(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(node) = %s", nodeupr, childupr, child->id);
        mem_free(childupr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        out_field("%s_%s(node) = %s", nodeupr, attrupr, attr->id);
        mem_free(attrupr);
    }
    mem_free(nodeupr);
}

void gen_node_constructor(Config *config, FILE *fp, Node *node) {
    char *nodeupr = strupr(node->id);
    char *nodelwr = strlwr(node->id);
    gen_init_function(config, fp, node);
    out_start_func_field();
    out_field("Node *node = node_init()");
    out_field("node->data.N_%s = mem_alloc(sizeof(struct NODE_DATA_%s))",
              nodelwr, nodeupr);
    out_field("NODE_TYPE(node) = " NT_ENUM_PREFIX "%s", nodelwr);
    if (node->children) {
        out_field(
            "NODE_CHILDREN(node) = node->data.N_%s->%s_children.%s_children_a",
            nodelwr, nodelwr, nodelwr);
        out_field("NODE_NUMCHILDREN(node) = %ld", array_size(node->children));
    }
    gen_members(config, fp, node);
    // TODO: Checks here or in another file?
    out_field("return node");
    out_end_func();
    mem_free(nodeupr);
    mem_free(nodelwr);
}

void gen_ast_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"include/core/ast_core.h\"\n");
    out("#include \"include/core/trav_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("// Constructor for Node %s\n", node->id);
        gen_node_constructor(config, fp, node);
    }
}