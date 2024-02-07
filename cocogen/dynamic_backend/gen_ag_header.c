/**
 * @file
 *
 * Traversal: dynamicGenAGHeader
 * UID      : DGAGH
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "gen_helpers/format.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"

static char *basic_node_type = "node_st";

/**
 * @fn DGAGHast
 */
node_st *DGAGHast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;

    OUT("/**\n");
    OUT(" * @fn CCNevaluateAttributes(%s *node)\n", basic_node_type);
    OUT(" * Evaluates all attributes starting from the provided node.\n");
    OUT(" *\n");
    OUT(" * The node cannot have inherited attributes.\n");
    OUT(" * Pass the root node to evaluate all the attributes in the AST.\n");
    OUT(" */\n");
    OUT("void CCNevaluateAttributes(%s *node);\n", basic_node_type);

    return node;
}
