/**
 * @file
 *
 * This generates the copy traversal to be used in a compiler.
 * Copying of children is deep, copying of attributes is deep on
 * primitives types. Attribute nodes are shallow copied.
 */

#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"
#include "dynamic_backend/gen_helpers.h"

static int arg_num = 0;
static node_st *curr_node;

node_st *DGCTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ccn_copy.c");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include <stddef.h>\n");
    OUT("#include \"palm/str.h\"\n");
    OUT("#include \"palm/ctinfo.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n\n");
    // Consider switching to C23 [[maybe_unused]] in the future
    OUT("#ifdef __GNUC__\n");
    OUT("#define MAYBE_UNUSED __attribute__((unused))\n");
    OUT("#else\n");
    OUT("#define MAYBE_UNUSED\n");
    OUT("#endif\n\n");
    OUT("static const char *const user_warn MAYBE_UNUSED =\n");
    GNindentIncrease(ctx);
    OUT("\"%%s:%%d: Attributes with user types do not support deep copying, \"\n");
    OUT("\"instead the attributes are copied by value. Make sure you set \"\n");
    OUT("\"a correct value for the copied node's attribute yourself. Add \"\n");
    OUT("\"`#define CCN_USES_UNSAFE_ACKNOWLEDGE` to user_types.h to \"\n");
    OUT("\"disable this warning.\";\n");
    GNindentDecrease(ctx);
    OUT_NO_INDENT("\n");
    OUT_START_FUNC("void CopyBaseNode(node_st *target, node_st *source)");
    OUT_STATEMENT("NODE_BCOL(target) = NODE_BCOL(source)");
    OUT_STATEMENT("NODE_ECOL(target) = NODE_ECOL(source)");
    OUT_STATEMENT("NODE_BLINE(target) = NODE_BLINE(source)");
    OUT_STATEMENT("NODE_ELINE(target) = NODE_ELINE(source)");
    OUT_STATEMENT("NODE_FILENAME(target) = STRcpy(NODE_FILENAME(source))");
    OUT_END_FUNC();
    TRAVinodes(node);
    return node;
}

node_st *DGCTinode(node_st *node)
{
    const char *node_argument_name = "arg_node";
    const char *new_node_name = "new_node";
    GeneratorContext *ctx = globals.gen_ctx;
    curr_node = node;
    arg_num = 0;
    OUT_START_FUNC(DGH_TRAV_FUNC_SIG(), "CPY", DGH_TRAVERSAL_TARGET_ID(INODE_NAME(node)), node_argument_name);
    TRAVstart(node, TRAV_DGCC);
    OUT_STATEMENT("CopyBaseNode(%s, %s)", new_node_name, node_argument_name);
    TRAVichildren(node);
    TRAViattributes(node);
    OUT_FIELD("return %s", new_node_name);
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}

node_st *DGCTchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *child_name = ID_UPR(CHILD_NAME(node));
    OUT_FIELD("%s_%s(new_node) = TRAVopt(%s_%s(arg_node))", node_name, child_name, node_name, child_name);
    TRAVnext(node);
    return node;
}

node_st *DGCTattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *attr_name = ID_UPR(ATTRIBUTE_NAME(node));

    // We do not copy inherited and synthesized attributes.
    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        TRAVnext(node);
        return node;
    }

    if (ATTRIBUTE_TYPE(node) == AT_string) {
        OUT_FIELD("%s_%s(new_node) = STRcpy(%s_%s(arg_node))", node_name, attr_name, node_name, attr_name);
    } else if (ATTRIBUTE_TYPE(node) == AT_user) {
        OUT_NO_INDENT("\n");
        OUT_NO_INDENT("#ifndef CCN_USES_UNSAFE_ACKNOWLEDGE\n");
        OUT("CTI(CTI_WARN, true, user_warn, __FILE__, __LINE__);\n");
        OUT_NO_INDENT("#endif // CCN_USES_UNSAFE_ACKNOWLEDGE\n");
        OUT("%s_%s(new_node) = %s_%s(arg_node); // might not be copied\n", node_name, attr_name, node_name, attr_name);
        OUT_NO_INDENT("\n");
    } else {
        OUT_FIELD("%s_%s(new_node) = %s_%s(arg_node)", node_name, attr_name, node_name, attr_name);
    }
    TRAVnext(node);
    return node;
}
