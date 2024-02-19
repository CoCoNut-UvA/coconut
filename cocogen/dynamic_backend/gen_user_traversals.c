#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "gen_helpers/out_macros.h"
#include <err.h>

static char *curr_trav;
static node_st *ast;

node_st *DUGTast(node_st *node)
{
    ast = node;
    TRAVitraversals(node);
    return node;
}

node_st *DUGTitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    {
        char *filename = STRfmt("trav_%s.c", ID_LWR(ITRAVERSAL_NAME(node)));
        GNopenUserFile(ctx, filename);
        MEMfree(filename);
    }

    OUT("/**\n");
    OUT(" * @file\n");
    OUT(" *\n");
    OUT(" * Traversal: %s\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    OUT(" * UID      : %s\n", ID_UPR(ITRAVERSAL_IPREFIX(node)));
    OUT(" *\n");
    OUT(" *\n");
    if (ITRAVERSAL_IINFO(node)) {
        OUT(" * @brief %s\n", ITRAVERSAL_IINFO(node));
    }
    OUT(" */\n\n");
    OUT("#include \"ccn/ccn.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccngen/trav.h\"\n");
    OUT("\n");

    curr_trav = ID_UPR(ITRAVERSAL_IPREFIX(node));
    if (ITRAVERSAL_DATA(node)) {
        OUT("void %sinit() { return; }\n", curr_trav);
        OUT("void %sfini() { return; }\n", curr_trav);
        OUT("\n");
    }
    if (ITRAVERSAL_INODES(node)) {
        TRAVinodes(node);
    } else {
        TRAVinodes(ast);
    }

    TRAVnext(node);
    return node;
}

node_st *DUGTinode(node_st *node)
{
    TRAVnext(node);
    TRAVname(node);
    return node;
}

node_st *DUGTid(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("/**\n");
    OUT(" * @fn %s%s\n", curr_trav, ID_LWR(node));
    OUT(" */\n");
    OUT("node_st *%s%s(node_st *node)\n{\n    TRAVchildren(node);\n    return node;\n}\n\n", curr_trav, ID_LWR(node));
    TRAVnext(node);
    return node;
}
