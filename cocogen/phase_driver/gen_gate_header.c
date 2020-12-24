#include <gen_helpers/out_macros.h>
#include "ccngen/ast.h"
#include "globals.h"
#include "filesystem/gen_files.h"
#include "ccn/dynamic_core.h"

static FILE *fp = NULL;
static int indent = 0;

node_st *GGHast(node_st *node)
{
    fp = FSgetIncludeFile("gate.h");
    TRAVdo(AST_IPHASES(node));
    return node;
}

node_st *GGHiphase(node_st *node)
{
    if (IPHASE_GATE_FUNC(node)) {
        OUT("bool %s();", ID_ORIG(IPHASE_GATE_FUNC(node)));
    }
    TRAVopt(IPHASE_NEXT(node));
    return node;
}
