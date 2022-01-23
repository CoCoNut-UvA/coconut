#pragma once

#include "ccngen/ast.h"
#include "../gen_helpers.h"
#include "generator/generator.h"

char *DGHpassFuncName(node_st *pass);
char *DGHattributeField(node_st *attr);
char *DGHattributeAccess(node_st *node, node_st *attr, char *arg);
void DGHchildAcces(GeneratorContext *ctx, node_st *node, node_st *name, char *arg);

/* Specified how the node (type) part of a traversal handler is formatted. */
#define DGH_TRAVERSAL_TARGET_ID(node) ID_LWR(node)
#define DGH_TRAVERSAL_PREFIX(trav) ID_UPR(ITRAVERSAL_IPREFIX(trav))
#define DGH_PASS_PREFIX(pass) ID_UPR(IPASS_IPREFIX(pass))
#define DGH_PASS_NAME(pass) ID_ORIG(IPASS_NAME(pass))
#define DGH_PASS_FUNC_SIG() GH_DEFAULT_NODE_TYPE() " *%s%s(" GH_DEFAULT_NODE_TYPE() " *%s)"
#define DGH_TRAV_FUNC_SIG() GH_DEFAULT_NODE_TYPE() " *%s%s(" GH_DEFAULT_NODE_TYPE() " *%s)"
#define DGH_NODE_CONSTR_CALL_SIG()  "AST%s("
#define DGH_NODE_CONSTR_TARGET(node) ID_LWR(INODE_NAME(node))
