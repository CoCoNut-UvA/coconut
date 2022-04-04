/**
 * @file
 *
 * Traversal that generates the consitency checking traversal based
 * on node information and lifetimes.
 */

#include <assert.h>
#include <ccngen/trav.h>
#include <palm/memory.h>
#include <stdio.h>

#include "ccn/dynamic_core.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/str.h"
#include "gen_helpers.h"

#define LT_NODE 1
#define LT_CHILD 2
#define LT_ATTRIBUTE 3

static node_st *ast;
static node_st *curr_node;
static node_st *curr_child;
static node_st *curr_attribute;
static bool gen_type_names;

void DGCHTinit() { return; }
void DGCHTfini() { return; }


node_st *DGCHTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ccn_check.c");
    OUT("#include <stdbool.h>\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"ccn/phase_driver.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/ctinfo.h\"\n");
    ast = node;
    OUT_START_FUNC("char *nodetypeToName(node_st *node)");
    OUT_BEGIN_SWITCH("NODE_TYPE(node)");
    gen_type_names = true;
    TRAVinodes(node);
    OUT_BEGIN_DEFAULT_CASE();
    OUT("return \"Unknown\";\n");
    OUT_END_CASE_NO_BREAK();
    OUT_END_SWITCH();
    OUT_END_FUNC();
    gen_type_names = false;

    TRAVopt(AST_INODESETS(node));
    TRAVopt(AST_INODES(node));
    return node;
}

node_st *DGCHTiactions(node_st *node)
{
    return node;
}

node_st *DGCHTiphase(node_st *node)
{
    return node;
}

node_st *DGCHTitraversal(node_st *node)
{
    return node;
}


node_st *DGCHTitravdata(node_st *node)
{
    return node;
}

node_st *DGCHTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGCHTinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (gen_type_names) {
        OUT_BEGIN_CASE("NT_%s", ID_UPR(INODE_NAME(node)));
        OUT_STATEMENT("return \"%s\"", ID_ORIG(INODE_NAME(node)));
        OUT_END_CASE();
        TRAVnext(node);
        return node;
    }
    struct data_dgcht *data = DATA_DGCHT_GET();
    data->lifetime_target = LT_NODE;
    curr_node = node;
    OUT_START_FUNC("struct ccn_node *CHK%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    OUT_FIELD("size_t action_id = CCNgetCurrentActionId()");
    OUT_FIELD("(void)action_id"); // Prevents unused error.
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    // Child & attributes change the target, so change it back.
    data->lifetime_target = LT_NODE;
    TRAVopt(INODE_LIFETIMES(node));
    if (INODE_ICHILDREN(node)) {
        OUT_FIELD("TRAVchildren(arg_node)");
    }
    OUT_FIELD("return arg_node");
    OUT_END_FUNC();
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGCHTinodeset(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_START_FUNC("static bool TypeIs%s(node_st *arg_node)", ID_LWR(INODESET_NAME(node)));
    OUT_FIELD("enum ccn_nodetype node_type = NODE_TYPE(arg_node)");
    OUT("return (false");
    TRAVopt(INODESET_EXPR(node));
    OUT(");\n");
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}

node_st *DGCHTchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    struct data_dgcht *data = DATA_DGCHT_GET();
    data->lifetime_target = LT_CHILD;
    curr_child = node;
    OUT_BEGIN_IF("%s_%s(arg_node)", ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
    {
        if (CHILD_TYPE(node) == CT_inode) {
            OUT_BEGIN_IF("NODE_TYPE(%s_%s(arg_node)) != NT_%s", ID_UPR(INODE_NAME(curr_node)),
                         ID_UPR(CHILD_NAME(node)), ID_UPR(CHILD_TYPE_REFERENCE(node)));
        } else {
            OUT_BEGIN_IF("!TypeIs%s(%s_%s(arg_node))", ID_LWR(CHILD_TYPE_REFERENCE(node)),
                         ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
        }
        OUT_FIELD("CTI(CTI_ERROR, true, \"Inconsistent node found in AST. Child(%s) of node(%s) has disallowed type(%%s) \", nodetypeToName(%s_%s(arg_node)))",
            ID_ORIG(CHILD_NAME(node)), ID_ORIG(INODE_NAME(curr_node)), ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
        OUT_END_IF();
    }
    OUT_END_IF();
    TRAVopt(CHILD_LIFETIMES(node));
    TRAVnext(node);

    return node;
}

node_st *DGCHTattribute(node_st *node)
{
    struct data_dgcht *data = DATA_DGCHT_GET();
    data->lifetime_target = LT_ATTRIBUTE;
    curr_attribute = node;
    TRAVlifetimes(node);
    TRAVnext(node);
    return node;
}

node_st *DGCHTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCHTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCHTsetliteral(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (!node) {
        return node;
    }
    if (node && SETLITERAL_REFERENCE(node)) {
        OUT_NO_INDENT(" || node_type == NT_%s", ID_UPR(SETLITERAL_REFERENCE(node)));
    }
    TRAVopt(SETLITERAL_LEFT(node));
    TRAVopt(SETLITERAL_RIGHT(node));
    return node;
}

node_st *DGCHTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCHTienum(node_st *node)
{
    return node;
}

node_st *DGCHTid(node_st *node)
{
    return node;
}

// NOTE: Do not go deeper, needs a refactor. Is written with little sleep.

static void LifetimeNodeDisallowed(node_st *lifetime_begin, node_st *lifetime_end, char *error)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_begin)));
        } else {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin)));
        }
    }
    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end)));
        } else {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_end)));
        }
    }
    OUT_NO_INDENT("true) {\n");
    GNindentIncrease(ctx);
    OUT_FIELD("CTI(CTI_ERROR, true, \"%s\\n\")",
              error);
    OUT_END_IF();
}

