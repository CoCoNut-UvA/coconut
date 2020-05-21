#include "ast/ast.h"
#include "ast/to-string.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void generate_user_trav_header(Config *config, FILE *fp, Traversal *trav) {

    out("#pragma once\n\n");
    out("#include \"ast.h\"\n");

    if (trav->nodes == NULL) {
        out("#include \"trav-ast.h\"\n");
    } else {
        for (int i = 0; i < array_size(trav->nodes); ++i) {
            Node *node = array_get(trav->nodes, i);
            out("#include \"trav-%s.h\"\n", node->id->orig);
        }
    }

    out("typedef struct Info Info;\n");
    out("Info *%s_createinfo(void);\n", trav->id->orig);
    out("void %s_freeinfo(Info *);\n", trav->id->orig);

    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *node = array_get(trav->nodes, i);
        out("void " TRAVERSAL_HANDLER_FORMAT "(%s *node, Info *info);\n",
            trav->id->orig, node->id->orig, node->id->orig);
    }
}
