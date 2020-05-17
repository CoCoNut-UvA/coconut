#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_default_values(Config *config, FILE *fp, Attr *attr) {
    switch (attr->type) {
    case AT_int:
    case AT_int8:
    case AT_int16:
    case AT_int32:
    case AT_int64:
        out("%i", 0);
        break;
    case AT_uint:
    case AT_uint8:
    case AT_uint16:
    case AT_uint32:
    case AT_uint64:
        out("%u", 0);
        break;
    case AT_float:
    case AT_double:
        out("%f", 0.0);
        break;
    case AT_bool:
        out("%s", "false");
        break;
    case AT_string:
    case AT_link:
        out("NULL");
        break;
    case AT_enum:
        out("%d", 0);
        break;
    }
}

void gen_init_arguments(Config *config, FILE *fp, Node *node) {
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        if (child->construct) {
            if (i > 0) {
                out(", ");
            }
            out("NULL");
        }
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        if (attr->construct) {
            if ((i == 0 && node->children > 0) || i > 0) {
                out(", ");
            }
            gen_default_values(config, fp, attr);
        }
    }
}

void gen_copy_func(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_start_func("Node *copy_%s(Node *arg_node)", nodelwr);
    out("    Node *new_node = node_init_%s(", nodelwr);
    gen_init_arguments(config, fp, node);
    out(");\n");
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        char *copyfunc;
        if (attr->type == AT_string) {
            copyfunc = "ccn_str_cpy";
        } else if (attr->type == AT_link) {
            copyfunc = "node_copy";
        } else {
            out_field("%s_%s(new_node) = %s_%s(arg_node)", nodeupr, attrupr,
                      nodeupr, attrupr);
            mem_free(attrupr);
            continue;
        }
        out_field("%s_%s(new_node) = %s(%s_%s(arg_node))", nodeupr, attrupr,
                  copyfunc, nodeupr, attrupr);
        mem_free(attrupr);
    }
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    out_field("return new_node");
    out_end_func();
    mem_free(nodelwr);
    mem_free(nodeupr);
}

void gen_copy_src(Config *config, FILE *fp) {
    out("#include \"include/core/trav_core.h\"\n");
    out("#include \"include/core/ast_core.h\"\n");
    out("#include \"lib/str.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_copy_func(config, fp, node);
    }
}