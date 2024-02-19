#pragma once
#include "ccn/ccn_types.h"

/**
 * @fn CCNevaluateAttributes(node_st *node)
 * Evaluates all attributes starting from the provided node.
 *
 * The node cannot have inherited attributes.
 * Pass the root node to evaluate all the attributes in the AST.
 */
void CCNevaluateAttributes(node_st *node);
