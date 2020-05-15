#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_free_func(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_comment("%s", node->id);
    out_start_func("Node *free_%s(Node *arg_node)", nodelwr);
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        char *freefunc;
        if (attr->type == AT_string) {
            freefunc = "mem_free";
        } else if (attr->type == AT_link) {
            freefunc = "node_free";
        } else {
            continue;
        }
        out_field("%s(%s_%s(arg_node))", freefunc, nodeupr, attrupr);
        mem_free(attrupr);
    }
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    out_field("mem_free(arg_node->data.N_%s)", nodelwr);
    out_field("mem_free(arg_node)");
    out_field("return arg_node");
    out_end_func();
    mem_free(nodelwr);
    mem_free(nodeupr);
}

void gen_free_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"include/core/trav_core.h\"\n");
    out("#include \"include/core/ast_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_free_func(config, fp, node);
    }
}