#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-ast.h"
#include "filegen/gen-trav-user.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

static int indent = 0;

// target_sources(${PROJECT_NAME}
//     PUBLIC
//         driver.h
//         formatting.h
//         genmacros.h
//         util.h
//         gen-ast.h
//         gen-enum.h
//         gen-util.h
//         gen-trav.h
//         gen-trav-user.h
//         gen-free.h
//         gen-copy.h
//     PRIVATE
//         util.c
//         driver.c
//         gen-ast.c
//         gen-enum.c
//         gen-util.c
//         gen-trav-user.c
//         gen-trav.c
//         gen-free.c
//         gen-copy.c
// )
void gen_trav_user_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PUBLIC\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("        trav_%s.h\n", travlwr);
        free(travlwr);
    }
    out("    PRIVATE\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("        trav_%s.c\n", travlwr);
        free(travlwr);
    }
    out(")\n");
}

void gen_trav_user_header(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    char *travupr = strupr(trav->id);
    out("#ifndef _CCN_TRAV_%s_H_\n", travupr);
    out("#define _CCN_TRAV_%s_H_\n\n", travupr);
    out("#include \"core/ast_core.h\"\n");
    out("\n");
    out_comment("Traversal %s", trav->id);
    out_field("TraversalData *%s_init_data()", travlwr);
    out_field("void %s_free_data(TraversalData *data)", travlwr);
    out_field("Node *%s_start(Node *syntaxtree)", travlwr);
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

void gen_trav_user_src(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    char *travupr = strupr(trav->id);
    out("#include \"core/trav_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    out_typedef_struct("TRAV_DATA");
    out_comment("Define your data here");
    out_typedef_struct_end("TraversalData");
    out_start_func("TraversalData *%s_init_data()", travlwr);
    out_field("TraversalData *data = mem_alloc(sizeof(TraversalData))");
    out_comment("Initialise your data here");
    out_field("return data");
    out_end_func();
    out_start_func("void %s_free_data(TraversalData *data)", travlwr);
    out_comment("Free your data here");
    out_field("mem_free(data)");
    out_end_func();
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        char *nodelwr = strlwr(node->id);
        char *nodeupr = strupr(node->id);

        // Traversal Function
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
    }
    free(travlwr);
    free(travupr);
}