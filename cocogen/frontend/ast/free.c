#include <stdio.h>

#include "ast/ast.h"

#include "lib/array.h"
#include "lib/memory.h"

static void free_commoninfo(NodeCommonInfo *info) {
    if (info->hash)
        mem_free(info->hash);
    mem_free(info);
}


static void free_range_spec(Range_spec_t *spec) {
    mem_free(spec->consistency_key);
    mem_free(spec->type);
    array_cleanup(spec->ids, mem_free);
    mem_free(spec);
}

static void free_lifetime(void *p) {
    Lifetime_t *lifetime = p;
    if (lifetime->owner) {
        free_range_spec(lifetime->start);
        free_range_spec(lifetime->end);
    }
    mem_free(lifetime->key);
    mem_free(lifetime);

}

void free_setOperation(SetOperation *operation);

void free_setExpr(SetExpr *expr) {
    switch (expr->type) {
    case SET_REFERENCE:
        mem_free(expr->ref_id);
        break;
    case SET_LITERAL:
        if (expr->id_set == NULL)
            break;
        ccn_set_free(expr->id_set);
        break;
    case SET_OPERATION:
        free_setOperation(expr->operation);
    }
    free_commoninfo(expr->common_info);
    mem_free(expr);
}

void free_setOperation(SetOperation *operation) {
    free_commoninfo(operation->common_info);
    free_setExpr(operation->left_child);
    free_setExpr(operation->right_child);
    mem_free(operation);
}

static void free_phase(void *);

static void free_action(void *a) {
    Action *action = a;
    if (action->type == ACTION_PHASE) {
        if (((Phase*)action->action)->original_ref != NULL) {
            free_phase(action->action);
        }
    }
    smap_free(action->active_specs);
    mem_free(action->id);
    mem_free(action);
}

static void free_phase(void *p) {
    Phase *phase = p;

    mem_free(phase->id);
    free_commoninfo(phase->common_info);
    if (phase->root != NULL) {
        mem_free(phase->root);
    }
    ccn_set_free(phase->roots);
    mem_free(phase->prefix);
    array_cleanup(phase->actions, free_action);
    smap_free(phase->active_specs);
    mem_free(phase);
}

static void free_pass(void *p) {
    Pass *pass = p;

    mem_free(pass->prefix);
    mem_free(pass->id);
    free_commoninfo(pass->common_info);
    mem_free(pass);
}

static void free_traversal(void *p) {
    Traversal *traversal = p;
    if (traversal->nodes != NULL)
        array_cleanup(traversal->nodes, mem_free);

    mem_free(traversal->prefix);
    mem_free(traversal->id);
    free_commoninfo(traversal->common_info);
    mem_free(traversal);
}

static void free_enum(void *p) {
    Enum *attr_enum = p;
    if (attr_enum->values != NULL)
        array_cleanup(attr_enum->values, mem_free);

    mem_free(attr_enum->id);
    mem_free(attr_enum->prefix);
    free_commoninfo(attr_enum->common_info);
    mem_free(attr_enum);
}

static void free_nodeset(void *p) {
    Nodeset *nodeset = p;
    if (nodeset->nodes != NULL)
        array_cleanup(nodeset->nodes, NULL);

    mem_free(nodeset->id);
    free_commoninfo(nodeset->common_info);
    mem_free(nodeset);
}

static void free_child(void *p) {
    Child *c = p;


    mem_free(c->id);
    if (c->type != NULL)
        mem_free(c->type);

    if (c->lifetimes != NULL)
        array_cleanup(c->lifetimes, free_lifetime);

    free_commoninfo(c->common_info);
    mem_free(c);
}

static void free_attr(void *p) {
    Attr *a = p;
    mem_free(a->id);
    if (a->type_id != NULL)
        mem_free(a->type_id);

    if (a->default_value != NULL) {
        if (a->default_value->type == AV_string ||
            a->default_value->type == AV_id) {
            free_commoninfo(a->default_value->common_info);
            mem_free(a->default_value->value.string_value);
        }
        mem_free(a->default_value);
    }
    if (a->lifetimes != NULL)
        array_cleanup(a->lifetimes, free_lifetime);
    free_commoninfo(a->common_info);
    mem_free(a);
}

static void free_node(void *p) {
    Node *node = p;

    if (node->children != NULL)
        array_cleanup(node->children, free_child);

    if (node->attrs != NULL)
        array_cleanup(node->attrs, free_attr);

    if (node->lifetimes != NULL)
        array_cleanup(node->lifetimes, free_lifetime);

    mem_free(node->id);
    free_commoninfo(node->common_info);
    mem_free(node);
}

void free_config(Config *config) {
    array_cleanup(config->phases, free_phase);
    array_cleanup(config->passes, free_pass);
    array_cleanup(config->traversals, free_traversal);
    array_cleanup(config->enums, free_enum);
    array_cleanup(config->nodesets, free_nodeset);
    array_cleanup(config->nodes, free_node);

    free_commoninfo(config->common_info);
    mem_free(config);
}
