
#include "ast/ast.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/memory.h"

static
bool EntryIsAction(struct symbol_table_entry *entry)
{
    return entry != NULL && (entry->type == STE_PASS || entry->type == STE_PHASE || entry->type == STE_TRAVERSAL);
}

bool CheckActionUpperAndLower(struct ast *ast, struct action *action)
{
    bool found = false;

    char *upr = STRupper(action->reference_to_action);
    char *lwr = STRlower(action->reference_to_action);
    struct symbol_table_entry *entry = HTlookup(ast->symbol_table, upr, &found);
    if (found) {
        if (EntryIsAction(entry)) {
            CTIwarnObj(action->loc_info, "Found indirect reference in uppercase form.");
        } else {
            CTIerrorObj(action->loc_info, "Found indirect reference in uppercase form, but reference is not an action.");
        }
    } else {
        entry = HTlookup(ast->symbol_table, lwr, &found);
        if (found) {
            if (EntryIsAction(entry)) {
                CTIwarnObj(action->loc_info, "Found indirect reference in lowercase form.");
            } else {
                CTIerrorObj(action->loc_info, "Found indirect reference in lowercase form, but reference is not an action.");
            }
        }
    }


    MEMfree(upr);
    MEMfree(lwr);
    return found;
}

void CheckLiteralSetExpr(struct ast *ast, struct id *value)
{
    bool found = false;
    if (HTlookup(ast->symbol_table, value->orig, &found) == NULL) {
        CTIerrorObj(value->loc_info, "Inline set expression references an undefined node");
    }
}

void CheckSetReference(struct ast *ast, struct ccn_set_expr *expr)
{
    bool found = false;
    struct symbol_table_entry *entry = HTlookup(ast->symbol_table, expr->reference->orig, &found);
    if (found && entry != NULL) {
        if (entry->type != STE_NODESET) {
            CTIerrorObj(expr->reference->loc_info, "Set expressions references an entity that is not a nodeset");
        }
    } else {
        CTIerrorObj(expr->reference->loc_info, "Set expression references an undefined nodeset");
    }
}

void CheckSetExpr(struct ast *ast, struct ccn_set_expr *expr)
{
    switch(expr->type) {
    case CCN_SET_LITERAL:
        SETfold(expr->set, (fold_ft)CheckLiteralSetExpr, ast);
        break;
    case CCN_SET_OPERATION:
        CheckSetExpr(ast, expr->operation->left_child);
        CheckSetExpr(ast, expr->operation->right_child);
        break;
    case CCN_SET_REFERENCE:
        CheckSetReference(ast, expr);
        break;
    default:
        break;
    }
}

void CheckNodesets(struct ast *ast)
{
    bool error = false;
    struct nodeset *nodeset = NULL;
    STAILQ_FOREACH(nodeset, ast->nodesets, next) {
        CheckSetExpr(ast, nodeset->expr);
    }
}

static
void CheckNode(struct ast *ast, struct node *node)
{
    struct child *child = NULL;
    SLIST_FOREACH(child, node->children, next) {
        bool found = false;
        struct symbol_table_entry *entry = HTlookup(ast->symbol_table, child->type_reference->orig, &found);
        if (!found || entry == NULL) {
            CTIerrorObj(child->type_reference->loc_info, "Type of child is not valid.");
            continue;
        }

        if (entry->type == STE_NODESET) {
            child->type = CT_NODESET;
            child->nodeset = entry->data;
        } else if (entry->type == STE_NODE) {
            child->type = CT_NODE;
            child->node = entry->data;
        }
    }
    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, node->attributes, next) {
        if (attr->type == AT_link_or_enum) {
            bool found = false;
            struct symbol_table_entry *entry = HTlookup(ast->symbol_table, attr->type_reference->orig, &found);
            if (!found || entry == NULL) {
                CTIerrorObj(attr->type_reference->loc_info, "Attribute type is not valid.");
            }
            if (entry->type == STE_ENUM) {
                attr->type = AT_enum;
            } else if (entry->type == STE_NODE || entry->type == STE_NODESET) {
                attr->type = AT_link;
            } else {
                CTIerrorObj(attr->type_reference->loc_info, "Attribute type is not valid.");
            }
        }
    }
}

struct ast *CEdoTrav(struct ast *ast)
{
    struct phase *phase;
    STAILQ_FOREACH(phase, ast->phases, next) {
        if (phase->is_start) {
            if (ast->start_phase == NULL) ast->start_phase = phase;
            else {
                CTIerrorObj(phase->name->loc_info, "Multiple start phases defined. Only one phase can be the starting phase.");
            }
        }
        struct action *action;
        SLIST_FOREACH(action, phase->actions, next) {
            if (action->action_type == ACTION_REFERENCE) {
                bool found = false;
                struct symbol_table_entry *entry = HTlookup(ast->symbol_table, action->reference_to_action, &found);
                if (found && !EntryIsAction(entry)) {
                    CTIerrorObj(action->loc_info, "Reference is not a valid action.");
                } else if (!found) {
                    if (!CheckActionUpperAndLower(ast, action)) {
                        CTIerrorObj(action->loc_info, "Reference could not be found.");
                    }
                } else {
                    MEMfree(action->reference_to_action);
                    if (entry->type == STE_PASS) {
                        action->pass = entry->data;
                        action->action_type = ACTION_PASS;
                    } else if (entry->type == STE_PHASE) {
                        action->phase = entry->data;
                        action->action_type = ACTION_PHASE;
                    } else if (entry->type == STE_TRAVERSAL) {
                        action->traversal = entry->data;
                        action->action_type = ACTION_TRAVERSAL;
                    }
                }
            }
        }
    }

    struct traversal *trav;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (trav->target_nodes_expr == NULL) {
            continue;
        }

        CheckSetExpr(ast, trav->target_nodes_expr);
    }

    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        if (node->is_root) {
            if (ast->root_node == NULL) ast->root_node = node;
            else {
                CTIerrorObj(node->name->loc_info, "Multiple nodes are defined as root. Only one node can be the root node.");
            }
        }
        CheckNode(ast, node);
    }

    CheckNodesets(ast);

    if (ast->start_phase == NULL) {
        CTIerror("No start phase specified. There must be one and only one start phase defined");
    }

    if (ast->root_node == NULL) {
        CTIerror("No root node specified. There must be one and only one root node defined");
    }

    CTIabortOnError();
    return ast;
}