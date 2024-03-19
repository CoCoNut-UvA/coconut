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
#include "ccngen/trav.h"
#include "dynamic_backend/collect_children.h"
#include "frontend/symboltable.h"
#include "gen_helpers/format.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/dbug.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

static const int htable_size = 100;

static char *basic_node_type = "node_st";
static node_st *ste = NULL;
static node_st *curr_node = NULL;

static inline void print_set_value(node_st *attribute, char *value) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    OUT("node->data.N_%s->_%s.ccn_val_set = true;\n",
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(attribute)));
    OUT("node->data.N_%s->_%s.%s = %s;\n", ID_LWR(INODE_NAME(curr_node)),
        ID_LWR(ATTRIBUTE_NAME(attribute)), ID_LWR(ATTRIBUTE_NAME(attribute)),
        value);
}

static inline void print_unused() {
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_NO_INDENT(" MAYBE_UNUSED");
}

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
    OUT_NO_INDENT("\n");
    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic push\n");
    OUT_NO_INDENT("// Ignore unused parameters\n");
    OUT_NO_INDENT("#pragma clang diagnostic ignored \"-Wunused-parameter\"\n");
    OUT_NO_INDENT("// Ignore uninitialized variable (already handled by assert)\n");
    OUT_NO_INDENT("#pragma clang diagnostic ignored \"-Wsometimes-uninitialized\"\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic push\n");
    OUT_NO_INDENT("// Ignore unused parameters\n");
    OUT_NO_INDENT("#pragma GCC diagnostic ignored \"-Wunused-parameter\"\n");
    OUT_NO_INDENT("#ifdef NDEBUG\n");
    OUT_NO_INDENT("// We do not initialize variables in unreachable cases, which are guarded\n"
                  "// by assertions in debug builds. We ignore this on release builds.\n");
    OUT_NO_INDENT("#pragma GCC diagnostic ignored \"-Wmaybe-uninitialized\"\n");
    OUT_NO_INDENT("#endif\n");
    OUT_NO_INDENT("#endif\n\n");

    ste = AST_STABLE(node);
    TRAVchildren(node);

    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic pop\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic pop\n");
    OUT_NO_INDENT("#endif\n\n");
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
        OUT("struct %s_out ", visit_name);
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

    for (node_st *in_arg = VISIT_INPUTS(node); in_arg;
         in_arg = VISIT_ARG_LIST_NEXT(in_arg)) {
        node_st *attribute = VISIT_ARG_LIST_ATTRIBUTE(in_arg);
        char *arg_str =
            STRfmt("%s_%s", get_node_name_this(attribute),
                   ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        print_set_value(ATTRIBUTE_REFERENCE_REFERENCE(attribute), arg_str);
        MEMfree(arg_str);
    }
    if (VISIT_INPUTS(node)) {
        OUT_NO_INDENT("\n");
    }

    TRAVsequence(node);

    if (!VISIT_OUTPUTS(node)) { // 0 output
        OUT("return;\n");
    } else if (!VISIT_ARG_LIST_NEXT(VISIT_OUTPUTS(node))) { // 1 output
        node_st *output = VISIT_ARG_LIST_ATTRIBUTE(VISIT_OUTPUTS(node));

        OUT("return %s_%s;\n", get_node_name_this(output),
            ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(output)));
    } else { // multiple output
        OUT("struct %s_out visit_output;\n", visit_name);
        for (node_st *out_arg = VISIT_OUTPUTS(node); out_arg;
             out_arg = VISIT_ARG_LIST_NEXT(out_arg)) {
            node_st *output = VISIT_ARG_LIST_ATTRIBUTE(out_arg);
            OUT("visit_output.%s_%s = %s_%s;\n", get_node_name_this(output),
                ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(output)),
                get_node_name_this(output),
                ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(output)));
        }
        OUT("return visit_output;\n");
    }

    curr_node = NULL;
    OUT_END_FUNC();

    MEMfree(visit_name);
    TRAVnext(node);
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

static inline node_st *get_child(node_st *child_name) {
    for (node_st *candidate = INODE_ICHILDREN(curr_node); candidate;
         candidate = CHILD_NEXT(candidate)) {
        if (STReq(ID_LWR(CHILD_NAME(candidate)), ID_LWR(child_name))) {
            return candidate;
        }
    }

    return NULL;
}

