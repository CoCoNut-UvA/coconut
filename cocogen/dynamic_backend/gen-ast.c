#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_init_function(Config *config, FILE *fp, Node *node) {
    out("Node *node_init_%s(", node->id->lwr);
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        if (child->construct) {
            if (i > 0) {
                out(", ");
            }
            out("Node *%s", child->id->lwr);
        }
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        if (attr->construct) {
            if ((i == 0 && node->children > 0) || i > 0) {
                out(", ");
            }
            if (attr->type == AT_link) {
                out("Node *%s", attr->id->lwr);
            } else {
                out("%s %s", str_attr_type(attr), attr->id->lwr);
            }
        }
    }
    out(")");
}

void gen_node_struct(Config *config, FILE *fp, Node *node) {
    out_struct("NODE_DATA_%s", node->id->upr);
    if (node->children) {
        out_union("NODE_CHILDREN_%s", node->id->upr);
        out_struct("NODE_CHILDREN_%s_STRUCT", node->id->upr);
        print_indent_level(indent, fp);
        out("Node ");
        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            out("*%s", child->id->lwr);
            if (i != array_size(node->children) - 1) {
                out(", ");
            } else {
                out(";\n");
            }
        }
        out_typedef_struct_end("%s_children_s", node->id->lwr);
        out_field("Node *%s_children_a[%ld]", node->id->lwr,
                  array_size(node->children));
        out_typedef_struct_end("%s_children", node->id->lwr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        if (attr->type == AT_link) {
            out_field("Node *%s", attr->id->lwr);
        } else {
            out_field("%s %s", str_attr_type(attr), attr->id->lwr);
        }
    }
    out_struct_end();
}

void gen_node_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("NODE_DATA");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out_field("struct NODE_DATA_%s *N_%s", node->id->upr, node->id->lwr);
    }
    out_struct_end();
}

void gen_node_macros(Config *config, FILE *fp, Node *node) {
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("#define %s_%s(n) "
            "((n)->data.N_%s->%s_children.%s_children_s.%s)\n",
            node->id->upr, child->id->upr, node->id->lwr, node->id->lwr,
            node->id->lwr, child->id->lwr);
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        out("#define %s_%s(n) ((n)->data.N_%s->%s)\n", node->id->upr,
            attr->id->upr, node->id->lwr, attr->id->lwr);
    }
    out("\n");
}

void gen_ast_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_AST_H_\n");
    out("#define _CCN_AST_H_\n\n");
    out("\n");
    out("#include \"ccn/types.h\"\n");
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
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        if (child->construct) {
            out_field("%s_%s(node) = %s", node->id->upr, child->id->upr,
                      child->id->lwr);
        } else {
            out_field("%s_%s(node) = NULL", node->id->upr, child->id->upr);
        }
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        if (attr->construct) {
            out_field("%s_%s(node) = %s", node->id->upr, attr->id->upr,
                      attr->id->lwr);
        } else {
            out_field("%s_%s(node) = %s", node->id->upr, attr->id->upr,
                      attr_default_value(config, fp, attr->type));
        }
    }
}

void gen_node_constructor(Config *config, FILE *fp, Node *node) {
    gen_init_function(config, fp, node);
    out_start_func_field();
    out_field("Node *node = node_init()");
    out_field("node->data.N_%s = mem_alloc(sizeof(struct NODE_DATA_%s))",
              node->id->lwr, node->id->upr);
    out_field("NODE_TYPE(node) = " NT_ENUM_PREFIX "%s", node->id->lwr);
    if (node->children) {
        out_field(
            "NODE_CHILDREN(node) = node->data.N_%s->%s_children.%s_children_a",
            node->id->lwr, node->id->lwr, node->id->lwr);
        out_field("NODE_NUMCHILDREN(node) = %ld", array_size(node->children));
    }
    gen_members(config, fp, node);
    out_field("return node");
    out_end_func();
}

void gen_ast_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"ccn/ast_core.h\"\n");
    out("#include \"ccn/trav_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("// Constructor for Node %s\n", node->id->lwr);
        gen_node_constructor(config, fp, node);
    }
}