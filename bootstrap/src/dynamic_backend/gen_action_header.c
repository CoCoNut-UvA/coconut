#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "filesystem/gen_files.h"

void *DGAHallocTravData()
{
    return 0;
}

void DGAHfreeTravData(void *data)
{

}

FILE *fp;
char *current_trav_prefix = NULL;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *prefix = NULL;
static node_st *ast;

node_st *DGAHast(node_st *node)
{
    fclose(globals.fp);
    globals.fp = FSgetIncludeFile("actions.h");
    fp = globals.fp;
    ast = node;
    TRAVchildren(node);
    fclose(fp);
    globals.fp = 0;
    return node;
}

node_st *DGAHitraversal(node_st *node)
{
    current_trav_prefix = ID_UPR(ITRAVERSAL_IPREFIX(node));
    if (ITRAVERSAL_INODES(node)) {
        TRAVdo(ITRAVERSAL_INODES(node));
    } else {
        TRAVstart(AST_INODES(ast), TRAV_DYNAMIC_GENTRAVACTIONS);
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *DGAHipass(node_st *node)
{
    char *prefix = "";
    if (IPASS_IPREFIX(node)) {
        prefix = ID_UPR(IPASS_IPREFIX(node));
    }
    OUT_FIELD("struct ccn_node *%s%s(struct ccn_node *)", prefix, ID_ORIG(IPASS_NAME(node)));
    TRAVopt(IPASS_NEXT(node));
    return node;
}

node_st *DGAHinode(node_st *node)
{
    OUT_FIELD("struct ccn_node *CPY%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    OUT_FIELD("struct ccn_node *DEL%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    OUT_FIELD("struct ccn_node *CHK%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    TRAVopt(INODE_NEXT(node));

    return node;
}

node_st *DGAHsetliteral(node_st *node)
{
    TRAVopt(SETLITERAL_REFERENCE(node));
    TRAVchildren(node);
    return node;
}


node_st *DGAHid(node_st *node)
{
    OUT_FIELD("struct ccn_node *%s%s(struct ccn_node *node)", current_trav_prefix, ID_LWR(node));
    TRAVchildren(node);
    return node;
}