static void LifetimeNodeAllowed(node_st *lifetime_begin, node_st *lifetime_end, char *error)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_begin));
        } else {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin));
        }
    }

    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end));
        } else {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_end));
        }
    }
    OUT_NO_INDENT("false) {\n");
    GNindentIncrease(ctx);
    OUT_FIELD("CTI(CTI_ERROR, true, \"Found disallowed %s in tree.\\n\")",
              error);
    OUT_END_IF();
}

/* Generation of lifetimes that are defined for a node.
 */
static void LifetimeNodeGen(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    assert(NODE_TYPE(node) == NT_ILIFETIME);
    // Nodes can only specify disallowed lifetimes.
    assert(ILIFETIME_TYPE(node) == LT_disallowed || ILIFETIME_TYPE(node) == LT_allowed);
    if (!ILIFETIME_BEGIN(node) && !ILIFETIME_END(node)) {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            OUT_FIELD("CTI(CTI_ERROR, true, \"Found disallowed node(%s) in tree.\\n\");", ID_ORIG(INODE_NAME(curr_node)));
        }
    } else {
        char *error = STRcatn(3, "Found disallowed node(", ID_ORIG(INODE_NAME(curr_node)), ") in tree.");
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            LifetimeNodeDisallowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node), error);
        } else {
            LifetimeNodeAllowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node), error);
        }
        MEMfree(error);
    }
}

