/**
 * @file
 *
 * Traversal: dynamicGenVisitFunctions
 * UID      : DGVF
 *
 *
 */
#include <assert.h>

#include "ccn/ccn.h"
#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "frontend/symboltable.h"
#include "gen_helpers/format.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/memory.h"
#include "palm/str.h"

static char *basic_node_type = "node_st";
static node_st *ste = NULL;

static inline void print_type(node_st *attribute) {
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(attribute) == AT_link) {
        OUT_NO_INDENT("%s *", basic_node_type);
    } else if (ATTRIBUTE_TYPE(attribute) == AT_link_or_enum) {
        node_st *ref = STlookup(ste, ATTRIBUTE_TYPE_REFERENCE(attribute));
        // Should be handled by check_existence.
        assert(ref);
        assert(NODE_TYPE(ref) == NT_IENUM);
        OUT("enum %s", ID_ORIG(IENUM_NAME(ref)));
    } else if (ATTRIBUTE_TYPE(attribute) == AT_user) {
        OUT_NO_INDENT("%s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(attribute)));
    } else {
        OUT_NO_INDENT("%s", FMTattributeTypeToString(ATTRIBUTE_TYPE(attribute)));
    }
}

static inline void print_argument(node_st *arg) {
    GeneratorContext *ctx = globals.gen_ctx;
    node_st *reference = ATTRIBUTE_REFERENCE_REFERENCE(arg);
    print_type(reference);
    char *node = "this";
    if (ATTRIBUTE_REFERENCE_INODE(arg)) {
        node = ID_LWR(ATTRIBUTE_REFERENCE_INODE(arg));
    }
    OUT_NO_INDENT(" %s_%s", node, ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(arg)));
}

/**
 * @fn DGVFast
 */
node_st *DGVFast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("\n");
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGVFvisit
 */
node_st *DGVFvisit(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;

    char *visit_name =
        STRfmt("CCNvisit_%s_%d", ID_LWR(INODE_NAME(VISIT_INODE(node))),
               VISIT_INDEX(node));
    if (!VISIT_OUTPUTS(node)) { // 0 output
        OUT("void ");
    } else if (!VISIT_ARG_LIST_NEXT(VISIT_OUTPUTS(node))) { // 1 output
        OUT("");
        print_type(ATTRIBUTE_REFERENCE_REFERENCE(
            VISIT_ARG_LIST_ATTRIBUTE(VISIT_OUTPUTS(node))));
        OUT_NO_INDENT(" ");
    } else { // multiple output
        OUT_STRUCT("%s_out", visit_name);
        for (node_st *out_arg = VISIT_OUTPUTS(node); out_arg; out_arg = VISIT_ARG_LIST_NEXT(out_arg)) {
            OUT("");
            print_argument(VISIT_ARG_LIST_ATTRIBUTE(out_arg));
            OUT_NO_INDENT(";\n");
        }
        OUT_STRUCT_END();
        OUT("struct %s_out ", visit_name);
    }

    OUT_NO_INDENT("%s(%s *node", visit_name, basic_node_type);
    for (node_st *in_arg = VISIT_INPUTS(node); in_arg; in_arg = VISIT_ARG_LIST_NEXT(in_arg)) {
        OUT_NO_INDENT(", ");
        print_argument(VISIT_ARG_LIST_ATTRIBUTE(in_arg));
    }
    OUT_NO_INDENT(");\n\n");
    MEMfree(visit_name);
    TRAVnext(node);
    return node;
}
