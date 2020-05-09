#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_trav_user_header(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    char *travupr = strupr(trav->id);
    out("#ifndef _CCN_TRAV_%s_H_\n", travupr);
    out("#define _CCN_TRAV_%s_H_\n\n", travupr);
    out("#include \"inc_core/ast_core.h\"\n");
    out("#include \"inc_core/trav_core.h\"\n");
    out("\n");
    out_comment("Traversal %s", trav->id);
    out_field("Trav *trav_init_%s()", travlwr);
    out_field("void trav_free_%s(Trav *trav)", travlwr);
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *%s_%s(Node *arg_node)", travlwr, nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("#endif /* _CCN_TRAV_%s_H_ */\n", travupr);
    free(travlwr);
    free(travupr);
}

void gen_trav_constructor(Config *config, FILE *fp, Traversal *trav) {
    char *travupr = strupr(trav->id);
    char *travlwr = strlwr(trav->id);
    out_start_func("Trav *trav_init_%s()", travlwr);
    out_field("Trav *trav = trav_init()");
    out_field("trav->travdata.TD_%s = mem_alloc(sizeof(struct TRAV_DATA_%s))",
              travlwr, travupr);
    out_comment("Define data here");
    out_field("return trav");
    out_end_func();
    free(travupr);
    free(travlwr);
}

void gen_trav_destructor(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    out_start_func("void trav_free_%s(Trav *trav)", travlwr);
    out_comment("Free attributes here");
    out_field("mem_free(trav->travdata.TD_%s)", travlwr);
    out_field("mem_free(trav)");
    out_end_func();
    free(travlwr);
}

void gen_trav_user_func(Config *config, FILE *fp, Traversal *trav, Node *node) {
    char *travlwr = strlwr(trav->id);
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);

    out_start_func("Node *%s_%s(Node *arg_node)", travlwr, nodelwr);
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(arg_node) = traverse(%s_%s(arg_node))", nodeupr,
                  childupr, nodeupr, childupr);
        free(childupr);
    }
    out_field("return arg_node");
    out_end_func();

    free(nodelwr);
    free(nodeupr);
    free(travlwr);
}

void gen_trav_user_src(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    char *travupr = strupr(trav->id);
    out("#include \"inc_generated/trav_%s.h\"\n", travlwr);
    out("#include \"lib/memory.h\"\n");
    out("\n");
    if (trav->data) {
        gen_trav_constructor(config, fp, trav);
        gen_trav_destructor(config, fp, trav);
    }
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        gen_trav_user_func(config, fp, trav, node);
    }
    free(travlwr);
    free(travupr);
}