static void LifetimeChildDisallowed(node_st *lifetime_begin, node_st *lifetime_end)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_begin)));
        } else {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin)));
        }
    }
    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end)));
        } else {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_end)));
        }
    }
    OUT_NO_INDENT("true) {\n");
    GNindentIncrease(ctx);

    {
        OUT_BEGIN_IF("%s_%s(arg_node) != NULL", ID_UPR(INODE_NAME(curr_node)),
                     ID_UPR(CHILD_NAME(curr_child)));
        OUT_FIELD("CTI(CTI_ERROR, true, \"Found disallowed child(%s) in node(%s).\\n\");",
                  ID_ORIG(CHILD_NAME(curr_child)),
                  ID_ORIG(INODE_NAME(curr_node)));
        OUT_END_IF();
    }

    OUT_END_IF();
}
static void LifetimeChildAllowed(node_st *lifetime_begin, node_st *lifetime_end)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_begin));
        } else {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin));
        }
    }

    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end));
        } else {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_end));
        }
    }
    OUT_NO_INDENT("false) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s_%s(arg_node) != NULL",
                 ID_UPR(INODE_NAME(curr_node)),
                 ID_UPR(CHILD_NAME(curr_child)));
    OUT_FIELD(
        "CTIerror(\"Found disallowed child(%s) in node(%s).\\n\");",
        ID_ORIG(CHILD_NAME(curr_child)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

static void LifetimeChildMandatory(node_st *lifetime_begin, node_st *lifetime_end)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_begin)));
        } else {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin)));
        }
    }
    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end)));
        } else {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_end)));
        }
    }
    OUT_NO_INDENT("true) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s_%s(arg_node) == NULL",
                 ID_UPR(INODE_NAME(curr_node)),
                 ID_UPR(CHILD_NAME(curr_child)));
    OUT_FIELD(
        "CTI(CTI_ERROR, true, \"Mandatory child(%s) in node(%s) is missing.\\n\");",
        ID_ORIG(CHILD_NAME(curr_child)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

static void LifetimeChildOptional(node_st *lifetime_begin, node_st *lifetime_end)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_begin));
        } else {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin));
        }
    }

    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end));
        } else {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_end));
        }
    }
    OUT_NO_INDENT("false) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s_%s(arg_node) == NULL",
                 ID_UPR(INODE_NAME(curr_node)),
                 ID_UPR(CHILD_NAME(curr_child)));
    OUT_FIELD(
        "CTI(CTI_ERROR, true, \"Mandatory child(%s) in node(%s) is missing.\\n\");",
        ID_ORIG(CHILD_NAME(curr_child)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

/* The check is executed in the parent of this child. */
static void LifetimeChildGen(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    assert(NODE_TYPE(node) == NT_ILIFETIME);
    if (!ILIFETIME_BEGIN(node) && !ILIFETIME_END(node)) {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            OUT_BEGIN_IF("%s_%s(arg_node) != NULL", ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(curr_child)));
            OUT_FIELD("CTI(CTI_ERROR, true, \"Found disallowed child(%s) in node(%s).\\n\");", ID_ORIG(CHILD_NAME(curr_child)), ID_ORIG(INODE_NAME(curr_node)));
            OUT_END_IF();
        }else if (ILIFETIME_TYPE(node) == LT_mandatory) {
            OUT_BEGIN_IF("%s_%s(arg_node) == NULL", ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(curr_child)));
            OUT_FIELD("CTI(CTI_ERROR, true, \"Child(%s) in node(%s) is missing, but specified as mandatory.\\n\");", ID_ORIG(CHILD_NAME(curr_child)), ID_ORIG(INODE_NAME(curr_node)));
            OUT_END_IF();
        }
    } else {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            LifetimeChildDisallowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node));
        } else if (ILIFETIME_TYPE(node) == LT_allowed) {
            LifetimeChildAllowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node));
        } else if (ILIFETIME_TYPE(node) == LT_mandatory) {
            LifetimeChildMandatory(ILIFETIME_BEGIN(node), ILIFETIME_END(node));
        } else if (ILIFETIME_TYPE(node) == LT_optional) {
            LifetimeChildOptional(ILIFETIME_BEGIN(node), ILIFETIME_END(node));
        }
    }
}

