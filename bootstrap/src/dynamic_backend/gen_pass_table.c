
#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGPTallocTravData()
{
    return 0;
}

void DGPTfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;

node_st *DGPTast(node_st *node)
{
    fp = stdout;
    ast = node;
    OUT("const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE] = { &PASSerror, ");
    TRAVopt(AST_IPASSES(node));
    OUT("};\n");
    return node;
}

node_st *DGPTiactions(node_st *node)
{
    return node;
}

node_st *DGPTiphase(node_st *node)
{
    return node;
}

node_st *DGPTitraversal(node_st *node)
{
    return node;
}

node_st *DGPTipass(node_st *node)
{
    char *prefix = "";
    char *func = ID_LWR(IPASS_NAME(node));
    if (IPASS_IPREFIX(node)) {
        prefix = ID_UPR(IPASS_IPREFIX(node));
    }
    if (IPASS_TARGET_FUNC(node)) {
        func = ID_ORIG(IPASS_TARGET_FUNC(node));
    }
    OUT("&%s%s, ", prefix, func);
    TRAVopt(IPASS_NEXT(node));
    return node;
}

node_st *DGPTinode(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGPTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGPTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGPTattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGPTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGPTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGPTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGPTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

static char *enum_prefx;
node_st *DGPTienum(node_st *node)
{
    return node;
}

node_st *DGPTid(node_st *node)
{
    return node;
}
