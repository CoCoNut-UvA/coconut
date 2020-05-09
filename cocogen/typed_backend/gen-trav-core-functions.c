#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "filegen/formatting.h"
#include "filegen/util.h"

static void generate_stack_functions(FILE *fp, bool header) {
    if (!header) {
        out("struct TravStack {\n");
        out("    struct TravStack *prev;\n");
        out("    " TRAV_ENUM_NAME " current;\n");
        out("};\n\n");
    }

    out("void " TRAV_PREFIX "push(" TRAV_ENUM_NAME " trav)");
    if (header) {
        out(";\n");
    } else {
        out(" {\n");
        out("    struct TravStack *new = (struct "
            "TravStack*)mem_alloc(sizeof(struct TravStack));\n");
        out("    new->current = trav;\n");
        out("    new->prev = current_traversal;\n");
        out("    current_traversal = new;\n");
        out("}\n\n");
    }

    out("void " TRAV_PREFIX "pop(void)");
    if (header) {
        out(";\n");
    } else {
        out(" {\n");
        out("    if (current_traversal == NULL) {\n");
        out("        print_user_error(\"traversal-driver\", \"Cannot pop of "
            "empty traversal stack.\");\n");
        out("        return;\n");
        out("    }\n");
        out("    struct TravStack *prev = current_traversal->prev;\n");
        out("    mem_free(current_traversal);\n");
        out("    current_traversal = prev;\n");
        out("}\n\n");
    }

    out(TRAV_ENUM_NAME " trav_current(void)");
    if (header) {
        out(";\n");
    } else {
        out(" {\n");
        out("    return current_traversal->current;\n");
        out("}\n\n");
    }

    if (header) {
        out("void trav_start(void *node, TraversalType travtype, NodeType nodetype);\n");
    }
}

void generate_trav_core_header(Config *config, FILE *fp) {
    out("#pragma once\n");

    out("#include \"generated/enum.h\"\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *t = array_get(config->traversals, i);
        out("#include \"generated/traversal-%s.h\"\n", t->id);
    }

    out("// Stack of traversals, so that new traversals can be started "
        "inside other traversals. \n");
    out(NT_ENUM_NAME " node_replacement_type;\n");
    out("void *node_replacement;\n");

    generate_stack_functions(fp, true);

    for (size_t i = 0; i < array_size(config->nodes); i++) {
        Node *n = array_get(config->nodes, i);
        out("extern %s *_" TRAV_PREFIX "%s(struct %s*, struct Info*);\n", n->id, n->id, n->id);
    }
}

static void generate_info_funcs_table(Config *config, FILE *fp) {
    out("static Info *(*info_create_funcs[%ld])(void) =\n", array_size(config->traversals));
    out("{\n");
    for(size_t i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        out("%s_createinfo,\n", trav->id);
    }
    out("};\n\n");

    out("static void (*info_free_funcs[%ld])(Info *) =\n", array_size(config->traversals));
    out("{\n");
    for(size_t i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        out("%s_freeinfo,\n", trav->id);
    }
    out("};\n\n");


}

void generate_trav_start(Config *config, FILE *fp) {
    int indent = 0;
    out_start_func("void trav_start(void *node, TraversalType travtype, NodeType nodetype)");
    {
        out_statement("void *info");
        out_statement("trav_push(travtype)");
        out_statement("info = (*info_create_funcs[travtype])()");
        out_statement("(*trav_node_funcs[nodetype])(node, info)");
        out_statement("(*info_free_funcs[travtype])(info)");
        out_statement("trav_pop()");
    }
    out_end_func();

}

void generate_node_trav_table(Config *config, FILE *fp) {
    out("static void *(*trav_node_funcs[%ld])() =\n", array_size(config->nodes));
    out("{\n");
    for (size_t i = 0; i < array_size(config->nodes); i++) {
        Node *n = array_get(config->nodes, i);
        out("(void *(*)())_" TRAV_PREFIX "%s,\n", n->id);
    }
    out("};\n\n");
}

void generate_trav_core_definitions(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("#include \"generated/enum.h\"\n");
    out("#include \"generated/trav-core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"lib/print.h\"\n");
    out("// Stack of traversals, so that new traversals can be started "
        "inside other traversals. \n");
    out("static struct TravStack *current_traversal;\n");

    out("// Replacement node holder\n");
    out(NT_ENUM_NAME " node_replacement_type;\n");
    out("void *node_replacement;\n");

    generate_stack_functions(fp, false);
    generate_info_funcs_table(config, fp);
    generate_node_trav_table(config, fp);
}

