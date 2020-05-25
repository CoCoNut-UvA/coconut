#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_free_func(Config *config, FILE *fp, Node *node) {
    out_comment("%s", node->id->orig);
    out_start_func("Node *free_%s(Node *arg_node)", node->id->lwr);
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        char *freefunc;
        if (attr->type == AT_link) {
            if (!attr->construct) {
                out_begin_if("%s_%s(arg_node)", node->id->upr, attr->id->upr);
            }
            out_field("free_%s(%s_%s(arg_node))", attr->type_id->lwr,
                      node->id->upr, attr->id->upr);
            if (!attr->construct) {
                out_end_if();
            }
        }
    }
    out_field("mem_free(arg_node->data.N_%s)", node->id->lwr);
    out_field("mem_free(arg_node)");
    out_field("return arg_node");
    out_end_func();
}

void gen_free_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"ccn/actions_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_free_func(config, fp, node);
    }
}