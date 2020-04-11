#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lib/array.h"

#include "filegen/driver.h"
#include "filegen/gen-trav.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"

void gen_trav_table_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_TRAV_TABLE_H_\n");
    out("#define _CCN_TRAV_TABLE_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"generated/trav.h\"\n");
    out("\n");
    out("typedef trav_fun_p TravFunArray[%ld];\n", array_size(config->nodes));
    out("typedef TravFunArray TravTable[%ld];\n",
        array_size(config->traversals));
    out("typedef trav_fun_p PrePostTable[%ld];\n",
        array_size(config->traversals));
    out("\n");
    out("extern TravTable travtable;\n");
    out("extern PrePostTable pretable;\n");
    out("extern PrePostTable posttable;\n");
    out("extern const char *travnames[%ld];\n", array_size(config->traversals));
    out("\n");

    out("#endif /* _CCN_TRAV_TABLE_H_ */\n");
}

void gen_trav_table(Config *config, FILE *fp) {
    out("TravTable travtables = {\n");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("    // Traversal TRAV_%s for traversal %s\n", trav->prefix,
            trav->id);
        out("    {\n");
        if (trav->nodes != NULL) {
            for (int i = 0; i < array_size(config->nodes); ++i) {
                Node *node = array_get(config->nodes, i);
                char *nodelwr = strlwr(node->id);
                if (node_in_traversal(config, fp, trav, node)) {
                    out("        &%s_%s,\n", travlwr, nodelwr);
                } else {
                    out("        &traverse_children,\n");
                }
                free(nodelwr);
            }
        } else {
            for (int i = 0; i < array_size(config->nodes); ++i) {
                Node *node = array_get(config->nodes, i);
                char *nodelwr = strlwr(node->id);
                out("        &%s_%s,\n", travlwr, nodelwr);
                free(nodelwr);
            }
        }
        out("    },\n\n");
        free(travlwr);
    }
    out("};\n\n");
}

void gen_pre_post_tabeles(Config *config, FILE *fp) {
    out("PrePostTable pretable = {\n");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        out("    NULL,\n");
    }
    out("};\n\n");

    out("PrePostTable posttable = {\n");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        out("    NULL,\n");
    }
    out("};\n\n");
}

void gen_trav_names_table(Config *config, FILE *fp) {
    out("const char *travnames[%ld] = {\n", array_size(config->traversals));
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->prefix);
        out("    \"%s\",\n", travlwr);
        free(travlwr);
    }
    out("};\n\n");
}

void gen_trav_table_src(Config *config, FILE *fp) {
    out("#include \"generated/trav_table.h\"\n");
    out("\n");
    gen_trav_table(config, fp);
    gen_pre_post_tabeles(config, fp);
    gen_trav_names_table(config, fp);
}