static void LifetimeAttributeDisallowed(node_st *lifetime_begin, node_st *lifetime_end, char *access)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_begin)));
        } else {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin)));
        }
    }
    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end)));
        } else {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_end)));
        }
    }
    OUT_NO_INDENT("true) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s != NULL", access);
    OUT_FIELD(
        "CTI(CTI_ERROR, true, \"Found disallowed attribute(%s) in node(%s).\\n\");",
        ID_ORIG(ATTRIBUTE_NAME(curr_attribute)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}
static void LifetimeAttributeAllowed(node_st *lifetime_begin, node_st *lifetime_end, char *access)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_begin));
        } else {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin));
        }
    }

    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end));
        } else {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_end));
        }
    }
    OUT_NO_INDENT("false) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s != NULL", access);
    OUT_FIELD(
        "CTIerror(\"Found disallowed attribute(%s) in node(%s).\\n\");",
        ID_ORIG(ATTRIBUTE_NAME(curr_attribute)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

static void LifetimeAttributeMandatory(node_st *lifetime_begin, node_st *lifetime_end, char *access)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_begin)));
        } else {
            OUT_NO_INDENT(
                "action_id >= %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin)));
        }
    }
    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end)));
        } else {
            OUT_NO_INDENT(
                "action_id < %d && ",
                (LIFETIME_RANGE_ACTION_ID(lifetime_end)));
        }
    }
    OUT_NO_INDENT("true) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s == NULL", access);
    OUT_FIELD(
        "CTI(CTI_ERROR, true, \"Mandatory attribute(%s) in node(%s) is missing.\\n\");",
        ID_ORIG(ATTRIBUTE_NAME(curr_attribute)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

static void LifetimeAttributeOptional(node_st *lifetime_begin, node_st *lifetime_end, char *access)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("if (");
    if (lifetime_begin) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_begin)) {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_begin));
        } else {
            OUT_NO_INDENT("action_id < %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_begin));
        }
    }

    if (lifetime_end) {
        if (LIFETIME_RANGE_INCLUSIVE(lifetime_end)) {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_NEXT_ACTION_ID(lifetime_end));
        } else {
            OUT_NO_INDENT("action_id >= %d || ", LIFETIME_RANGE_ACTION_ID(lifetime_end));
        }
    }
    OUT_NO_INDENT("false) {\n");
    GNindentIncrease(ctx);

    OUT_BEGIN_IF("%s == NULL", access);
    OUT_FIELD(
        "CTI(CTI_ERROR, true, \"Mandatory attribute(%s) in node(%s) is missing.\\n\");",
        ID_ORIG(ATTRIBUTE_NAME(curr_attribute)),
        ID_ORIG(INODE_NAME(curr_node)));
    OUT_END_IF();

    OUT_END_IF();
}

static void LifetimeAttributeGen(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    assert(NODE_TYPE(node) == NT_ILIFETIME);
    assert(curr_attribute != NULL);
    char *access = DGHattributeAccess(curr_node, curr_attribute, "arg_node");
    if (!ILIFETIME_BEGIN(node) && !ILIFETIME_END(node)) {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            OUT_BEGIN_IF("%s != NULL", access);
            OUT_FIELD("CTI(CTI_ERROR, true, \"Found disallowed attribute(%s) in node(%s).\\n\");", ID_ORIG(ATTRIBUTE_NAME(curr_attribute)), ID_ORIG(INODE_NAME(curr_node)));
            OUT_END_IF();
        }else if (ILIFETIME_TYPE(node) == LT_mandatory) {
            OUT_BEGIN_IF("%s == NULL", access);
            OUT_FIELD("CTI(CTI_ERROR, true, \"Attribute(%s) in node(%s) is missing, but specified as mandatory.\\n\");", ID_ORIG(ATTRIBUTE_NAME(curr_attribute)), ID_ORIG(INODE_NAME(curr_node)));
            OUT_END_IF();
        }
    } else {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            LifetimeAttributeDisallowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node), access);
        } else if (ILIFETIME_TYPE(node) == LT_allowed) {
            LifetimeAttributeAllowed(ILIFETIME_BEGIN(node), ILIFETIME_END(node), access);
        } else if (ILIFETIME_TYPE(node) == LT_mandatory) {
            LifetimeAttributeMandatory(ILIFETIME_BEGIN(node), ILIFETIME_END(node), access);
        } else if (ILIFETIME_TYPE(node) == LT_optional) {
            LifetimeAttributeOptional(ILIFETIME_BEGIN(node), ILIFETIME_END(node), access);
        }
    }
    MEMfree(access);
}

node_st *DGCHTilifetime(node_st *node)
{
    struct data_dgcht *data = DATA_DGCHT_GET();
    if (data->lifetime_target == LT_NODE) {
        LifetimeNodeGen(node);
    } else if (data->lifetime_target == LT_CHILD) {
        LifetimeChildGen(node);
    } else if (data->lifetime_target == LT_ATTRIBUTE) {
        LifetimeAttributeGen(node);
    }

    return node;
}

node_st *DGCHTlifetime_range(node_st *node)
{
    return node;
}
