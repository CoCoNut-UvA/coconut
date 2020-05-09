#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "ast/to-string.h"
#include "lib/memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void generate_user_trav_header(Config *config, FILE *fp, Traversal *trav) {

    out("#pragma once\n\n");
    out("#include \"generated/ast.h\"\n");

    if (trav->nodes == NULL) {
        out("#include \"generated/trav-ast.h\"\n");
    } else {
        for (int i = 0; i < array_size(trav->nodes); ++i) {
            Node *node = array_get(trav->nodes, i);
            out("#include \"generated/trav-%s.h\"\n", node->id);
        }
    }

    out("typedef struct Info Info;\n");
    out("Info *%s_createinfo(void);\n", trav->id);
    out("void %s_freeinfo(Info *);\n", trav->id);

    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        out("void " TRAVERSAL_HANDLER_FORMAT "(%s *node, Info *info);\n",
            trav->id, node->id, node->id);
    }
}
