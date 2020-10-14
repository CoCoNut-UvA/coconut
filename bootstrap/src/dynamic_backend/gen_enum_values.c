#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

void *DGEVallocTravData()
{
    return 0;
}

void DGEVfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";


static void do_enum(char *name, node_st *node, char *prefix)
{
    OUT_ENUM("%s", name);
    OUT_ENUM_FIELD("%sNULL", prefix);
    TRAVopt(node);
    OUT_ENUM_FIELD("_%sSIZE", prefix);
    OUT_ENUM_END();
}

node_st *DGEVast(node_st *node)
{
    fp = globals.fp;
    TRAVopt(AST_ENUMS(node));
    return node;
}

node_st *DGEViactions(node_st *node)
{
    return node;
}

node_st *DGEViphase(node_st *node)
{
    return node;
}

node_st *DGEVitraversal(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVinode(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGEVste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGEVsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGEVsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGEVsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

static char *enum_prefx;
node_st *DGEVienum(node_st *node)
{
    OUT_ENUM("%s", ID_LWR(IENUM_NAME(node)));
    enum_prefx = ID_UPR(IENUM_IPREFIX(node));
    TRAVopt(IENUM_VALS(node));
    OUT_ENUM_END();
    TRAVopt(IENUM_NEXT(node));
    return node;
}

node_st *DGEVid(node_st *node)
{
    fp = globals.fp;
    OUT_ENUM_FIELD("%s_%s", enum_prefx, ID_LWR(node));
    TRAVchildren(node);
    return node;
}
