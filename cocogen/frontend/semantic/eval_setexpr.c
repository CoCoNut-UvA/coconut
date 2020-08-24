
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "palm/dbug.h"
#include "palm/memory.h"
#include "palm/ctinfo.h"
#include "palm/set.h"

static
void EvalSetOperation(struct ast *ast, struct ccn_set_expr *expr)
{
    set_st *new = NULL;
    switch(expr->operation->operator) {
    case CCN_SET_UNION:
        new = SETunion(expr->operation->left_child->set, expr->operation->right_child->set);
        break;
    case CCN_SET_INTERSECT:
        new = SETintersect(expr->operation->left_child->set, expr->operation->right_child->set);
        break;
    case CCN_SET_DIFFERENCE:
        new = SETdifference(expr->operation->left_child->set, expr->operation->right_child->set);
        break;
    }
    SETdelete(expr->operation->left_child->set);
    SETdelete(expr->operation->right_child->set);
    MEMfree(expr->operation);
    expr->set = new;
    expr->type = CCN_SET_LITERAL;
}

static
void EvalSetExpr(struct ast *ast, struct ccn_set_expr *expr)
{
    bool found = false;
    struct symbol_table_entry *entry = NULL;
    struct nodeset *nodeset = NULL;

    switch(expr->type) {
    case CCN_SET_LITERAL:
        break;
    case CCN_SET_REFERENCE:
        entry = HTlookup(ast->symbol_table, expr->reference->orig, &found);
        nodeset = (struct nodeset*)entry->data;
        DBUG_ASSERT(found, "Nodeset not found, but should be present due to previous traversal");
        EvalSetExpr(ast, nodeset->expr);
        DBUG_ASSERT(nodeset->expr->type == CCN_SET_LITERAL, "Nodeset expr should be a literal but it isnt.");
        MEMfree(expr->reference);
        expr->set = SETcpy(nodeset->expr->set);
        expr->type = CCN_SET_LITERAL;
        break;
    case CCN_SET_OPERATION:
        EvalSetExpr(ast, expr->operation->left_child);
        EvalSetExpr(ast, expr->operation->right_child);
        EvalSetOperation(ast, expr);
    }
}

static
void PrintSetExpression_Id(void *data, struct id *id)
{
    printf(" %s,", id->orig);
}

static
void CheckSetExpressions(struct ccn_set_expr *expr, bool can_be_null, struct id *owner)
{
    if (expr == NULL && !can_be_null) {
        DBUG_ASSERT(false, "Set expression is NULL but that is not allowed.");
    }
    DBUG_ASSERT((expr == NULL || expr->type == CCN_SET_LITERAL), "Set expression is not of type literal, while it should.");
    if (expr != NULL) {
        printf("%s : {", owner->orig);
        SETfold(expr->set, (fold_ft)PrintSetExpression_Id, NULL);
        printf("}\n");
    } else {
        printf("%s goes over all nodes\n", owner->orig);
    }
}

struct fold_helper {
    struct ast *ast;
    struct node_wrapper_slist *list;
};

void AddNodeToList(struct fold_helper *helper, struct id *node_id)
{
    bool found = false;
    struct symbol_table_entry *entry = HTlookup(helper->ast->symbol_table, node_id->orig, &found);
    DBUG_ASSERT((entry != NULL && found), "Node not present in symbol table or equals NULL");
    DBUG_ASSERT(entry->type == STE_NODE, "Not of type Node");

    struct node_wrapper *nodeset_node = MEMmalloc(sizeof(struct node_wrapper));
    nodeset_node->node = (struct node *)entry->data;
    SLIST_INSERT_HEAD(helper->list, nodeset_node, next);
}

struct ast *ESEdoTrav(struct ast *ast)
{

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (trav->target_nodes_expr == NULL) {
            continue;
        }
        EvalSetExpr(ast, trav->target_nodes_expr);
#ifndef NDEBUG
        CTInote("Checking set expression for traversals");
        CheckSetExpressions(trav->target_nodes_expr, true, trav->name);
#endif
    }

    struct nodeset *nodeset = NULL;
    STAILQ_FOREACH(nodeset, ast->nodesets, next) {
        EvalSetExpr(ast, nodeset->expr);
#ifndef NDEBUG
        CTInote("Checking set expression for nodesets");
        CheckSetExpressions(nodeset->expr, false, nodeset->name);
#endif
    }

    nodeset = NULL;

    STAILQ_FOREACH(nodeset, ast->nodesets, next) {
        struct node_wrapper_slist *nodes = MEMmalloc(sizeof(struct node_wrapper_slist));
        SLIST_INIT(nodes);
        struct fold_helper helper = {.ast = ast, .list = nodes};
        SETfold(nodeset->expr->set, (fold_ft)AddNodeToList, &helper);
        // TODO: free the set.

        nodeset->nodes = nodes;
        nodeset->type = NS_LISTOFNODES;
    }

    trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (trav->target_nodes_expr == NULL) {
            trav->nodes = NULL;
            continue;
        }
        struct node_wrapper_slist *nodes = MEMmalloc(sizeof(struct node_wrapper_slist));
        SLIST_INIT(nodes);
        struct fold_helper helper = {.ast = ast, .list = nodes};

        SETfold(trav->target_nodes_expr->set, (fold_ft)AddNodeToList, &helper);

        // TODO: free the set.
        trav->target_nodes_expr = NULL;
        trav->nodes = nodes;
    }

    return ast;
}