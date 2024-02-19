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
static char *basic_node_type = "node_st";
static node_st *ste = NULL;
static node_st *curr_nodeset = NULL;
static node_st *curr_attribute = NULL;
static bool is_classic = false;

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

node_st *DGNSAFast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    ste = AST_STABLE(node);
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

node_st *DGNSAFinodeset(node_st *node) {
    curr_nodeset = node;
    TRAViattributes(node);
    curr_nodeset = NULL;
    TRAVnext(node);
    return node;
}

node_st *DGNSAFattribute(node_st *node) {
    assert(curr_nodeset);
    GeneratorContext *ctx = globals.gen_ctx;
    is_classic =
        !ATTRIBUTE_IS_INHERITED(node) && !ATTRIBUTE_IS_SYNTHESIZED(node);

    OUT("void CCNaccesserror_");
    if (!is_classic) {
        OUT_NO_INDENT("readonly_");
    }
    OUT_NO_INDENT(
        "%s_%s(int line, const char *file, const char *func);\n",
        ID_LWR(INODESET_NAME(curr_nodeset)), ID_LWR(ATTRIBUTE_NAME(node)));

    OUT("static inline ");
    print_type(node);
    OUT_NO_INDENT(" ");
    if (is_classic) {
        OUT_NO_INDENT("*");
    }
    OUT_NO_INDENT("CCNaccess_");
    if (!is_classic) {
        OUT_NO_INDENT("readonly_");
    }
    OUT_NO_INDENT(
        "%s_%s(int line, const char *file, const char *func, %s *node) {\n",
        ID_LWR(INODESET_NAME(curr_nodeset)), ID_LWR(ATTRIBUTE_NAME(node)),
        basic_node_type);
    GNindentIncrease(ctx);
    OUT_BEGIN_SWITCH("NODE_TYPE(node)");
    curr_attribute = node;
    TRAVexpr(curr_nodeset);
    curr_attribute = NULL;
    OUT_BEGIN_DEFAULT_CASE();
    OUT_END_CASE();
    OUT_END_SWITCH();
    OUT("#ifndef NDEBUG\n");
    OUT("CCNaccesserror_");
    if (!is_classic) {
        OUT_NO_INDENT("readonly_");
    }
    OUT_NO_INDENT("%s_%s(line, file, func);\n",
                  ID_LWR(INODESET_NAME(curr_nodeset)),
                  ID_LWR(ATTRIBUTE_NAME(node)));
    OUT("#endif\n");
    OUT("return (");
    print_type(node);
    if (is_classic) {
        OUT_NO_INDENT("*");
    }
    OUT_NO_INDENT(") 0;\n");
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}

node_st *DGNSAFsetliteral(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_BEGIN_CASE("NT_%s", ID_UPR(SETLITERAL_REFERENCE(node)));
    OUT("return ");
    if (is_classic) {
        OUT_NO_INDENT("&(");
    }
    OUT_NO_INDENT("%s_%s(node)", ID_UPR(SETLITERAL_REFERENCE(node)),
                  ID_UPR(ATTRIBUTE_NAME(curr_attribute)));
    if (is_classic) {
        OUT_NO_INDENT(")");
    }
    OUT_NO_INDENT(";\n");
    OUT_END_CASE_NO_BREAK();
    TRAVchildren(node);
    return node;
}
