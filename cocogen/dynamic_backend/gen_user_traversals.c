#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "gen_helpers/out_macros.h"
#include <err.h>

static char *curr_trav;
static FILE *fp;
static node_st *ast;

node_st *DUGTast(node_st *node)
{
    ast = node;
    TRAVopt(AST_ITRAVERSALS(node));
    return node;
}

node_st *DUGTitraversal(node_st *node)
{
    char *filename = STRcatn(4, globals.gen_user_dir, "trav_", ID_LWR(ITRAVERSAL_NAME(node)), ".c");
    fp = fopen(filename, "w");
    if (!fp) {
        err(EXIT_FAILURE, "Can not open user file: %s", filename);
    }
    int indent = 0;
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("\n");
    curr_trav = ID_UPR(ITRAVERSAL_IPREFIX(node));
    if (ITRAVERSAL_INODES(node)) {
        TRAVdo(ITRAVERSAL_INODES(node));
    } else {
        TRAVdo(AST_INODES(ast));
    }

    fclose(fp);
    MEMfree(filename);

    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *DUGTinode(node_st *node)
{
    TRAVopt(INODE_NEXT(node));
    TRAVdo(INODE_NAME(node));
    return node;
}

node_st *DUGTid(node_st *node)
{
    int indent = 0;
    OUT("node_st *%s%s(node_st *node)\n{\n    return node;\n}\n\n", curr_trav, ID_LWR(node));
    TRAVopt(ID_NEXT(node));
    return node;
}
