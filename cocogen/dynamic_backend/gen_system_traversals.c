
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

node_st *DGSTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    ast = node;
    OUT(system_trav_table_template, "copy");
    prefix = "CPY";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT(system_trav_table_template, "check");
    prefix = "CHK";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT(system_trav_table_template, "free");
    prefix = "DEL";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT(system_trav_table_template, "error");
    prefix = "ERR";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

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

