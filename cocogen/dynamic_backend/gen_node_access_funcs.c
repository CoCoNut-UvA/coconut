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

node_st *DGNAFast(node_st *node) {
    GeneratorContext *ctx = globals.gen_ctx;
    ste = AST_STABLE(node);

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

node_st *DGNAFinode(node_st *node) {
    curr_node = node;
    TRAVichildren(node);
    TRAViattributes(node);
    curr_node = NULL;
    TRAVnext(node);
    return node;
}

node_st *DGNAFchild(node_st *node) {
    assert(curr_node);
    GeneratorContext *ctx = globals.gen_ctx;

    OUT("void CCNaccesserror_%s_%s(int line, const char *file, "
        "const char *func);\n",
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(CHILD_NAME(node)));

    OUT_START_FUNC("static inline %s **CCNaccess_%s_%s(int line, "
                   "const char *file, const char *func, %s *node)",
                   basic_node_type, ID_LWR(INODE_NAME(curr_node)),
                   ID_LWR(CHILD_NAME(node)), basic_node_type);

    OUT("#ifndef NDEBUG\n");
    OUT_BEGIN_IF("NODE_TYPE(node) != NT_%s", ID_UPR(INODE_NAME(curr_node)));
    OUT("CCNaccesserror_%s_%s(line, file, func);\n",
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(CHILD_NAME(node)));
    OUT_END_IF();
    OUT("#endif\n");

    OUT("return &(node->data.N_%s->%s_children.%s_children_st.%s);\n",
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(INODE_NAME(curr_node)),
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(CHILD_NAME(node)));
    OUT_END_FUNC();

    TRAVnext(node);
    return node;
}

node_st *DGNAFattribute(node_st *node) {
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

    OUT("void CCNaccesserror%s_%s_%s(int line, const char *file, "
        "const char *func",
        readonly, ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)));

    if (!is_classic) {
        OUT_NO_INDENT(", bool uninitialized");
    }

    OUT_NO_INDENT(");\n");

    OUT("static inline ");
    print_type(node);
    OUT_NO_INDENT(" ");
    if (is_classic) {
        OUT_NO_INDENT("*");
    }
    OUT_NO_INDENT("CCNaccess%s_%s_%s(int line, const char *file, "
                  "const char *func, %s *node) {\n",
                  readonly, ID_LWR(INODE_NAME(curr_node)),
                  ID_LWR(ATTRIBUTE_NAME(node)), basic_node_type);
    GNindentIncrease(ctx);

    OUT("#ifndef NDEBUG\n");
    OUT_BEGIN_IF("NODE_TYPE(node) != NT_%s", ID_UPR(INODE_NAME(curr_node)));
    OUT("CCNaccesserror%s_%s_%s(line, file, func", readonly,
        ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)));
    if (!is_classic) {
        OUT_NO_INDENT(", false");
    }
    OUT_NO_INDENT(");\n");
    OUT_END_IF();
    OUT("#endif\n");

    if (!is_classic) {
        OUT("#ifndef NDEBUG\n");
        OUT_BEGIN_IF("node->data.N_%s->_%s.ccn_val_set == false",
                     ID_LWR(INODE_NAME(curr_node)),
                     ID_LWR(ATTRIBUTE_NAME(node)));
        OUT("CCNaccesserror%s_%s_%s(line, file, func, true);\n", readonly,
            ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)));
        OUT_END_IF();
        OUT("#endif\n");

        OUT("return node->data.N_%s->_%s.%s;\n", ID_LWR(INODE_NAME(curr_node)),
            ID_LWR(ATTRIBUTE_NAME(node)), ID_LWR(ATTRIBUTE_NAME(node)));
    } else {
        OUT("return &(node->data.N_%s->%s);\n", ID_LWR(INODE_NAME(curr_node)),
            ID_LWR(ATTRIBUTE_NAME(node)));
    }
    OUT_END_FUNC();

    TRAVnext(node);
    return node;
}
