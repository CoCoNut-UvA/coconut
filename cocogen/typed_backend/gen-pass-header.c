#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/util.h"

#include <stdio.h>

void generate_pass_header(Config *config, FILE *fp, Pass *pass) {

    out("#pragma once\n\n");
    out("#include \"generated/ast.h\"\n");

    if (pass->func == NULL) {
        out("%s *" PASS_ENTRY_FORMAT "(%s *syntaxtree);\n", config->root_node->id,
            pass->id, config->root_node->id);

        array *values = ccn_set_values(pass->roots);
        for (int i = 0; i < array_size(values); ++i) {
            char *root = array_get(values, i);
            out("%s *" PASS_ENTRY_FORMAT "_%s(%s *syntaxtree);\n", root, pass->id, root, root);
        }
        array_cleanup(values, NULL);

    } else {
        out("%s *%s(%s *syntaxtree);\n", config->root_node->id, pass->func, config->root_node->id);

        array *values = ccn_set_values(pass->roots);
        for (int i = 0; i < array_size(values); ++i) {
            char *root = array_get(values, i);
            out("%s *%s_%s(%s *syntaxtree);\n", root, pass->func, root, root);
        }

        array_cleanup(values, NULL);

    }
}