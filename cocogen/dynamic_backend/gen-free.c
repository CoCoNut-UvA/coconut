#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "gen-functions.h"

static int indent = 0;

void gen_free_vtable(Config *config, FILE *fp) {
    out("TravFunc vtable_free[_NT_SIZE] = { &trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        out("&free_%s, ", nodelwr);
        free(nodelwr);
    }
    out(" };\n\n");
}

void gen_free_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_FREE_H_\n");
    out("#define _CCN_FREE_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *free_%s(Node *arg_node)", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("extern TravFunc vtable_free[_NT_SIZE];");
    out("\n");
    out("#endif /* _CCN_FREE_H_ */\n");
}

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
            freefunc = "free_node";
        } else {
            continue;
        }
        out_field("%s(%s_%s(arg_node))", freefunc, nodeupr, attrupr);
        free(attrupr);
    }
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(arg_node) = traverse(%s_%s(arg_node))", nodeupr,
                  childupr, nodeupr, childupr);
        free(childupr);
    }
    out_field("mem_free(arg_node->data.N_%s)", nodelwr);
    out_field("mem_free(arg_node)");
    out_field("return NULL");
    out_end_func();
    free(nodelwr);
    free(nodeupr);
}

void gen_free_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"core/free_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_free_func(config, fp, node);
    }
    gen_free_vtable(config, fp);
}