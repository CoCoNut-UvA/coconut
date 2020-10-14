#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *GITallocTravData()
{
    return 0;
}

void GITfreeTravData(void *data)
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

static char *enum_action_pref = "CCNAC_ID";
static char *enum_action_name = "ccn_action_id";


node_st *GITast(node_st *node)
{
    fp = stdout;
    ast = node;
    OUT("enum %s cleanup_ids_table[2] = { %s_free, %s_NULL };\n", enum_action_name, enum_action_pref, enum_action_pref);
    TRAVchildren(node);
    OUT("};\n");
    return node;
}

node_st *GITiactions(node_st *node)
{
    OUT("%s_%s, ", enum_action_pref, ID_UPR(IACTIONS_REFERENCE(node)));
    return node;
}

node_st *GITiphase(node_st *node)
{
    OUT("enum %s %s_ids_table[] = {\n", enum_action_name, ID_LWR(IPHASE_NAME(node)));
    TRAVopt(IPHASE_IACTIONS(node));
    OUT("%s_NULL, ", enum_action_pref);
    OUT_ENUM_END();
    return node;
}

node_st *GITitraversal(node_st *node)
{
    return node;
}

node_st *GITipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *GITinode(node_st *node)
{

    return node;
}

node_st *GITinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *GITchild(node_st *node)
{

    return node;
}

node_st *GITattribute(node_st *node)
{

    return node;
}

node_st *GITste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *GITsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *GITsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *GITsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *GITienum(node_st *node)
{
    return node;
}

node_st *GITid(node_st *node)
{
    return node;
}
