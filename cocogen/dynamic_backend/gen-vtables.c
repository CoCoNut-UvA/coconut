#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/to-string.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/genmacros.h"
#include "filegen/reachability.h"
#include "filegen/util.h"
#include "gen-functions.h"
#include "lib/print.h"
#include "lib/smap.h"

static int indent = 0;

void gen_system_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = strlwr(version);
    out("TravFunc vtable_%s[_NT_SIZE] = { &trav_error, ", verlwr);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        out("&%s_%s, ", verlwr, nodelwr);
        free(nodelwr);
    }
    out(" };\n\n");
    free(verlwr);
}

void gen_error_vtable(Config *config, FILE *fp) {
    out("const TravFunc vtable_error[_NT_SIZE] = { &trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        out("&trav_error, ");
    }
    out("};\n");
}

void gen_trav_vtable(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    out("const TravFunc vtable_%s[_NT_SIZE] = { &trav_error, ", travlwr);
    int tindex = get_trav_index(trav->id);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        if (is_traversal_node(trav, node)) {
            out("&%s_%s, ", travlwr, nodelwr);
        } else if (is_pass_node(trav, node)) {
            out("&trav_pass, ");
        } else {
            out("&trav_noop, ");
        }
        free(nodelwr);
    }
    out(" };\n\n");
    free(travlwr);
}

void gen_vtables(Config *config, FILE *fp) {
    gen_error_vtable(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_vtable(config, fp, trav);
    }
    gen_system_vtable(config, fp, "free");
    gen_system_vtable(config, fp, "copy");
    out("const TravFunc *trav_mat[_TRAV_SIZE] = { vtable_error, ");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("vtable_%s, ", travlwr);
        free(travlwr);
    }
    out("vtable_free, ");
    out("vtable_copy");
    out("};\n\n");
}

void gen_travdata_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = strlwr(version);
    out("const %sFunc trav_data_%s_array[_TRAV_SIZE] = {", version, verlwr);
    out("&trav_%s_error, ", verlwr);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        if (trav->data) {
            out("&trav_%s_%s, ", verlwr, travlwr);
        } else {
            out("&trav_%s, ", verlwr);
        }
        free(travlwr);
    }
    out("&trav_%s, ", verlwr); // Free
    out("&trav_%s, ", verlwr); // Copy
    out("};\n\n");
    free(verlwr);
}

void gen_vtables_src(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"inc_core/copy_core.h\"\n");
    out("#include \"inc_core/free_core.h\"\n");
    out("#include \"inc_core/trav_core.h\"\n");
    out("#include \"inc_core/vtables_core.h\"");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("#include \"inc_generated/trav_%s.h\"\n", travlwr);
        free(travlwr);
    }
    out("\n");
    gen_vtables(config, fp);
    gen_travdata_vtable(config, fp, "Init");
    gen_travdata_vtable(config, fp, "Free");
}
