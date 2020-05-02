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
}
