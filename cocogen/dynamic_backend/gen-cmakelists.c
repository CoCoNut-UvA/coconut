#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/gen-ast.h"
#include "filegen/gen-trav-user.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

void gen_generated_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PUBLIC\n");
    out("        actions.h\n");
    out("        ast.h\n");
    out("        copy.h\n");
    out("        enum.h\n");
    out("        free.h\n");
    out("        trav.h\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("        trav_%s.h\n", travlwr);
        free(travlwr);
    }
    out("    PRIVATE\n");
    out("        actions.c\n");
    out("        ast.c\n");
    out("        copy.c\n");
    out("        free.c\n");
    out("        trav.c\n");
    out(")\n");
}

void gen_user_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PRIVATE\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("        trav_%s.c\n", travlwr);
        free(travlwr);
    }
    out(")\n");
}