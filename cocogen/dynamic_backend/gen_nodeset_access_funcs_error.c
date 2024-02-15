#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "dynamic_backend/gen_helpers.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
static node_st *curr_nodeset = NULL;

node_st *DGNSAFEast(node_st *node) {
    TRAVinodesets(node);
    return node;
}

node_st *DGNSAFEinodeset(node_st *node) {
    curr_nodeset = node;
    TRAVopt(INODESET_IATTRIBUTES(node));
    curr_nodeset = NULL;
    TRAVopt(INODESET_NEXT(node));
    return node;
}

node_st *DGNSAFEattribute(node_st *node) {
    assert(curr_nodeset);
    GeneratorContext *ctx = globals.gen_ctx;

    char *readonly;
    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        readonly = "readonly_";
    } else {
        readonly = "";
    }

    OUT_START_FUNC("void CCNaccess_%s%s_%s_error(int line, const char *file, "
                   "const char *func)",
                   readonly, ID_LWR(INODESET_NAME(curr_nodeset)),
                   ID_LWR(ATTRIBUTE_NAME(node)));
    OUT("DBUGprintAssert(line, (char *)file, func, \"Node is not a '%s'\");\n",
        ID_ORIG(INODESET_NAME(curr_nodeset)));
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}
