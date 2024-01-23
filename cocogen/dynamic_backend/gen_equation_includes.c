/**
 * @file
 *
 * Traversal: dynamicGenEquationIncludes
 * UID      : DGEI
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "globals.h"
#include "gen_helpers/out_macros.h"

/**
 * @fn DGEIast
 */
node_st *DGEIast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("\n");
    TRAVopt(AST_INODES(node));
    return node;
}

/**
 * @fn DGEIinode
 */
node_st *DGEIinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (INODE_IEQUATIONS(node)) {
        OUT("#include \"ccngen/equations/%s.h\"\n", ID_LWR(INODE_NAME(node)));
    }
    TRAVopt(INODE_NEXT(node));
    return node;
}
