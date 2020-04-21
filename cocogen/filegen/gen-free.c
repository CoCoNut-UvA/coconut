#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-free.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

void gen_free_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_FREE_H_\n");
    out("#define _CCN_FREE_H_\n\n");
    out("#include \"../core/ast_core.h\"\n");
    out("#include \"../core/free_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out("extern void free_%s(Node *arg_node, Info *arg_info);\n", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("#endif /* _CCN_FREE_H_ */\n");
}

void gen_free_func(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_comment("%s", node->id);
    out_start_func("void free_%s(Node *arg_node, Info *arg_info)", nodelwr);
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        char *freefunc;
        if (attr->type == AT_string) {
            freefunc = "free_string";
        } else if (attr->type == AT_link) {
            freefunc = "free_node";
        } else if (attr->type == AT_link_or_enum) {
            if (type_is_link(config, attr)) {
                freefunc = "free_node";
            } else {
                freefunc = "free_string";
            }
        } else {
            continue;
        }
        out_field("%s(%s_%s(arg_node))", freefunc, nodeupr, attrupr);
        free(attrupr);
    }
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(arg_node) = FREETRAV(%s_%s(arg_node), arg_info)",
                  nodeupr, childupr, nodeupr, childupr);
        free(childupr);
    }
    out_field("mem_free(arg_node->data->N_%s)", nodelwr);
    out_field("mem_free(arg_node)");
    out_end_func();
    free(nodelwr);
    free(nodeupr);
}

void gen_free_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"free.h\"\n");
    out("#include \"../../../palm/include/lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_free_func(config, fp, node);
    }
}