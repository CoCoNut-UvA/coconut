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
    TRAVinodes(node);
    return node;
}

node_st *DGNAFEinode(node_st *node) {
    curr_node = node;
    TRAVopt(INODE_IATTRIBUTES(node));
    curr_node = NULL;
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGNAFEattribute(node_st *node) {
    assert(curr_node);
    GeneratorContext *ctx = globals.gen_ctx;

    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        OUT_START_FUNC("void CCNaccess_readonly_%s_%s_error(int line, const "
                       "char *file, const char *func)",
                       ID_LWR(INODE_NAME(curr_node)),
                       ID_LWR(ATTRIBUTE_NAME(node)));
        OUT("DBUGprintAssert(line, (char *)file, func, \"Trying to access "
            "attribute before it has been evaluated.\");\n",
            ID_ORIG(INODE_NAME(curr_node)));
        OUT_END_FUNC();
    }
    TRAVnext(node);
    return node;
}
