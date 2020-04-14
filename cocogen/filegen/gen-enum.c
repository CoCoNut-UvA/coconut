#include <stdio.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-enum.h"
#include "filegen/gen-util.h"

void gen_enum(Config *config, FILE *fp, Enum *arg_enum) {
    char *arg_enumupr = strupr(arg_enum->id);
    out("// Enum %s\n", arg_enum->id);
    out("typedef enum %s {\n", arg_enumupr);
    for (int i = 0; i < array_size(arg_enum->values); i++) {
        char *id = (char *)array_get(arg_enum->values, i);
        out("    %s_%s,\n", arg_enum->prefix, id);
    }
    out("    %s_NULL\n", arg_enum->prefix);
    out("} %s;\n\n", arg_enum->id);
    free(arg_enumupr);
}

void gen_nodetype_enum(Config *config, FILE *fp) {
    out("// Enum NodeType\n");
    out("typedef enum NODETYPE {\n");
    out("    // Nodes\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out("    NT_%s,\n", nodelwr);
        free(nodelwr);
    }
    out("    // Nodesets\n");
    for (int i = 0; i < array_size(config->nodesets); ++i) {
        Nodeset *nodeset = (Nodeset *)array_get(config->nodesets, i);
        char *nodesetlwr = strlwr(nodeset->id);
        out("    NT_%s,\n", nodesetlwr);
        free(nodesetlwr);
    }
    out("    NT_NULL\n");
    out("} NodeType;\n\n");
}

void gen_traversal_enum(Config *config, FILE *fp) {
    out("typedef enum TRAVERSALS {\n");
    out("    // Traversals\n");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("    TRAV_%s,\n", travlwr);
        free(travlwr);
    }
    out("    TRAV_NULL\n");
    out("} TraversalType;\n\n");
}

void gen_enum_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_ENUM_H_\n");
    out("#define _CCN_ENUM_H_\n\n");
    gen_nodetype_enum(config, fp);
    gen_traversal_enum(config, fp);
    for (int i = 0; i < array_size(config->enums); ++i) {
        Enum *arg_enum = (Enum *)array_get(config->enums, i);
        gen_enum(config, fp, arg_enum);
    }
    out("#endif /* _CCN_ENUM_H_ */\n");
}

void gen_enum_src(Config *config, FILE *fp) {}
