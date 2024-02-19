#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"

static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static char *curr_nodeset_name = NULL;
static char *curr_nodeset_name_upr = NULL;
static int child_num = 0;

node_st *DGNMast(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGNMinodeset(node_st *node)
{
    curr_nodeset_name = ID_LWR(INODESET_NAME(node));
    curr_nodeset_name_upr = ID_UPR(INODESET_NAME(node));
    TRAViattributes(node);
    curr_nodeset_name_upr = NULL;
    curr_nodeset_name = NULL;
    TRAVnext(node);
    return node;
}

node_st *DGNMinode(node_st *node)
{
    curr_node_name = ID_LWR(INODE_NAME(node));
    curr_node_name_upr = ID_UPR(INODE_NAME(node));
    TRAVichildren(node);
    TRAViattributes(node);
    curr_node_name_upr = NULL;
    curr_node_name = NULL;
    TRAVnext(node);
    return node;
}

node_st *DGNMchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node_name);
    child_num++;
    OUT("#define %s_%s(n) (*CCNaccess_%s_%s(__LINE__, __FILE__, __func__, n))\n", curr_node_name_upr, ID_UPR(CHILD_NAME(node)), curr_node_name, ID_LWR(CHILD_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGNMattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (!ATTRIBUTE_IS_INHERITED(node) && !ATTRIBUTE_IS_SYNTHESIZED(node)) {
        if (curr_node_name) {
            OUT("#define %s_%s(n) (*CCNaccess_%s_%s(__LINE__, __FILE__, __func__, n))\n", curr_node_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), curr_node_name, ID_LWR(ATTRIBUTE_NAME(node)));
        } else {
            assert(curr_nodeset_name);
            OUT("#define %s_%s(n) (*CCNaccess_%s_%s(__LINE__, __FILE__, __func__, n))\n", curr_nodeset_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), curr_nodeset_name, ID_LWR(ATTRIBUTE_NAME(node)));
        }
    } else {
        if (curr_node_name) {
            OUT("#define %s_%s(n) CCNaccess_readonly_%s_%s(__LINE__, __FILE__, __func__, n)\n", curr_node_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), curr_node_name, ID_LWR(ATTRIBUTE_NAME(node)));
        } else {
            assert(curr_nodeset_name);
            OUT("#define %s_%s(n) CCNaccess_readonly_%s_%s(__LINE__, __FILE__, __func__, n)\n", curr_nodeset_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), curr_nodeset_name, ID_LWR(ATTRIBUTE_NAME(node)));
        }
    }
    TRAVchildren(node);
    return node;
}
