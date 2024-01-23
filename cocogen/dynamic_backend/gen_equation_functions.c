/**
 * @file
 *
 * Traversal: dynamicGenEquationFunctions
 * UID      : DGEF
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/memory.h"
#include "palm/str.h"
#include <assert.h>

static char *basic_node_type = "node_st";
static node_st *curr_node = NULL;
static node_st *ste = NULL;

static inline char *get_node_name_this(node_st *attribute) {
    char *node_name = "this";
    if (ATTRIBUTE_REFERENCE_INODE(attribute)) {
        node_name = ID_LWR(ATTRIBUTE_REFERENCE_INODE(attribute));
    }

    return node_name;
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
        OUT("enum %s", ID_ORIG(IENUM_NAME(ref)));
    } else if (ATTRIBUTE_TYPE(attribute) == AT_user) {
        OUT_NO_INDENT("%s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(attribute)));
    } else {
        OUT_NO_INDENT("%s",
                      FMTattributeTypeToString(ATTRIBUTE_TYPE(attribute)));
    }
}

/**
 * @fn DGEFast
 */
node_st *DGEFast(node_st *node) {
    ste = AST_STABLE(node);
    TRAVopt(AST_INODES(node));
    return node;
}

/**
 * @fn DGEFinode
 */
node_st *DGEFinode(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    if (INODE_IEQUATIONS(node)) {
        char *filename = STRfmt("%s.h", ID_LWR(INODE_NAME(node)));
        GNopenEquationIncludeFile(ctx, filename);
        MEMfree(filename);
        OUT("#include \"ccn/ccn_types.h\"\n");
        OUT("#include \"ccngen/enum.h\"\n");
        OUT("#ifdef CCN_USES_UNSAFE\n");
        OUT("#include \"user_types.h\"\n");
        OUT("#endif\n");
        OUT("\n");
    }
    curr_node = node;
    TRAVopt(INODE_IEQUATIONS(node));
    curr_node = NULL;
    TRAVopt(INODE_NEXT(node));
    return node;
}

static inline void print_equation_args(node_st *equation) {
    GeneratorContext *ctx = globals.gen_ctx;
    for (node_st *arg = EQUATION_IARGS(equation); arg;
         arg = EQUATION_DEPENDENCY_NEXT(arg)) {
        node_st *attribute = EQUATION_DEPENDENCY_IATTRIBUTE(arg);
        node_st *reference = ATTRIBUTE_REFERENCE_REFERENCE(attribute);

        print_type(reference);
        OUT_NO_INDENT(" %s_%s", get_node_name_this(attribute),
                      ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));

        if (EQUATION_DEPENDENCY_NEXT(arg)) {
            OUT_NO_INDENT(", ");
        }
    }
}

/**
 * @fn DGEFequation
 */
node_st *DGEFequation(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node);
    node_st *attribute = EQUATION_RULE(node);

    OUT("");
    print_type(ATTRIBUTE_REFERENCE_REFERENCE(attribute));
    OUT_NO_INDENT(" EVAL%s_%s_%s(", ID_LWR(INODE_NAME(curr_node)),
                  get_node_name_this(attribute),
                  ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(attribute)));
    print_equation_args(node);
    OUT_NO_INDENT(");\n\n");

    TRAVopt(EQUATION_NEXT(node));
    return node;
}
