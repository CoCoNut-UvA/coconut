#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_user_data_constructor(Config *config, FILE *fp, Traversal *trav) {
    out_start_func("Trav *%s_init_trav_data(Trav *trav)", trav->id->lwr);
    out_comment("Define data here");
    out_field("return trav");
    out_end_func();
}

void gen_user_data_destructor(Config *config, FILE *fp, Traversal *trav) {
    out_start_func("void %s_free_trav_data(Trav *trav)", trav->id->lwr);
    out_comment("Free attributes here");
    out_end_func();
}

void gen_trav_user_func(Config *config, FILE *fp, Traversal *trav, Node *node) {

    out_start_func("Node *%s_%s(Node *arg_node)", trav->id->lwr, node->id->lwr);
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    out_field("return arg_node");
    out_end_func();
}

void gen_trav_user_src(Config *config, FILE *fp, Traversal *trav) {
    out("#include \"../copra/include/trav_core.h\"\n");
    out("#include \"../copra/include/ast_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    if (trav->data) {
        gen_user_data_constructor(config, fp, trav);
        gen_user_data_destructor(config, fp, trav);
    }
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        gen_trav_user_func(config, fp, trav, node);
    }
}