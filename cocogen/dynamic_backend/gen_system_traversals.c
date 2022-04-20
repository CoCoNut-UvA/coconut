
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

/**
 * Generates a vtable according to the given data.
 * The name of the vtable is `ccn_<trav_name>_vtable` and should correspond
 * to the one defined in vtable entries(see dynamic_backend/gen_trav_table.c).
 * 
 * The second parameter (trav_prefix) is the prefix or uid of the system traversal.
 * This will be used in the generation of the handler functions for the traversal.
 * It is a good idea to prefix this with `CCN_`.
 * Keep this in mind when generating your system traversal handlers
 */
static void GenSystemTraversalVtable(char *trav_prefix, char *trav_name)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT(system_trav_table_template, trav_name);
    prefix = trav_prefix;
    TRAVopt(AST_INODES(ast));
    OUT("};\n");
}

node_st *DGSTast(node_st *node)
{
    ast = node;
    GenSystemTraversalVtable("CPY", "copy");
    GenSystemTraversalVtable("CHK", "check");
    GenSystemTraversalVtable("DEL", "free");
    GenSystemTraversalVtable("ERR", "error");

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

