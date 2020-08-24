#include "ast/constructors.h"

#include "ast/ast.h"
#include "palm/memory.h"
#include "palm/str.h"
#include "palm/dbug.h"
#include "palm/ctinfo.h"


struct symbol_table_entry *ASTnewSymbolTableEntry(enum symbol_table_entry_type type, void *data)
{
    struct symbol_table_entry *entry = MEMmalloc(sizeof(struct symbol_table_entry));
    entry->data = data;
    entry->type = type;

    return entry;
}


struct phase *ASTnewPhaseHeader(struct id *name, bool is_start, bool is_cycle)
{
    struct phase *p = MEMmalloc(sizeof(struct phase));
    p->name = name;
    p->is_cycle = is_cycle;
    p->is_start = is_start;
    p->actions = NULL;
    p->loc_info = NULL;

    return p;
}

void ASTfillPhaseHeader(struct phase *header, struct id *prefix, char *info, struct action_slist *actions, struct id *gate_func)
{
    header->prefix = prefix;
    header->info = info;
    header->gate_func = gate_func;
    header->actions = actions;
}

struct pass *ASTnewPass(struct id *name, struct id *prefix, char *info, struct id *target_func)
{
    struct pass *pass = MEMmalloc(sizeof(struct pass));
    pass->name = name;
    pass->prefix = prefix;
    pass->target_func = target_func;
    pass->info = info;
    pass->loc_info = NULL;

    return pass;
}

struct traversal *ASTnewTraversal(struct id *name, struct id *prefix, char *info, struct ccn_set_expr *target_nodes)
{
    struct traversal *trav = MEMmalloc(sizeof(struct traversal));
    trav->name = name;
    trav->prefix = prefix;
    trav->info = info;
    trav->loc_info = NULL;
    trav->target_nodes_expr = target_nodes;

    return trav;
}

struct action *ASTnewAction(enum action_type action_type, void *new_action)
{
    struct action *action = MEMmalloc(sizeof(struct action));
    action->action_type = action_type;
    action->loc_info = NULL;

    switch(action_type) {
    case ACTION_REFERENCE:
        action->reference_to_action = (char *)new_action;
        break;
    case ACTION_PASS:
        action->pass = (struct pass *)new_action;
        break;
    case ACTION_TRAVERSAL:
        action->traversal = (struct traversal *)new_action;
        break;
    case ACTION_PHASE:
        action->phase = (struct phase *)new_action;
        break;
    default:
        DBUG_ASSERT(false, "Action type not known.");
        break;
    }
}

struct ccn_set_operation *ASTnewSetOperation(enum ccn_set_operator operator, struct ccn_set_expr *left, struct ccn_set_expr *right)
{
    struct ccn_set_operation *operation = MEMmalloc(sizeof(struct ccn_set_operation));
    operation->operator= operator;
    operation->left_child = left;
    operation->right_child = right;
    operation->loc_info = NULL;

    return operation;
}

struct ccn_set_expr *ASTnewSetExpr(enum ccn_set_type type, void *value)
{
    struct ccn_set_expr *expr = MEMmalloc(sizeof(struct ccn_set_expr));
    expr->type = type;
    expr->loc_info = NULL;

    switch (type) {
    case CCN_SET_REFERENCE:
        expr->reference = (struct id*)value;
        break;
    case CCN_SET_LITERAL:
        expr->set = (set_st *)value;
        break;
    case CCN_SET_OPERATION:
        expr->operation = (struct ccn_set_operation *)value;
        break;
    default:
        CTIabort("%s: no such type for a set expression", __func__);
    }
    return expr;
}

struct attribute *ASTnewAttribute(struct id *name, enum attribute_type type, bool in_constructor)
{
    struct attribute *attribute = MEMmalloc(sizeof(struct attribute));
    attribute->name = name;
    attribute->type = type;
    attribute->type_reference = NULL;
    attribute->in_constructor = in_constructor;
    attribute->include_file = NULL;
    attribute->loc_info = NULL;

    return attribute;
}

struct child *ASTnewChild(struct id *name, struct id *type, bool is_mandatory, bool in_constructor)
{
    struct child *child = MEMmalloc(sizeof(struct child));
    child->name = name;
    child->type_reference = type;
    child->is_mandatory = is_mandatory;
    child->in_constructor = in_constructor;
    child->type = CT_NULL;
    child->node = NULL;

    child->loc_info = NULL;

    return child;
}


struct node *ASTnewNode(struct id *name, char *info, struct attribute_slist *attributes, struct child_slist *children, bool is_root)
{
    struct node *node = MEMmalloc(sizeof(struct node));
    node->name = name;
    node->info = info;
    node->is_root = is_root;
    node->attributes = attributes;
    node->children = children;
    node->loc_info = NULL;

    return node;
}

struct nodeset *ASTnewNodeSet(struct id *name, char *info, struct ccn_set_expr *expr)
{
    struct nodeset *nodeset = MEMmalloc(sizeof(struct nodeset));
    nodeset->name = name;
    nodeset->info = info;
    nodeset->expr = expr;
    nodeset->type = NS_SETEXPR;

    nodeset->loc_info = NULL;

    return nodeset;
}


/**
 * @brief takes ownership of original.
 */
struct id *ASTnewId(char *original)
{
    struct id *id = MEMmalloc(sizeof(struct id));
    id->orig = original;
    id->lwr = STRlower(original);
    id->upr = STRupper(original);
    id->loc_info = NULL;

    return id;
}

struct ccn_enum *ASTnewEnum(struct id *name, struct id *prefix, char *info, struct id_slist *values)
{
    struct ccn_enum *enm = MEMmalloc(sizeof(struct ccn_enum));
    enm->name = name;
    enm->prefix = prefix;
    enm->info = info;
    enm->values = values;
    enm->loc_info = NULL;

    return enm;
}


struct ast *ASTnew()
{
    struct ast *ast = MEMmalloc(sizeof(struct ast));
    ast->phases = MEMmalloc(sizeof(struct phase_stailq));
    ast->traversals = MEMmalloc(sizeof(struct traversal_stailq));
    ast->passes = MEMmalloc(sizeof(struct pass_stailq));
    ast->nodes = MEMmalloc(sizeof(struct node_stailq));
    ast->nodesets = MEMmalloc(sizeof(struct nodeset_stailq));
    ast->enums = MEMmalloc(sizeof(struct ccn_enum_stailq));

    ast->root_node = NULL;
    ast->start_phase = NULL;
    ast->symbol_table = HTnew_String(10);

    STAILQ_INIT(ast->phases);
    STAILQ_INIT(ast->passes);
    STAILQ_INIT(ast->traversals);
    STAILQ_INIT(ast->nodes);
    STAILQ_INIT(ast->nodesets);
    STAILQ_INIT(ast->enums);

    return ast;
}