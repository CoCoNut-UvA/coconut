#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-free.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

void gen_free_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_FREE_H_\n");
    out("#define _CCN_FREE_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"core/free_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out("extern Node *free_%s(Node *arg_node, Info *arg_info);\n", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("#endif /* _CCN_FREE_H_ */\n");
}

void gen_free_src(Config *config, FILE *fp) {
    out("#include \"generated/free.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
}