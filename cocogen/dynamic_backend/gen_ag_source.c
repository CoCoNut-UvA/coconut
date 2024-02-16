/**
 * @file
 *
 * Traversal: dynamicGenAGSource
 * UID      : DGAGS
 *
 *
 */
#include <assert.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "frontend/symboltable.h"
#include "gen_helpers/format.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/str.h"
#include "palm/memory.h"

static char *basic_node_type = "node_st";
enum trav_state {gen_type_names, create_function, call_function};
static enum trav_state curr_state;
static node_st *ste = NULL;
static node_st *curr_node = NULL;

static inline void print_type(node_st *attribute) {
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(attribute) == AT_link) {
        OUT_NO_INDENT("%s *", basic_node_type);
    } else if (ATTRIBUTE_TYPE(attribute) == AT_link_or_enum) {
        node_st *ref = STlookup(ste, ATTRIBUTE_TYPE_REFERENCE(attribute));
        // Should be handled by check_existence.
        assert(ref);
        assert(NODE_TYPE(ref) == NT_IENUM);
        OUT_NO_INDENT("enum %s", ID_ORIG(IENUM_NAME(ref)));
    } else if (ATTRIBUTE_TYPE(attribute) == AT_user) {
        OUT_NO_INDENT("%s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(attribute)));
    } else {
        OUT_NO_INDENT("%s",
                      FMTattributeTypeToString(ATTRIBUTE_TYPE(attribute)));
    }
}

static inline char *get_node_name_this(node_st *attribute) {
    char *node_name = "this";
    if (ATTRIBUTE_REFERENCE_INODE(attribute)) {
        node_name = ID_LWR(ATTRIBUTE_REFERENCE_INODE(attribute));
    }

    return node_name;
}

/**
 * @fn DGAGSast
 */
node_st *DGAGSast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    ste = AST_STABLE(node);

    OUT_START_FUNC("static inline char *nodetypeToName(node_st *node)");
    OUT_BEGIN_SWITCH("NODE_TYPE(node)");
    curr_state = gen_type_names;
    TRAVinodes(node);
    OUT_BEGIN_DEFAULT_CASE();
    OUT("return \"Unknown\";\n");
    OUT_END_CASE_NO_BREAK();
    OUT_END_SWITCH();
    OUT_END_FUNC();

    curr_state = create_function;
    TRAVinodes(node);

    OUT_START_FUNC("void CCNevaluateAttributes(%s *node)", basic_node_type);
    OUT_BEGIN_SWITCH("NODE_TYPE(node)");
    curr_state = call_function;
    TRAVinodes(node);
    OUT_BEGIN_DEFAULT_CASE();
    OUT_END_CASE();
    OUT_END_SWITCH();
    OUT("CTI(CTI_ERROR, true, \"CCNevaluateAttributes called with node of invalid node type '%%s'.\", nodetypeToName(node));\n");
    OUT("CTIabortCompilation();\n");
    OUT_END_FUNC();
    return node;
}

/**
 * @fn DGAGSinode
 */
node_st *DGAGSinode(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    if (curr_state == gen_type_names) {
        OUT_BEGIN_CASE("NT_%s", ID_UPR(INODE_NAME(node)));
        OUT_STATEMENT("return \"%s\"", ID_ORIG(INODE_NAME(node)));
        OUT_END_CASE_NO_BREAK();
        TRAVnext(node);
        return node;
    }

    bool has_inherited_attributes = false;
    for (node_st *visit = INODE_VISIT(node); visit; visit = VISIT_NEXT(visit)) {
        if (VISIT_INPUTS(visit)) {
            has_inherited_attributes = true;
        }
    }

    if (INODE_VISIT(node) && !has_inherited_attributes) {
        if (curr_state == create_function) {
            OUT_START_FUNC(
                "static inline void CCNevaluateAttributes_%s(%s *node)",
                ID_LWR(INODE_NAME(node)), basic_node_type);
            curr_node = node;
            TRAVvisit(node);
            curr_node = NULL;
            OUT_END_FUNC();
        } else {
            OUT_BEGIN_CASE("NT_%s", ID_UPR(INODE_NAME(node)));
            OUT("CCNevaluateAttributes_%s(node);\n", ID_LWR(INODE_NAME(node)));
            OUT("return;\n");
            OUT_END_CASE_NO_BREAK();
        }
    }

    TRAVnext(node);
    return node;
}

/**
 * @fn DGAGSvisit
 */
node_st *DGAGSvisit(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    assert(!VISIT_INPUTS(node));

    char *visit_name =
        STRfmt("CCNvisit_%s_%d", ID_LWR(INODE_NAME(VISIT_INODE(node))),
               VISIT_INDEX(node));
    OUT("%s(node);\n", visit_name);
    MEMfree(visit_name);

    TRAVnext(node);
    return node;
}
