
#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "filesystem/gen_files.h"

void *DGCTallocTravData()
{
    return 0;
}

void DGCTfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int arg_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;
static node_st *curr_node;

node_st *DGCTast(node_st *node)
{
    fp = FSgetSourceFile("ccn_copy.c");
    globals.fp = fp;
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include <stddef.h>\n");
    OUT("#include \"palm/str.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    TRAVopt(AST_INODES(node));
    fclose(fp);
    globals.fp = 0;
    return node;
}

node_st *DGCTiactions(node_st *node)
{
    return node;
}

node_st *DGCTiphase(node_st *node)
{
    return node;
}

node_st *DGCTitraversal(node_st *node)
{
    return node;
}

node_st *DGCTitravdata(node_st *node)
{
    return node;
}

node_st *DGCTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGCTinode(node_st *node)
{
    curr_node = node;
    arg_num = 0;
    OUT_START_FUNC("struct ccn_node *CPY%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    TRAVstart(node, TRAV_DYNAMIC_GENCONSTRUCTORCALL);
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    OUT_FIELD("return new_node");
    OUT_END_FUNC();
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGCTinodeset(node_st *node)
{
    return node;
}

node_st *DGCTchild(node_st *node)
{
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *child_name = ID_UPR(CHILD_NAME(node));
    OUT_FIELD("%s_%s(new_node) = TRAVopt(%s_%s(arg_node))", node_name, child_name, node_name, child_name);
    TRAVopt(CHILD_NEXT(node));
    return node;
}

node_st *DGCTattribute(node_st *node)
{
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *attr_name = ID_UPR(ATTRIBUTE_NAME(node));
    if (ATTRIBUTE_TYPE(node) == AT_string) {
        OUT_FIELD("%s_%s(new_node) = STRcpy(%s_%s(arg_node))", node_name, attr_name, node_name, attr_name);
    } else {
        OUT_FIELD("%s_%s(new_node) = %s_%s(arg_node)", node_name, attr_name, node_name, attr_name);
    }
    TRAVopt(ATTRIBUTE_NEXT(node));
    return node;
}

node_st *DGCTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTienum(node_st *node)
{
    return node;
}

node_st *DGCTid(node_st *node)
{
    return node;
}
