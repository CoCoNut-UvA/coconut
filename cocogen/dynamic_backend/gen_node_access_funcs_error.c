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
static node_st *curr_node = NULL;

node_st *DGNAFEast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    // Unset in gen_nodeset_access_funcs
    OUT_NO_INDENT("#ifdef NDEBUG\n");
    OUT_NO_INDENT("// Ignore unused parameters for debug info\n");
    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic push\n");
    OUT_NO_INDENT("#pragma clang diagnostic ignored \"-Wunused-parameter\"\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic push\n");
    OUT_NO_INDENT("// Ignore unused parameters\n");
    OUT_NO_INDENT("#pragma GCC diagnostic ignored \"-Wunused-parameter\"\n");
    OUT_NO_INDENT("#endif\n");
    OUT_NO_INDENT("#endif\n\n");

    TRAVinodes(node);
    return node;
}

node_st *DGNAFEinode(node_st *node) {
    curr_node = node;
    TRAVichildren(node);
    TRAViattributes(node);
    curr_node = NULL;
    TRAVnext(node);
    return node;
}

node_st *DGNAFEchild(node_st *node) {
    assert(curr_node);
    GeneratorContext *ctx = globals.gen_ctx;

    OUT_START_FUNC("void CCNaccesserror_%s_%s(int line, const "
                    "char *file, const char *func)",
                    ID_LWR(INODE_NAME(curr_node)),
                    ID_LWR(CHILD_NAME(node)));
    OUT_NO_INDENT("#ifndef NDEBUG\n");
    OUT("DBUGprintAssert(line, (char *)file, func, \"Node passed to %s_%s"
        " is not a '%s'\");\n", ID_UPR(INODE_NAME(curr_node)),
        ID_UPR(CHILD_NAME(node)), ID_ORIG(INODE_NAME(curr_node)));
    OUT_NO_INDENT("#endif\n");
    OUT_END_FUNC();

    TRAVnext(node);
    return node;
}

node_st *DGNAFEattribute(node_st *node) {
    assert(curr_node);
    GeneratorContext *ctx = globals.gen_ctx;

    bool is_classic =
        !ATTRIBUTE_IS_INHERITED(node) && !ATTRIBUTE_IS_SYNTHESIZED(node);
    const char *readonly;
    if (is_classic) {
        readonly = "";
    } else {
        readonly = "_readonly";
    }

    OUT("void CCNaccesserror%s_%s_%s(int line, const "
                    "char *file, const char *func",
                    readonly,
                    ID_LWR(INODE_NAME(curr_node)),
                    ID_LWR(ATTRIBUTE_NAME(node)));
    if (!is_classic) {
        OUT_NO_INDENT(", bool uninitialized");
    }
    OUT_NO_INDENT(") {\n");
    GNindentIncrease(ctx);

    OUT_NO_INDENT("#ifndef NDEBUG\n");
    if (!is_classic) {
        OUT_BEGIN_IF("uninitialized");
        OUT("DBUGprintAssert(line, (char *)file, func, \"Trying to access "
            "attribute '%s' before it has been evaluated.\");\n",
            ID_ORIG(ATTRIBUTE_NAME(node)));
        GNindentDecrease(ctx);
        OUT("} else {\n");
        GNindentIncrease(ctx);
    }

    OUT("DBUGprintAssert(line, (char *)file, func, \"Node passed to %s_%s"
        " is not a '%s'\");\n", ID_UPR(INODE_NAME(curr_node)),
        ID_UPR(ATTRIBUTE_NAME(node)), ID_ORIG(INODE_NAME(curr_node)));
    if (!is_classic) {
        OUT_END_IF();
    }
    OUT_NO_INDENT("#endif\n");

    OUT_END_FUNC();

    TRAVnext(node);
    return node;
}
