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
    GeneratorContext *ctx = globals.gen_ctx;
    TRAVinodesets(node);

    // Set in gen_node_access_funcs
    OUT_NO_INDENT("#ifdef NDEBUG\n");
    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic pop\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic pop\n");
    OUT_NO_INDENT("#endif\n");
    OUT_NO_INDENT("#endif\n\n");
    return node;
}

node_st *DGNSAFEinodeset(node_st *node) {
    curr_nodeset = node;
    TRAViattributes(node);
    curr_nodeset = NULL;
    TRAVnext(node);
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

    OUT_START_FUNC("void CCNaccesserror_%s%s_%s(int line, const char *file, "
                   "const char *func)",
                   readonly, ID_LWR(INODESET_NAME(curr_nodeset)),
                   ID_LWR(ATTRIBUTE_NAME(node)));
    OUT_NO_INDENT("#ifndef NDEBUG\n");
    OUT("DBUGprintAssert(line, (char *)file, func, \"Node passed to %s_%s "
        "is not a '%s'\");\n", ID_UPR(INODESET_NAME(curr_nodeset)),
        ID_UPR(ATTRIBUTE_NAME(node)), ID_ORIG(INODESET_NAME(curr_nodeset)));
    OUT_NO_INDENT("#endif\n");
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}
