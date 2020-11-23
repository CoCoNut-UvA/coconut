#include <stddef.h>
#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "filesystem/gen_files.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int arg_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;
static node_st *curr_node;

node_st *DGCHTast(node_st *node)
{
    fp = FSgetSourceFile("ccn_check.c");
    globals.fp = fp;
    OUT("#include <stdbool.h>\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"ccn/phase_driver.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/ctinfo.h\"\n");
    ast = node;
    TRAVopt(AST_INODESETS(node));
    TRAVopt(AST_INODES(node));
    fclose(fp);
    globals.fp = 0;
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
    curr_node = node;
    OUT_START_FUNC("struct ccn_node *CHK%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    OUT_FIELD("size_t action_id = CCNgetCurrentActionId()");
    TRAVopt(INODE_ICHILDREN(node));
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
    OUT_START_FUNC("static bool TypeIs%s(node_st *arg_node)", ID_LWR(INODESET_NAME(node)));
    OUT_FIELD("enum ccn_nodetype node_type = NODE_TYPE(arg_node)");
    OUT("return (false");
    TRAVdo(INODESET_EXPR(node));
    OUT(");\n");
    OUT_END_FUNC();
    return node;
}

node_st *DGCHTchild(node_st *node)
{
    OUT_BEGIN_IF("%s_%s(arg_node)", ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
    {
        if (CHILD_TYPE(node) == CT_inode) {
            OUT_BEGIN_IF("NODE_TYPE(%s_%s(arg_node)) != NT_%s", ID_UPR(INODE_NAME(curr_node)),
                         ID_UPR(CHILD_NAME(node)), ID_UPR(CHILD_TYPE_REFERENCE(node)));
        } else {
            OUT_BEGIN_IF("!TypeIs%s(%s_%s(arg_node))", ID_LWR(CHILD_TYPE_REFERENCE(node)),
                         ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
        }
        OUT_FIELD("CTIerror(\"Inconsistent node found in AST. Child %s of node %s has disallowed type %%d \", NODE_TYPE(%s_%s(arg_node)))",
            ID_ORIG(CHILD_NAME(node)), ID_ORIG(INODE_NAME(curr_node)), ID_UPR(INODE_NAME(curr_node)), ID_UPR(CHILD_NAME(node)));
        OUT_END_IF();
    }
    OUT_END_IF();

    return node;
}

node_st *DGCHTattribute(node_st *node)
{

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
    // In case of an empty setliteral, we have 1 entry with no reference.
    if (node && SETLITERAL_REFERENCE(node)) {
        OUT_NO_INDENT(" || node_type == NT_%s", ID_UPR(SETLITERAL_REFERENCE(node)));
    }
    TRAVopt(SETLITERAL_NEXT(node));
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

node_st *DGCHTilifetime(node_st *node)
{
    if (!ILIFETIME_BEGIN(node) && !ILIFETIME_END(node)) {
        if (ILIFETIME_TYPE(node) == LT_disallowed) {
            OUT_FIELD("CTIerror(\"Found disallowed node(%s) in tree.\\n\");", ID_ORIG(INODE_NAME(curr_node)));
        }
    } else {
        OUT("if (");
        if (ILIFETIME_BEGIN(node)) {
            OUT_NO_INDENT("action_id >= %d && ", (LIFETIME_RANGE_ACTION_ID(ILIFETIME_BEGIN(node))));
        }
        if (ILIFETIME_END(node)) {
            OUT_NO_INDENT("action_id <= %d && ", (LIFETIME_RANGE_ACTION_ID(ILIFETIME_END(node))));
        }
        OUT_NO_INDENT("true) {\n");
        indent++;
        OUT_FIELD("CTIerror(\"Found disallowed node(%s) in tree.\\n\")", ID_ORIG(INODE_NAME(curr_node)));
        OUT_END_IF();


    }
    return node;
}

node_st *DGCHTlifetime_range(node_st *node)
{
    return node;
}
