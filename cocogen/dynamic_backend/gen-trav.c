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

void gen_trav_struct(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    char *travupr = strupr(trav->id);
    out_comment("Traversal %s Attributes", trav->id);
    out_struct("TRAV_DATA_%s", travupr);
    for (int i = 0; i < array_size(trav->data); ++i) {
        Attr *td = (Attr *)array_get(trav->data, i);
        char *type_str = str_attr_type(td);
        if (td->type == AT_link_or_enum) {
            out_field("%s *%s", type_str, td->id);
        } else {
            out_field("%s %s", type_str, td->id);
        }
    }
    out_struct_end();
    free(travupr);
}

void gen_trav_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("TRAV_DATA");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(config->traversals, i);
        if (!trav->data) {
            continue;
        }
        char *travupr = strupr(trav->id);
        char *travlwr = strlwr(trav->id);
        out_field("struct TRAV_DATA_%s *TD_%s", travupr, travlwr);
        free(travupr);
        free(travlwr);
    }
    out_struct_end();
}

void gen_trav_macros(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    out_comment("Macros for Traversal %s", trav->id);
    char *travupr = strupr(trav->id);
    char *travlwr = strlwr(trav->id);
    for (int i = 0; i < array_size(trav->data); ++i) {
        Attr *td = (Attr *)array_get(trav->data, i);
        char *attrupr = strupr(td->id);
        out("#define %s_%s (trav_current()->travdata.TD_%s->%s)\n", travupr,
            attrupr, travlwr, td->id);
        free(attrupr);
    }
    out("\n");
    free(travupr);
    free(travlwr);
}

void gen_trav_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"inc_core/ast_core.h\"\n");
    out("\n");

    out_comment("Includes for user defined types");
    smap_t *include_names = smap_init(16);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        for (int i = 0; i < array_size(trav->data); i++) {
            Attr *td = array_get(trav->data, i);
            if (td->type == AT_link_or_enum) {
                char *include = td->include;
                if (!smap_retrieve(include_names, include)) {
                    smap_insert(include_names, include, td);
                    out("#include \"%s\"\n", include);
                }
            }
        }
    }
    smap_free(include_names);
    out("\n");

    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_struct(config, fp, trav);
    }
    gen_trav_union(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_macros(config, fp, trav);
    }
    out("\n");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        if (!node->children) {
            continue;
        }
        char *nodelwr = strlwr(node->id);
        out_field("Node *trav_%s(Node *arg_node)", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("#endif /* _CCN_TRAV_H_ */\n");
}

void gen_error_vtable(Config *config, FILE *fp) {
    out("TravFunc vtable_error[_NT_SIZE] = { &trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        out("&trav_error, ");
    }
    out("};\n");
}

void gen_trav_vtable(Config *config, FILE *fp, Traversal *trav) {
    char *travlwr = strlwr(trav->id);
    out("TravFunc vtable_%s[_NT_SIZE] = { &trav_error, ", travlwr);
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

void gen_travdata_arrays(Config *config, FILE *fp, char *version) {
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

void gen_trav_node_func(Config *config, FILE *fp, Node *node) {
    if (!node->children) {
        return;
    }
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);

    out_start_func("Node *trav_%s(Node *arg_node)", nodelwr);
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(arg_node) = trav_pass(%s_%s(arg_node))", nodeupr,
                  childupr, nodeupr, childupr);
        free(childupr);
    }
    out_field("return arg_node");
    out_end_func();

    free(nodelwr);
    free(nodeupr);
}

void lifetimes(Config *config, FILE *fp) {
    for (int i = 0; i < array_size(config->nodes); i++) {
        Node *node = array_get(config->nodes, i);
        for (int j = 0; j < array_size(node->lifetimes); j++) {
        }
    }
}

void gen_trav_src(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"inc_core/copy_core.h\"\n");
    out("#include \"inc_core/free_core.h\"\n");
    out("#include \"inc_core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("#include \"inc_generated/trav_%s.h\"\n", travlwr);
        free(travlwr);
    }
    out("\n");
    gen_vtables(config, fp);
    gen_travdata_arrays(config, fp, "Init");
    gen_travdata_arrays(config, fp, "Free");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        gen_trav_node_func(config, fp, node);
    }
}
