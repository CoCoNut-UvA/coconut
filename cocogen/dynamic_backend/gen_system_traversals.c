
#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"



static char *prefix = NULL;
static node_st *ast;
static char *system_trav_table_template = "const ccn_trav_ft ccn_%s_vtable[_NT_SIZE] = { &TRAVerror, ";

static void GenSystemTraversalVtable(node_st *node, char *pref, char *trav_name)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT(system_trav_table_template, trav_name);
    prefix = pref;
    TRAVopt(AST_INODES(node));
    OUT("};\n");
}

node_st *DGSTast(node_st *node)
{
    ast = node;
    GenSystemTraversalVtable(node, "CPY", "copy");
    GenSystemTraversalVtable(node, "CHK", "check");
    GenSystemTraversalVtable(node, "DEL", "free");
    GenSystemTraversalVtable(node, "ERR", "error");

    return node;
}

node_st *DGSTinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    // Important, OUT should be before the traversal.
    if (STReq(prefix, "ERR")) {
        OUT("&TRAVerror,");
    } else {
        OUT("&%s%s, ", prefix, ID_LWR(INODE_NAME(node)));
    }
    TRAVopt(INODE_NEXT(node));

    return node;
}

