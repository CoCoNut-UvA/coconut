/**
 * @file
 *
 * Traversal: dynamicGenVisitSource
 * UID      : DGVS
 *
 *
 */
#include <assert.h>

#include "ccn/ccn.h"
#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "frontend/symboltable.h"
#include "gen_helpers/format.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/memory.h"
#include "palm/str.h"

static char *basic_node_type = "node_st";
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
        OUT("enum %s", ID_ORIG(IENUM_NAME(ref)));
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

static inline void print_argument(node_st *arg) {
    GeneratorContext *ctx = globals.gen_ctx;
    node_st *reference = ATTRIBUTE_REFERENCE_REFERENCE(arg);
    print_type(reference);

    OUT_NO_INDENT(" %s_%s", get_node_name_this(arg),
                  ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(arg)));
}

/**
 * @fn DGVFast
 */
node_st *DGVSast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("\n");
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGVFvisit
 */
node_st *DGVSvisit(node_st *node) {
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
        OUT("struct %s_out ");
    }

    OUT_NO_INDENT("%s(%s *node", visit_name, basic_node_type);
    for (node_st *in_arg = VISIT_INPUTS(node); in_arg;
         in_arg = VISIT_ARG_LIST_NEXT(in_arg)) {
        OUT_NO_INDENT(", ");
        print_argument(VISIT_ARG_LIST_ATTRIBUTE(in_arg));
    }
    OUT_NO_INDENT(")");
    OUT_START_FUNC_FIELD();
    curr_node = VISIT_INODE(node);
    TRAVopt(VISIT_SEQUENCE(node));

    if (!VISIT_OUTPUTS(node)) { // 0 output
        OUT("return;\n");
    } else if (!VISIT_ARG_LIST_NEXT(VISIT_OUTPUTS(node))) { // 1 output
        node_st *output = VISIT_ARG_LIST_ATTRIBUTE(VISIT_OUTPUTS(node));

        OUT("return %s_%s;\n", get_node_name_this(output),
            ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(output)));
    } else { // multiple output
        OUT("struct %s_out visit_output;\n");
        OUT("return visit_output;\n");
    }

    curr_node = NULL;
    OUT_END_FUNC();

    MEMfree(visit_name);
    TRAVopt(VISIT_NEXT(node));
    return node;
}

static inline node_st *get_equation(node_st *node, node_st *attribute) {
    char *find_node_name = get_node_name_this(attribute);
    for (node_st *equation = INODE_IEQUATIONS(node); equation;
         equation = EQUATION_NEXT(equation)) {
        node_st *cand_attribute = EQUATION_RULE(equation);
        char *cand_node_name = get_node_name_this(cand_attribute);
        if (STReq(find_node_name, cand_node_name) &&
            STReq(ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)),
                  ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(cand_attribute)))) {
            return equation;
        }
    }

    return NULL;
}

static inline void print_args(node_st *equation) {
    GeneratorContext *ctx = globals.gen_ctx;
    for (node_st *arg = EQUATION_IARGS(equation); arg;
         arg = EQUATION_DEPENDENCY_NEXT(arg)) {
        node_st *attribute = EQUATION_DEPENDENCY_IATTRIBUTE(arg);
        node_st *reference = ATTRIBUTE_REFERENCE_REFERENCE(attribute);
        // Classic attribute dependency
        if (!ATTRIBUTE_IS_INHERITED(reference) &&
            !ATTRIBUTE_IS_SYNTHESIZED(reference)) {
            node_st *node_name = INODE_NAME(curr_node);
            assert(node_name != NULL);
            bool is_child = ATTRIBUTE_REFERENCE_INODE(attribute) != NULL;
            if (is_child) {
                node_name = ATTRIBUTE_REFERENCE_NODE_TYPE(attribute);
            }

            OUT_NO_INDENT("%s_%s(", ID_UPR(node_name),
                          ID_UPR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
            if (is_child) {
                OUT_NO_INDENT("%s_%s(node))", ID_UPR(INODE_NAME(curr_node)),
                              ID_UPR(ATTRIBUTE_REFERENCE_INODE(attribute)));
            } else {
                OUT_NO_INDENT("node)");
            }
        } else {
            OUT_NO_INDENT("%s_%s", get_node_name_this(attribute),
                          ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        }

        if (EQUATION_DEPENDENCY_NEXT(arg)) {
            OUT_NO_INDENT(", ");
        }
    }
}

/**
 * @fn DGVSvisit_sequence_eval
 */
node_st *DGVSvisit_sequence_eval(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    assert(NODE_TYPE(curr_node) == NT_INODE);

    node_st *attribute = VISIT_SEQUENCE_EVAL_ATTRIBUTE(node);
    OUT("");
    print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
    OUT_NO_INDENT(" %s_%s = ", get_node_name_this(attribute), ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));

    OUT_NO_INDENT("EVAL%s_%s_%s(", ID_LWR(INODE_NAME(curr_node)),
        get_node_name_this(attribute),
        ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
    node_st *equation = get_equation(curr_node, attribute);
    assert(equation != NULL);
    print_args(equation);
    OUT_NO_INDENT(");\n");

    TRAVopt(VISIT_SEQUENCE_EVAL_NEXT(node));
    return node;
}

/**
 * @fn DGVSvisit_sequence_visit
 */
node_st *DGVSvisit_sequence_visit(node_st *node) {
    TRAVopt(VISIT_SEQUENCE_VISIT_NEXT(node));
    // node_st *child = ATTRIBUTE_REFERENCE_INODE(attribute);
    // if (child == NULL) {
    //     OUT_NO_INDENT("node");
    // } else {
    //     assert(NODE_TYPE(child) == NT_ID);
    //     OUT_NO_INDENT("%s_%s(node)", ID_UPR(INODE_NAME(curr_node)),
    //                   ID_UPR(child));
    // }
    return node;
}