static inline void print_equation_args(node_st *equation,
                                       htable_st *children_null) {
    GeneratorContext *ctx = globals.gen_ctx;
    bool first = true;
    for (node_st *arg = EQUATION_IARGS(equation); arg;
         arg = EQUATION_DEPENDENCY_NEXT(arg)) {
        node_st *attribute = EQUATION_DEPENDENCY_IATTRIBUTE(arg);
        bool is_child = ATTRIBUTE_REFERENCE_INODE(attribute) != NULL;
        if (is_child) {
            node_st *child = get_child(ATTRIBUTE_REFERENCE_INODE(attribute));
            assert(child != NULL);
            if ((enum child_htable)HTlookup(children_null, child) == CH_FALSE) {
                continue;
            }
        }

        if (first) {
            first = false;
        } else {
            OUT_NO_INDENT(", ");
        }

        // // Classic attribute dependency
        // if (!ATTRIBUTE_IS_INHERITED(reference) &&
        //     !ATTRIBUTE_IS_SYNTHESIZED(reference)) {
            node_st *node_name = INODE_NAME(curr_node);
            assert(node_name != NULL);
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
        // } else {
        //     OUT_NO_INDENT("%s_%s", get_node_name_this(attribute),
        //                   ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        // }
    }
}

/**
 * @fn DGVSvisit_sequence_eval
 */
