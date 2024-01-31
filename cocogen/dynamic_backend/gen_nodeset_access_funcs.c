#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "dynamic_backend/gen_helpers.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
static char *basic_node_type = "node_st";
static node_st *ste = NULL;
static node_st *curr_nodeset = NULL;
static node_st *curr_attribute = NULL;

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
        OUT_NO_INDENT("%s",
                      FMTattributeTypeToString(ATTRIBUTE_TYPE(attribute)));
    }
}

node_st *DGNAFast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVopt(AST_INODESETS(node));
    return node;
}

node_st *DGNAFinodeset(node_st *node)
{
    curr_nodeset = node;
    TRAVopt(INODESET_IATTRIBUTES(node));
    curr_nodeset = NULL;
    TRAVopt(INODESET_NEXT(node));
    return node;
}

node_st *DGNAFattribute(node_st *node)
{
    assert(curr_nodeset);
    GeneratorContext *ctx = globals.gen_ctx;
    if (!ATTRIBUTE_IS_INHERITED(node) && !ATTRIBUTE_IS_SYNTHESIZED(node)) {
        OUT("static inline ");
        print_type(node);
        OUT_NO_INDENT(" *CCNaccess_%s_%s(%s *node) {\n", ID_LWR(INODESET_NAME(curr_nodeset)), ID_LWR(ATTRIBUTE_NAME(node)), basic_node_type);
        GNindentIncrease(ctx);
        OUT_BEGIN_SWITCH("NODE_TYPE(node)");
        curr_attribute = node;
        TRAVopt(INODESET_EXPR(curr_nodeset));
        curr_attribute = NULL;
        OUT_BEGIN_DEFAULT_CASE();
        OUT_END_CASE();
        OUT_END_SWITCH();
        OUT("assert(false); // Should not be able to get here\n");
        OUT("return NULL;\n");
        OUT_END_FUNC();
        }
    TRAVchildren(node);
    return node;
}

node_st *DGNAFsetliteral(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_BEGIN_CASE("NT_%s", ID_UPR(SETLITERAL_REFERENCE(node)));
    OUT("return &(%s_%s(node));\n", ID_UPR(SETLITERAL_REFERENCE(node)), ID_UPR(ATTRIBUTE_NAME(curr_attribute)));
    OUT_END_CASE_NO_BREAK();
    TRAVchildren(node);
    return node;
}