node_st *DGVSvisit_sequence_eval(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    assert(NODE_TYPE(curr_node) == NT_INODE);

    htable_st *children_null = HTnew_Ptr(htable_size);
    node_st *attribute = VISIT_SEQUENCE_EVAL_ATTRIBUTE(node);
    node_st *equation = get_equation(curr_node, attribute);

    if (equation == NULL) {
        CTI(CTI_ERROR, true, "Could not find eval function in %s for %s.%s",
            ID_ORIG(INODE_NAME(curr_node)), get_node_name_this(attribute),
            ID_ORIG(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        CCNerrorAction();
        HTdelete(children_null);
        TRAVnext(node);
        return node;
    }

    struct child_list *children =
        collect_children_equation_args(children_null, curr_node, equation);

    if (HTelementCount(children_null) == 0) {
        OUT("");
        print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
        OUT_NO_INDENT(" %s_%s", get_node_name_this(attribute),
                      ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        print_unused();
        OUT_NO_INDENT(" = EVAL%s_%s_%s(", ID_LWR(INODE_NAME(curr_node)),
                      get_node_name_this(attribute),
                      ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        print_equation_args(equation, children_null);
        OUT_NO_INDENT(");\n\n");
    } else {
        // Declare output
        OUT("");
        print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
        OUT_NO_INDENT(" %s_%s", get_node_name_this(attribute),
                      ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        print_unused();
        OUT_NO_INDENT(";\n");
        OUT("");
        // 2^{elements}
        size_t combinations = ((size_t)1UL) << HTelementCount(children_null);
        for (size_t bitmask = 0; bitmask < combinations; ++bitmask) {
            // Set bitmask in htable
            size_t i = 0;
            for (htable_iter_st *iter = HTiterate(children_null); iter;
                 iter = HTiterateNext(iter)) {
                if (bitmask & (0x1UL << i)) {
                    HTiterSetValue(iter, (void *)CH_TRUE);
                } else {
                    HTiterSetValue(iter, (void *)CH_FALSE);
                }
                ++i;
            }

            if (bitmask != combinations - 1) {
                OUT_NO_INDENT("if (");
                bool first = true;
                for (htable_iter_st *iter = HTiterate(children_null); iter;
                     iter = HTiterateNext(iter)) {
                    node_st *child = HTiterKey(iter);
                    if ((enum child_htable)HTiterValue(iter) == CH_FALSE) {
                        if (first) {
                            first = false;
                        } else {
                            OUT_NO_INDENT(" && ");
                        }
                        OUT_NO_INDENT("%s_%s(node) == NULL",
                                      ID_UPR(INODE_NAME(curr_node)),
                                      ID_UPR(CHILD_NAME(child)));
                    }
                }
                OUT_NO_INDENT(") {\n");
            } else {
                OUT_NO_INDENT("{\n");
            }

            GNindentIncrease(ctx);

            OUT("%s_%s = ", get_node_name_this(attribute),
                ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));

            OUT_NO_INDENT("EVAL%s_%s_%s", ID_LWR(INODE_NAME(curr_node)),
                          get_node_name_this(attribute),
                          ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
            for (struct child_list *entry = children; entry;
                 entry = entry->next) {
                if ((enum child_htable)HTlookup(children_null, entry->child) ==
                    CH_FALSE) {
                    OUT_NO_INDENT("_%s_NULL", ID_LWR(CHILD_NAME(entry->child)));
                }
            }
            OUT_NO_INDENT("(");
            print_equation_args(equation, children_null);
            OUT_NO_INDENT(");\n");
            GNindentDecrease(ctx);
            if (bitmask != combinations - 1) {
                OUT("} else ");
            } else {
                OUT("}\n\n");
            }
        }
    }

    if (ATTRIBUTE_REFERENCE_INODE(attribute) ==
        NULL) { // attribute of current node
        assert(
            ATTRIBUTE_IS_SYNTHESIZED(ATTRIBUTE_REFERENCE_REFERENCE(attribute)));
        char *arg_str =
            STRfmt("%s_%s", get_node_name_this(attribute),
                   ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));

        print_set_value(ATTRIBUTE_REFERENCE_REFERENCE(attribute), arg_str);
        MEMfree(arg_str);
    }

    HTdelete(children_null);
    destroy_child_list(children);

    TRAVnext(node);
    return node;
}

static inline bool is_desired(node_st *output, node_st *outputs) {
    assert(NODE_TYPE(output) == NT_VISIT_ARG_LIST);
    assert(NODE_TYPE(outputs) == NT_VISIT_ARG_LIST);
    for (node_st *candidate = outputs; candidate;
         candidate = VISIT_ARG_LIST_NEXT(candidate)) {
        node_st *attr = VISIT_ARG_LIST_ATTRIBUTE(output);
        node_st *cand_attr = VISIT_ARG_LIST_ATTRIBUTE(candidate);
        if (STReq(ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attr)),
                  ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(cand_attr)))) {
            return true;
        }
    }

    return false;
}

static void print_visit_call(node_st *node, node_st *outputs,
                             bool output_declaration, bool is_nodeset) {
    GeneratorContext *ctx = globals.gen_ctx;
    bool multiple_outputs = false;

    node_st *visit = VISIT_SEQUENCE_VISIT_VISIT(node);
    char *visit_name =
        STRfmt("CCNvisit_%s_%d", ID_LWR(INODE_NAME(VISIT_INODE(visit))),
               VISIT_INDEX(visit));

    node_st *child = VISIT_SEQUENCE_VISIT_CHILD(node);
    char *child_access = STRfmt("%s_%s(node)", ID_UPR(INODE_NAME(curr_node)),
                                ID_UPR(CHILD_NAME(child)));

    if (!is_nodeset) {
        OUT("DBUG_ASSERT(NODE_TYPE(%s) == NT_%s, \"Problem in visit\");\n",
            child_access, ID_UPR(INODE_NAME(VISIT_INODE(visit))));
    }

    if (VISIT_OUTPUTS(visit)) {
        if (!VISIT_ARG_LIST_NEXT(VISIT_OUTPUTS(visit))) { // 1 output
            node_st *attribute = VISIT_ARG_LIST_ATTRIBUTE(VISIT_OUTPUTS(visit));
            OUT("");
            if (output_declaration) {
                print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
                OUT_NO_INDENT(" ");
            }

            // If the output is not desired, we discard the output
            if (output_declaration || is_desired(VISIT_OUTPUTS(visit),
                                                 outputs)) {
                OUT_NO_INDENT("%s_%s",
                    ID_LWR(CHILD_NAME(VISIT_SEQUENCE_VISIT_CHILD(node))),
                    ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
                if (output_declaration) {
                    print_unused();
                }
                OUT_NO_INDENT(" = ");
            }
        } else { // multiple output
            multiple_outputs = true;
            OUT("struct %s_out %s_output", visit_name, visit_name);
            print_unused();
            OUT_NO_INDENT(" = ");
        }
    } else {
        OUT("");
    }

    OUT_NO_INDENT("%s(%s", visit_name, child_access);

    for (node_st *in_arg = VISIT_INPUTS(visit); in_arg;
         in_arg = VISIT_ARG_LIST_NEXT(in_arg)) {
        node_st *attribute = VISIT_ARG_LIST_ATTRIBUTE(in_arg);
        OUT_NO_INDENT(", %s_%s",
                      ID_LWR(CHILD_NAME(VISIT_SEQUENCE_VISIT_CHILD(node))),
                      ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
    }

    OUT_NO_INDENT(");\n");

    if (multiple_outputs) {
        for (node_st *out_arg = VISIT_OUTPUTS(visit); out_arg;
             out_arg = VISIT_ARG_LIST_NEXT(out_arg)) {
            if (!is_desired(out_arg, outputs)) {
                continue;
            }
            node_st *attribute = VISIT_ARG_LIST_ATTRIBUTE(out_arg);
            OUT("");
            if (output_declaration) {
                print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
                OUT_NO_INDENT(" ");
            }
            OUT_NO_INDENT("%s_%s = %s_output.%s_%s;\n",
                          ID_LWR(CHILD_NAME(VISIT_SEQUENCE_VISIT_CHILD(node))),
                          ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)),
                          visit_name, get_node_name_this(attribute),
                          ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
        }
    }

    MEMfree(visit_name);
    MEMfree(child_access);
}

static inline node_st *get_alt(node_st *visit) {
    switch (NODE_TYPE(visit)) {
    case NT_VISIT_SEQUENCE_VISIT:
        return VISIT_SEQUENCE_VISIT_ALT(visit);
    case NT_VISIT_SEQUENCE_DUMMY:
        return VISIT_SEQUENCE_DUMMY_ALT(visit);
    default:
        assert(false);
        return NULL;
    }
}

/**
 * @fn DGVSvisit_sequence_visit
 */
node_st *DGVSvisit_sequence_visit(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    assert(NODE_TYPE(curr_node) == NT_INODE);

    node_st *visit = VISIT_SEQUENCE_VISIT_VISIT(node);
    node_st *child = VISIT_SEQUENCE_VISIT_CHILD(node);
    char *child_access = NULL;

    if (VISIT_SEQUENCE_VISIT_ALT(node) || !CHILD_IS_MANDATORY(child)) {
        // Declare output arguments
        for (node_st *out_arg = VISIT_OUTPUTS(visit); out_arg;
             out_arg = VISIT_ARG_LIST_NEXT(out_arg)) {
            node_st *attribute = VISIT_ARG_LIST_ATTRIBUTE(out_arg);
            OUT("");
            print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
            OUT_NO_INDENT(" %s_%s", ID_LWR(CHILD_NAME(child)),
                          ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
            print_unused();
            OUT_NO_INDENT(";\n");
        }

        child_access = STRfmt("%s_%s(node)", ID_UPR(INODE_NAME(curr_node)),
                              ID_UPR(CHILD_NAME(child)));
    }

    if (!CHILD_IS_MANDATORY(child)) {
        OUT_BEGIN_IF("%s != NULL", child_access);
    }

    if (VISIT_SEQUENCE_VISIT_ALT(node)) { // Visit type is a nodeset
        /* At this point we do not know what the actual nodeset visit outputs
           were, so instead we simply declare the outputs of the first visit.
           This should at least include all the nodeset visit outputs, and
           the other variables might be left uninitialized, but should not
           be accessed later on. For every visit we check which outputs are
           'desired' by checking if it was declared.*/
        node_st *outputs = VISIT_OUTPUTS(visit);
        OUT("");
        for (node_st *alt_visit = node; alt_visit;
             alt_visit = get_alt(alt_visit)) {
            if (NODE_TYPE(alt_visit) == NT_VISIT_SEQUENCE_VISIT) {
                OUT_NO_INDENT("if (NODE_TYPE(%s) == NT_%s) {\n", child_access,
                              ID_UPR(INODE_NAME(VISIT_INODE(
                                  VISIT_SEQUENCE_VISIT_VISIT(alt_visit)))));
                GNindentIncrease(ctx);
                print_visit_call(alt_visit, outputs, false, true);
                GNindentDecrease(ctx);
            } else {
                OUT_NO_INDENT(
                    "if (NODE_TYPE(%s) == NT_%s) {\n", child_access,
                    ID_UPR(INODE_NAME(VISIT_SEQUENCE_DUMMY_INODE(alt_visit))));
                GNindentIncrease(ctx);
                OUT("// This node type does not require a visit.\n");
                GNindentDecrease(ctx);
            }
            OUT("} else ");
        }

        OUT_NO_INDENT("{\n");
        GNindentIncrease(ctx);
        OUT("DBUG_ASSERT(false, \"Problem in visit\"); // Should not be able "
            "to get here\n");
        OUT_END_IF();

    } else {
        print_visit_call(node, VISIT_OUTPUTS(visit), CHILD_IS_MANDATORY(child),
                         false);
        OUT_NO_INDENT("\n");
    }

    if (!CHILD_IS_MANDATORY(child)) {
        OUT_END_IF();
    }

    MEMfree(child_access);

    TRAVnext(node);
    return node;
}
