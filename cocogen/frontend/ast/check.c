#include "ast/check.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/create.h"
#include "ast/free.h"
#include "ast/util.h"

#include "lib/array.h"
#include "lib/memory.h"
#include "lib/print.h"
#include "lib/set.h"
#include "lib/set_implementations.h"
#include "lib/smap.h"
#include "lib/str.h"

struct Info {
    smap_t *enum_name;
    smap_t *enum_prefix;
    smap_t *node_name;
    smap_t *nodeset_name;
    smap_t *traversal_name;
    smap_t *phase_name;
    smap_t *pass_name;
    smap_t *action_prefix;
    ccn_set_t *functions;

    Config *config;
    Node *root_node;
    Phase *root_phase;
};

static struct Info *create_info(Config *config) {

    struct Info *info = (struct Info *)mem_alloc(sizeof(struct Info));
    // 32 is a magic number.
    info->enum_name = smap_init(32);
    info->enum_prefix = smap_init(32);
    info->node_name = smap_init(32);
    info->nodeset_name = smap_init(32);
    info->traversal_name = smap_init(32);
    info->phase_name = smap_init(32);
    info->pass_name = smap_init(32);
    info->action_prefix = smap_init(32);
    info->functions = ccn_set_string_create(32);

    info->config = config;
    info->root_node = NULL;
    info->root_phase = NULL;
    return info;
}

static void free_info(struct Info *info) {
    smap_free(info->enum_name);
    smap_free(info->enum_prefix);
    smap_free(info->node_name);
    smap_free(info->nodeset_name);
    smap_free(info->traversal_name);
    smap_free(info->phase_name);
    smap_free(info->pass_name);
    smap_free(info->action_prefix);
    ccn_set_free(info->functions);
    mem_free(info);
}

static void *check_prefix_exists(struct Info *info, char *prefix) {
    return smap_retrieve(info->action_prefix, prefix);
}

static char *gen_unique_prefix_from_existing_prefix(char *prefix,
                                                    struct Info *info) {
    size_t i = 1;
    int max_size = (int)ceil(log10((double)SIZE_MAX));
    char buffer[max_size + 2];
    while (true) {
        snprintf(buffer, max_size + 2, "%zu", i);
        char *new_prefix = ccn_str_cat(prefix, buffer);
        if (check_prefix_exists(info, new_prefix) == NULL) {
            mem_free(prefix);
            return new_prefix;
        }
        i++;
        if (i == 0) // Reached when overflowed.
            return NULL;
    }
}

static char *gen_prefix_from_string(const char *string, struct Info *info) {
    size_t len = strlen(string);
    assert(len > 0);
    size_t index = 0;
    bool push = false;

    char *chars = mem_alloc(len * sizeof(char));
    chars[0] = *string;
    index++;

    while (*string++) {
        if (push) {
            chars[index] = *string;
            index++;
        }
        push = false;

        if (isupper(*string)) {
            chars[index] = *string;
            index++;
        }

        if (*string == '_') {
            push = true;
        }
    }
    chars[index] = '\0';
    return gen_unique_prefix_from_existing_prefix(
        realloc(chars, (strlen(chars) + 1) * sizeof(char)), info);
}

static void *check_name_exists(struct Info *info, char *name) {
    Enum *enum_orig;
    Node *node_orig;
    Nodeset *nodeset_orig;
    Traversal *traversal_orig;
    Phase *phase_orig;
    Pass *pass_orig;

    if ((enum_orig = smap_retrieve(info->enum_name, name)) != NULL)
        return enum_orig->id;
    if ((node_orig = smap_retrieve(info->node_name, name)) != NULL)
        return node_orig->id;
    if ((nodeset_orig = smap_retrieve(info->nodeset_name, name)) != NULL)
        return nodeset_orig->id;
    if ((traversal_orig = smap_retrieve(info->traversal_name, name)) != NULL)
        return traversal_orig->id;
    if ((phase_orig = smap_retrieve(info->phase_name, name)) != NULL)
        return phase_orig->id;
    if ((pass_orig = smap_retrieve(info->pass_name, name)) != NULL)
        return pass_orig->id;
    return NULL;
}

static int check_enums(array *enums, struct Info *info) {

    int error = 0;

    for (int i = 0; i < array_size(enums); ++i) {
        Enum *cur_enum = (Enum *)array_get(enums, i);
        void *orig_def;

        // TODO check if name of enum overlaps with autogen enum.
        if ((orig_def = check_name_exists(info, cur_enum->id->orig)) != NULL) {
            print_error(cur_enum->id, "Redefinition of name '%s'",
                        cur_enum->id->orig);
            print_note(orig_def, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->enum_name, cur_enum->id->orig, cur_enum);
        }

        char *orig_prefix;
        // TODO check if prefix is not forbidden: NS_.
        if ((orig_prefix = smap_retrieve(info->enum_prefix,
                                         cur_enum->prefix->orig)) != NULL) {
            print_error(cur_enum->prefix, "Redefinition of prefix '%s'",
                        cur_enum->prefix->orig);
            print_note(orig_prefix, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->enum_prefix, cur_enum->prefix->orig,
                        cur_enum->prefix->orig);
        }
    }
    return error;
}

static int check_nodes(array *nodes, struct Info *info) {

    int error = 0;

    for (int i = 0; i < array_size(nodes); ++i) {
        Node *cur_node = (Node *)array_get(nodes, i);
        void *orig_def;

        if ((orig_def = check_name_exists(info, cur_node->id->orig))) {
            print_error(cur_node->id, "Redefinition of name '%s'",
                        cur_node->id->orig);
            print_note(orig_def, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->node_name, cur_node->id->orig, cur_node);
        }

        if (cur_node->root) {
            if (info->root_node != NULL) {
                orig_def = info->root_node->id;
                print_error(cur_node->id, "Duplicate declaration of root node");
                print_note(orig_def, "Previously declared here");
                error = 1;
            } else {
                info->root_node = cur_node;
            }
        }
    }
    return error;
}

static int check_nodesets(array *nodesets, struct Info *info) {

    int error = 0;

    for (int i = 0; i < array_size(nodesets); ++i) {
        Nodeset *cur_nodeset = (Nodeset *)array_get(nodesets, i);
        void *orig_def;

        if ((orig_def = check_name_exists(info, cur_nodeset->id->orig)) !=
            NULL) {
            print_error(cur_nodeset->id, "Redefinition of name '%s'",
                        cur_nodeset->id->orig);
            print_note(orig_def, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->nodeset_name, cur_nodeset->id->orig, cur_nodeset);
        }
    }
    return error;
}
static int check_actions_reference(array *actions, struct Info *info);
static Action *copy_action(Action *action) {
    Action *new = NULL;
    if (action->type == ACTION_REFERENCE) {
        new = create_action(action->type, strdup(action->action), action->id);
    } else {
        new = create_action(action->type, action->action, action->id);
    }
    new->checked = false;
    new->action_owner = false;
    return new;
}

static Phase *copy_phase_shallow(Phase *phase) {
    Phase *new =
        create_phase_header(id_cpy(phase->id), phase->start, phase->cycle);
    array *actions = create_array();
    for (int i = 0; i < array_size(phase->actions); ++i) {
        array_append(actions, copy_action(array_get(phase->actions, i)));
    }
    Id *root = NULL;
    if (phase->root != NULL) {
        root = id_cpy(phase->root);
    }
    new->common_info->hash = phase->common_info->hash;
    new->common_info->hash_matches = phase->common_info->hash_matches;
    new = create_phase(new, root, id_cpy(phase->prefix), actions,
                       phase->gate_func);
    ccn_set_free(new->roots);
    new->roots = ccn_set_copy(phase->roots);
    new->root_owner = false;
    new->original_ref = phase;
    return new;
}

Id *get_action_id(const Action *action) {
    switch (action->type) {
    case ACTION_PASS:
        return ((Pass *)action->action)->id;
    case ACTION_TRAVERSAL:
        return ((Traversal *)action->action)->id;
    case ACTION_PHASE:
        return ((Phase *)action->action)->id;
    case ACTION_REFERENCE:
        return action->id;
    }
}

static int check_action_reference(Action *action, struct Info *info) {
    Id *ref = (Id *)get_action_id(action);
    void *item = smap_retrieve(info->phase_name, ref->orig);
    if (item != NULL) {
        Phase *p = copy_phase_shallow(item);
        action->type = ACTION_PHASE;
        action->action = p;
        action->checked = true;
        check_actions_reference(p->actions, info);
        return 0;
    }

    item = smap_retrieve(info->traversal_name, ref->orig);
    if (item != NULL) {
        action->type = ACTION_TRAVERSAL;
        action->action = item;
        action->checked = true;
        return 0;
    }

    item = smap_retrieve(info->pass_name, ref->orig);
    if (item != NULL) {
        action->type = ACTION_PASS;
        action->action = item;
        action->checked = true;
        return 0;
    }

    print_error(action->action,
                "ID is not a reference to a defined pass, traversal or phase.");
    exit(1);
    // TODO !important propogate this error!
    return 1;
}

static int check_actions_reference(array *actions, struct Info *info) {
    int error = 0;
    for (int i = 0; i < array_size(actions); ++i) {
        Action *action = array_get(actions, i);
        if (action->type == ACTION_REFERENCE) {
            error = check_action_reference(action, info) == 0 ? error : 1;
        }
    }
    return error;
}

static int check_phases(array *phases, struct Info *info) {
    int error = 0;

    for (int i = 0; i < array_size(phases); ++i) {
        Phase *cur_phase = (Phase *)array_get(phases, i);
        void *orig_def;
        if ((orig_def = check_name_exists(info, cur_phase->id->orig)) != NULL) {
            print_error(cur_phase->id, "Redefinition of name '%s'",
                        cur_phase->id->orig);
            print_note(orig_def, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->phase_name, cur_phase->id->orig, cur_phase);
        }

        if (cur_phase->root != NULL) {
            Node *node_def =
                smap_retrieve(info->node_name, cur_phase->root->orig);
            if (node_def == NULL) {
                print_error(cur_phase->root, "Not a valid node.");
                error = 1;
            }
            bool has_child_next = false;
            for (int j = 0; j < array_size(node_def->children); ++j) {
                Child *child = array_get(node_def->children, j);
                if (strcmp(child->id->lwr, "next") == 0) {
                    has_child_next = true;
                    break;
                }
            }
            if (!has_child_next) {
                print_error(cur_phase->root,
                            "Specified root has no child named next.");
                error = 1;
            }
        }

        if (cur_phase->prefix != NULL) {
            if ((orig_def = check_prefix_exists(
                     info, cur_phase->prefix->orig)) != NULL) {
                print_error(cur_phase->prefix, "Redefinition of prefix '%s'",
                            cur_phase->prefix->orig);
                print_note(orig_def, "Previously declared here");
                error = 1;
            } else {
                smap_insert(info->action_prefix, cur_phase->prefix->orig,
                            cur_phase->prefix);
            }
        } else {
            char *pref = gen_prefix_from_string(cur_phase->id->orig, info);
            if (pref == NULL) {
                print_user_error("CCN", "Could not create prefix");
                exit(1);
            }
            cur_phase->prefix = create_id(pref);
            smap_insert(info->action_prefix, cur_phase->prefix->orig,
                        cur_phase->prefix);
        }
    }

    for (int i = 0; i < array_size(phases); ++i) {
        Phase *cur_phase = (Phase *)array_get(phases, i);
        for (int j = 0; j < array_size(cur_phase->actions); ++j) {
            Action *action = array_get(cur_phase->actions, j);
            if (action->type == ACTION_REFERENCE) {
                check_action_reference(action, info);
            }
        }
    }
    return error;
}

static int check_passes(array *passes, struct Info *info) {

    int error = 0;

    for (int i = 0; i < array_size(passes); ++i) {
        Pass *cur_pass = (Pass *)array_get(passes, i);
        void *orig_def;

        if ((orig_def = check_name_exists(info, cur_pass->id->orig)) != NULL) {
            print_error(cur_pass->id, "Redefinition of name '%s'",
                        cur_pass->id->orig);
            print_note(orig_def, "Previously declared here");
        } else {
            smap_insert(info->pass_name, cur_pass->id->orig, cur_pass);
        }
        if (cur_pass->prefix != NULL) {
            if ((orig_def = check_prefix_exists(
                     info, cur_pass->prefix->orig)) != NULL) {
                print_error(cur_pass->prefix, "Redefinition of prefix '%s'",
                            cur_pass->prefix->orig);
                print_note(orig_def, "Previously declared here");
                error = 1;
            } else {
                smap_insert(info->action_prefix, cur_pass->prefix->orig,
                            cur_pass->prefix);
            }
        } else {
            char *pref = gen_prefix_from_string(cur_pass->id->orig, info);
            if (pref == NULL) {
                print_user_error("CCN", "Could not create prefix");
                exit(1);
            }
            cur_pass->prefix = create_id(pref);
            smap_insert(info->action_prefix, cur_pass->prefix->orig,
                        cur_pass->prefix);
        }
    }
    return error;
}

static int check_traversals(array *traversals, struct Info *info) {

    int error = 0;

    for (int i = 0; i < array_size(traversals); ++i) {
        Traversal *cur_traversal = (Traversal *)array_get(traversals, i);
        void *orig_def;

        if ((orig_def = check_name_exists(info, cur_traversal->id->orig)) !=
            NULL) {
            print_error(cur_traversal->id, "Redefinition of name '%s'",
                        cur_traversal->id->orig);
            print_note(orig_def, "Previously declared here");
            error = 1;
        } else {
            smap_insert(info->traversal_name, cur_traversal->id->orig,
                        cur_traversal);
        }

        if (cur_traversal->prefix != NULL) {
            if ((orig_def = check_prefix_exists(
                     info, cur_traversal->prefix->orig)) != NULL) {
                print_error(cur_traversal->prefix,
                            "Redefinition of prefix '%s'",
                            cur_traversal->prefix->orig);
                print_note(orig_def, "Previously declared here");
                error = 1;
            } else {
                smap_insert(info->action_prefix, cur_traversal->prefix->orig,
                            cur_traversal->prefix);
            }
        } else {
            char *pref = gen_prefix_from_string(cur_traversal->id->orig, info);
            if (pref == NULL) {
                print_user_error("CCN", "Could not create prefix");
                exit(1);
            }
            cur_traversal->prefix = create_id(pref);
            smap_insert(info->action_prefix, cur_traversal->prefix->orig,
                        cur_traversal->prefix);
        }
    }
    return error;
}

static int check_node(Node *node, struct Info *info) {
    int error = 0;

    smap_t *child_name = smap_init(16);

    if (node->children) {
        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            Child *orig_child;

            // Check if there is no duplicate naming.
            if ((orig_child = smap_retrieve(child_name, child->id->orig)) !=
                NULL) {
                print_error(child->id,
                            "Duplicate name '%s' in children of node '%s'",
                            child->id->orig, node->id->orig);
                print_note(orig_child->id, "Previously declared here");
                error = 1;
            } else {
                smap_insert(child_name, child->id->orig, child);
            }

            Node *child_node =
                (Node *)smap_retrieve(info->node_name, child->type->orig);
            Nodeset *child_nodeset =
                (Nodeset *)smap_retrieve(info->nodeset_name, child->type->orig);

            if (!child_node && !child_nodeset) {
                print_error(child->type,
                            "Unknown type '%s' of child '%s' of node '%s'",
                            child->type->orig, child->id->orig, node->id->orig);
                error = 1;
            } else {
                child->node = child_node;
                child->nodeset = child_nodeset;
            }

            if (child_node && child_node == info->root_node) {
                print_error(
                    child->id,
                    "Child '%s' of node '%s' cannot use root node as type",
                    child->id->orig, node->id->orig);
                error = 1;
            }
        }
    }

    smap_t *attr_name = smap_init(16);

    if (node->attrs) {
        for (int i = 0; i < array_size(node->attrs); i++) {
            Attr *attr = (Attr *)array_get(node->attrs, i);
            Attr *orig_attr;

            if ((orig_attr = smap_retrieve(attr_name, attr->id->orig)) !=
                NULL) {
                print_error(attr->id,
                            "Duplicate name '%s' in atributes of node '%s'",
                            attr->id->orig, node->id->orig);
                print_note(orig_attr->id, "Previously declared here");
                error = 1;
            } else {
                smap_insert(attr_name, attr->id->orig, attr);
            }

            if (attr->type == AT_link_or_enum) {
                Node *attr_node =
                    (Node *)smap_retrieve(info->node_name, attr->type_id->orig);
                Enum *attr_enum =
                    (Enum *)smap_retrieve(info->enum_name, attr->type_id->orig);

                if (attr_node) {
                    attr->type = AT_link;
                } else if (attr_enum) {
                    attr->type = AT_enum;
                } else {
                    print_error(
                        attr->type_id,
                        "Unknown type '%s' of attribute '%s' of node '%s'",
                        attr->type_id->orig, attr->id->orig, node->id->orig);
                    error = 1;
                }
            }
        }
    }

    smap_free(child_name);
    smap_free(attr_name);

    return error;
}

static int check_nodeset(Nodeset *nodeset, struct Info *info) {
    int error = 0;

    smap_t *node_name = smap_init(16);

    for (int i = 0; i < array_size(nodeset->nodes); ++i) {
        Node *node = (Node *)array_get(nodeset->nodes, i);
        Node *orig_node = smap_retrieve(node_name, node->id->orig);

        // Check if there is no duplicate naming.
        if (orig_node != NULL) {
            print_error(node, "Duplicate name '%s' in nodes of nodeset '%s'",
                        node, nodeset->id->orig);
            print_note(orig_node, "Previously declared here");
            error = 1;
        } else {
            smap_insert(node_name, node->id->orig, node);
        }

        Node *nodeset_node =
            (Node *)smap_retrieve(info->node_name, node->id->orig);

        Nodeset *nodeset_nodeset =
            (Nodeset *)smap_retrieve(info->nodeset_name, node->id->orig);

        if (nodeset_nodeset) {
            print_error(node, "Nodeset '%s' contains other nodeset '%s'",
                        nodeset->id->orig, nodeset_nodeset->id->orig);
            error = 1;
        } else if (!nodeset_node) {
            print_error(node, "Unknown type of node '%s' in nodeset '%s'",
                        node->id->orig, nodeset->id->orig);
            error = 1;
        } else {
            /// TODO: Something with cleaning the array? probably happens
            /// somewhere else
            // mem_free(node);
            // array_set(nodeset->nodes, i, nodeset_node);
        }
    }

    smap_free(node_name);

    return error;
}

static int check_enum(Enum *arg_enum, struct Info *info) {

    int error = 0;
    smap_t *value_name = smap_init(16);

    for (int i = 0; i < array_size(arg_enum->values); ++i) {
        Id *cur_value = (Id *)array_get(arg_enum->values, i);
        Id *orig_value = smap_retrieve(value_name, cur_value->orig);

        // Check if there is no duplicate naming.
        if (orig_value != NULL) {
            print_error(cur_value, "Duplicate name '%s' in values of enum '%s'",
                        cur_value, arg_enum->id->orig);
            print_note(orig_value, "Previously declared here");

            error = 1;
        } else {
            smap_insert(value_name, cur_value->orig, cur_value);
        }
    }

    smap_free(value_name);
    return error;
}

static int check_traversal(Traversal *traversal, struct Info *info) {

    // TODO: check collission of func

    int error = 0;

    if (traversal->nodes == NULL)
        return 0;

    smap_t *node_name = smap_init(16);
    smap_t *node_name_expanded = smap_init(16);

    array *nodes_expanded = array_init(16);

    for (int i = 0; i < array_size(traversal->nodes); ++i) {
        Node *node = (Node *)array_get(traversal->nodes, i);
        Node *orig_node = smap_retrieve(node_name, node->id->orig);

        // Check if there is no duplicate naming.
        if (orig_node != NULL) {
            print_error(node, "Duplicate name '%s' in nodes of traversal '%s'",
                        node->id->orig, traversal->id->orig);
            print_note(orig_node, "Previously declared here");
            error = 1;
        } else {
            smap_insert(node_name, node->id->orig, node);
        }

        Node *traversal_node =
            (Node *)smap_retrieve(info->node_name, node->id->orig);
        Nodeset *traversal_nodeset =
            (Nodeset *)smap_retrieve(info->nodeset_name, node->id->orig);

        if (traversal_node) {
            array_append(nodes_expanded, strdup(node->id->orig));
            smap_insert(node_name_expanded, node->id->orig, node);
        } else if (traversal_nodeset) {

            // Adds every node in the nodeset to the expanded node list
	    // This makes sure nodesets in traversals are expended to their actual values.
            for (int j = 0; j < array_size(traversal_nodeset->nodes); j++) {
                Node *nodeset_node = array_get(traversal_nodeset->nodes, j);
                if (smap_retrieve(node_name_expanded, nodeset_node->id->orig) ==
                    NULL) {
                    array_append(nodes_expanded,
                                 strdup(nodeset_node->id->orig));
                    smap_insert(node_name_expanded, nodeset_node->id->orig,
                                nodeset_node);
                }
            }
        } else {
            print_error(
                node, "Unknown type of node or nodeset '%s' in traversal '%s'",
                node->id->orig, traversal->id->orig);
            error = 1;
        }
    }

    // We clean the old array here and replace it with the expanded nodes. 
    array_cleanup(traversal->nodes, NULL);
    traversal->nodes = nodes_expanded;

    smap_t *td_name = smap_init(16);

    if (traversal->data) {
        for (int i = 0; i < array_size(traversal->data); i++) {
            Attr *td = (Attr *)array_get(traversal->data, i);
            Attr *orig_td;

            if ((orig_td = smap_retrieve(td_name, td->id->orig)) != NULL) {
                print_error(
                    td->id,
                    "Duplicate name '%s' in atributes of traversal '%s'",
                    td->id->orig, traversal->id->orig);
                print_note(orig_td->id, "Previously declared here");
                error = 1;
            } else {
                smap_insert(td_name, td->id->orig, td);
            }
        }
    }


    smap_free(node_name);
    smap_free(node_name_expanded);

    return error;
}

static int check_pass(Pass *pass, struct Info *info) {

    int error = 0;
    if (pass->func != NULL) {
        if (ccn_set_contains(info->functions, pass->func)) {
            error = 1;
            print_error(pass, "Function already used in another pass.");
        } else {
            ccn_set_insert(info->functions, pass->func->orig);
        }
    }
    return error;
}

static inline void add_required_root_to_phase(Phase *phase, char *root) {
    if (!phase->root_owner) {
        ccn_set_insert(phase->original_ref->roots, ccn_str_cpy(root));
    } else {
        ccn_set_insert(phase->roots, ccn_str_cpy(root));
    }
}

static int check_phase(Phase *phase, struct Info *info, smap_t *phase_order) {
    int error = 0;

    if (phase->start) {
        if (info->root_phase !=
            NULL) { // TODO: rename root_phase to start_phase in info struct.
            print_error(phase, "Double declaration of start phase");
            print_note(info->root_phase, "Previously declared here");
            error = 1;
        } else {
            info->root_phase = phase;
            info->config->start_phase = phase;
        }
    }

    smap_insert(phase_order, phase->id->orig, phase);

    return error;
}

void print_cyclic_error(array *vals, SetExpr *to_add) {
    print_error(to_add, "Element creates a cyclic dependency.");
    print_note_no_loc("Dependency chain:");
    for (int i = 0; i < array_size(vals); ++i) {
        char *val = array_get(vals, i);
        printf("%s->", val);
    }
    printf("%s\n", to_add->ref_id->orig);
}

bool is_cyclic_dependency(array *vals, char *new_to_add) {
    for (int i = 0; i < array_size(vals); ++i) {
        char *val = array_get(vals, i);
        if (ccn_str_equal(val, new_to_add)) {
            return true;
        }
    }
    return false;
}

static void evaluate_set_expr(SetExpr *expr, struct Info *info, int *error,
                              array *merged_sets, bool report) {
    ccn_set_t *new_set = NULL;
    if (expr->type == SET_REFERENCE) {
        if (is_cyclic_dependency(merged_sets, expr->ref_id->orig)) {
            (*error)++;
            if (report) {
                print_cyclic_error(merged_sets, expr);
            }
            return;
        } else {
            array_append(merged_sets, expr->ref_id->orig);
        }
        Nodeset *target = smap_retrieve(info->nodeset_name, expr->ref_id->orig);
        if (target == NULL) {
            exit(EXIT_FAILURE);
        }
        evaluate_set_expr(target->expr, info, error, merged_sets, report);
        if (*error) {
            return;
        }
        array_pop(merged_sets);
        assert(target->expr->type == SET_LITERAL);

        new_set = ccn_set_copy(target->expr->id_set);

        mem_free(expr->ref_id->orig);
        expr->type = SET_LITERAL;
        expr->id_set = new_set;
    } else if (expr->type == SET_OPERATION) {
        evaluate_set_expr(expr->operation->left_child, info, error, merged_sets,
                          report);
        evaluate_set_expr(expr->operation->right_child, info, error,
                          merged_sets, report);
        if (*error) {
            return;
        }
        ccn_set_t *left = expr->operation->left_child->id_set;
        ccn_set_t *right = expr->operation->right_child->id_set;

        switch (expr->operation->operator) {
        case SET_UNION:
            new_set = ccn_set_union(left, right);
            break;
        case SET_INTERSECT:
            new_set = ccn_set_intersect(left, right);
            break;
        case SET_DIFFERENCE:
            new_set = ccn_set_difference(left, right);
            break;
        default: // TODO throw unsupported error.
            break;
        }
        free_setOperation(expr->operation);
        expr->type = SET_LITERAL;
        expr->id_set = new_set;
    } else if (expr->type == SET_LITERAL) {
        assert(expr->id_set != NULL);
    }
}

static int evaluate_nodesets_expr(array *nodesets, struct Info *info) {
    int error = 0;
    array *merged_sets = create_array();
    for (int i = 0; i < array_size(nodesets); ++i) {
        Nodeset *nodeset = (Nodeset *)array_get(nodesets, i);
        array_append(merged_sets, nodeset->id);
        evaluate_set_expr(nodeset->expr, info, &error, merged_sets, true);
        if (error) {
            goto cleanup;
        }
        array_clear(merged_sets);
    }

cleanup:
    array_cleanup(merged_sets, NULL);
    return error;
}

static int evaluate_traversals_expr(array *traversals, struct Info *info) {
    int error = 0;
    array *merged_sets = create_array();
    for (int i = 0; i < array_size(traversals); i++) {
        Traversal *trav = (Traversal *)array_get(traversals, i);
        if (trav->expr == NULL)
            continue;
        evaluate_set_expr(trav->expr, info, &error, merged_sets, false);

        if (error) {
            goto cleanup;
        }

        array_clear(merged_sets);
    }

cleanup:
    array_cleanup(merged_sets, NULL);
    return error;
}

// TODO: move this??
static array *set_to_array(ccn_set_t *set, struct Info *info) {
    assert(set != NULL);
    array *names = smap_values(set->hash_map);
    array *nodes = array_init(1);
    for (int i = 0; i < array_size(names); ++i) {
        char *name = array_get(names, i);
        Node *node = smap_retrieve(info->node_name, name);
        array_append(nodes, node);
    }
    smap_free(set->hash_map);
    mem_free(set);
    // array_cleanup(names, NULL);
    return nodes;
}

static int nodesets_expr_to_array(array *nodesets, struct Info *info) {
    int error = 0;

    for (int i = 0; i < array_size(nodesets); ++i) {
        Nodeset *nodeset = (Nodeset *)array_get(nodesets, i);
        if (nodeset->expr->type != SET_LITERAL) {
            continue;
        }
        array *nodes = set_to_array(nodeset->expr->id_set, info);
        nodeset->expr->id_set = NULL;
        free_setExpr(nodeset->expr);
        nodeset->nodes = nodes;
    }

    return error;
}

static int traversals_expr_to_array(array *traversals, struct Info *info) {
    int error = 0;

    for (int i = 0; i < array_size(traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(traversals, i);
        array *nodes = NULL;
        if (trav->expr == NULL) {
            nodes = info->config->nodes;
        } else {
            nodes = set_to_array(trav->expr->id_set, info);
            trav->expr->id_set = NULL;
            free_setExpr(trav->expr);
        }
        trav->nodes = nodes;
    }

    return error;
}

/* This makes sure inline defined actions are also taken into account. */
static void unwrap_all_actions(array *phases, struct Info *info) {
    array *new_phases = create_array();
    for (int i = 0; i < array_size(phases); ++i) {
        Phase *phase = array_get(phases, i);
        for (int j = 0; j < array_size(phase->actions); ++j) {
            Action *action = array_get(phase->actions, j);
            switch (action->type) {
            case ACTION_PASS:
                if (phase->root != NULL) {
                    Pass *pass = action->action;
                    ccn_set_insert(pass->roots, ccn_str_cpy(phase->root->orig));
                }
                array_append(info->config->passes, (Pass *)action->action);
                break;
            case ACTION_TRAVERSAL:
                array_append(info->config->traversals,
                             (Traversal *)action->action);
                break;
            case ACTION_PHASE:
                array_append(new_phases, (Phase *)action->action);
                if (phase->root != NULL) {
                    add_required_root_to_phase(action->action,
                                               phase->root->orig);
                }
                break;
            default:
                break;
            }
        }
    }
    if (array_size(new_phases) > 0) {
        unwrap_all_actions(new_phases, info);
        for (int i = 0; i < array_size(new_phases); ++i) {
            array_append(info->config->phases, array_get(new_phases, i));
        }
    }
    array_clear(new_phases);
    array_cleanup(new_phases, NULL);
}

bool name_is_action(char *name, struct Info *info) {
    if (smap_retrieve(info->pass_name, name) != NULL)
        return true;
    if (smap_retrieve(info->phase_name, name) != NULL)
        return true;
    if (smap_retrieve(info->traversal_name, name) != NULL)
        return true;
    if (smap_retrieve(info->action_prefix, name) != NULL)
        return true;
    return false;
}

static char *get_current_namespace(Range_spec_t *spec) {
    return array_get(spec->ids, spec->id_index);
}

static bool is_last_namespace(Range_spec_t *spec) {
    return array_size(spec->ids) - 1 == spec->id_index;
}

int insert_active_spec(smap_t *active_specs, Range_spec_t *curr_spec) {
    Range_spec_t *old = smap_retrieve(active_specs, curr_spec->consistency_key);
    if (old == NULL) {
        smap_insert(active_specs, curr_spec->consistency_key, curr_spec);
        return 0;
    }
    if (old->life_type != curr_spec->life_type) {
        print_error(curr_spec->owner,
                    "Lifetime conflicts with already present lifetime.");
        print_note(old->owner, "Lifetime that is being conflicted with.");
        return 1;
    }
    return 0;
}

static void lifetime_phase_set_specs(Phase *phase, Range_spec_t *spec,
                                     int *error);

// TODO add lifetime range spec to passes.
static void action_add_lifetime_spec(Action *action, Range_spec_t *curr_spec,
                                     int *error) {
    switch (action->type) {
    case ACTION_TRAVERSAL:
        *error = insert_active_spec(action->active_specs, curr_spec) == 0
                     ? *error
                     : 1;
        break;
    case ACTION_PASS:
        *error = insert_active_spec(action->active_specs, curr_spec) == 0
                     ? *error
                     : 1;
        break;
    case ACTION_PHASE:
        *error = insert_active_spec(((Phase *)action->action)->active_specs,
                                    curr_spec) == 0
                     ? *error
                     : 1;
        if (error)
            break;
        lifetime_phase_set_specs(action->action, curr_spec, error);
        break;
    default:
        break;
    }
}

static void lifetime_phase_set_specs(Phase *phase, Range_spec_t *spec,
                                     int *error) {
    for (int i = 0; i < array_size(phase->actions); ++i) {
        if (error)
            return;
        Action *action = array_get(phase->actions, i);
        action_add_lifetime_spec(action, spec, error);
    }
}

uint32_t get_last_action_id(Phase *phase) {
    Action *action = array_get(phase->actions, array_size(phase->actions) - 1);
    return action->id_counter;
}

void last_action_found(Action *action, Range_spec_t *spec, bool active) {
    if (action->type == ACTION_PHASE) {
        if ((active && spec->inclusive) || (!active && !spec->inclusive)) {
            spec->action_counter_id = get_last_action_id(action->action);
        } else {
            spec->action_counter_id = action->id_counter;
        }
    } else {
        spec->action_counter_id = action->id_counter;
    }
}

char *get_action_prefix(const Action *action) {
    switch (action->type) {
    case ACTION_PASS:
        return ((Pass *)action->action)->prefix->orig;
    case ACTION_TRAVERSAL:
        return ((Traversal *)action->action)->prefix->orig;
    case ACTION_PHASE:
        return ((Phase *)action->action)->prefix->orig;
    default:
        return NULL;
    }
}

bool find_lifetime_spec(Lifetime_t *lifetime, struct Info *info, Phase *phase,
                        bool active, bool *found) {
    Range_spec_t *spec = NULL;
    if (active)
        spec = lifetime->end;
    else
        spec = lifetime->start;

    for (int i = 0; i < array_size(phase->actions); ++i) {
        Action *action = array_get(phase->actions, i);
        if (ccn_str_equal(action->id->orig, get_current_namespace(spec)) ||
            ccn_str_equal(get_action_prefix(action),
                          get_current_namespace(spec))) {
            if (spec->inclusive) {
                int error = 0;
                action_add_lifetime_spec(action, spec, &error);
                if (error) // TODO handle this better.
                    return false;
            }
            if (is_last_namespace(spec)) {
                last_action_found(action, spec, active);
                *found = true;
            } else {
                if (action->type == ACTION_PHASE) {
                    spec->id_index++;
                    find_lifetime_spec(lifetime, info, action->action, active,
                                       found); // Must be found in this phase.
                }
            }
            return false; // Either last namespace found or checked in
                          // namespace action, so never continue.
        }
        if (action->type == ACTION_PHASE) {
            bool cont = find_lifetime_spec(lifetime, info, action->action,
                                           active, found);
            if (!cont)
                return false;
        }
    }

    return true;
}

bool check_lifetime_spec_root(Lifetime_t *lifetime, struct Info *info,
                              bool active, Phase *root) {
    Range_spec_t *spec = NULL;
    bool found = false;
    if (active)
        spec = lifetime->end;
    else
        spec = lifetime->start;

    if (ccn_str_equal(root->id->orig, get_current_namespace(spec)) ||
        ccn_str_equal(root->prefix->orig, get_current_namespace(spec))) {
        if (spec->inclusive) {
            int error = 0;
            error =
                insert_active_spec(root->active_specs, spec) == 0 ? error : 1;
            lifetime_phase_set_specs(root, spec, &error);
            if (error) // TODO: Handle better.
                return false;
        }
        if (is_last_namespace(spec)) {
            if ((active && spec->inclusive) || (!active && !spec->inclusive)) {
                spec->action_counter_id = get_last_action_id(root);
            } else {
                spec->action_counter_id =
                    1; // 1 is the id for the root phase, special
                       // as its not really an action.
            }
            return true;
        }
        spec->id_index++;
    }
    find_lifetime_spec(lifetime, info, root, active, &found);
    return found;
}

// TODO: possible rewrite.
static int check_lifetime_reach(Lifetime_t *lifetime, struct Info *info) {
    // TODO: refactor this into a function, not DRY!
    if (lifetime->start == NULL) {
        array *ids = array_init(1);
        array_append(ids, strdup(info->root_phase->id->orig));
        lifetime->owner = true;
        lifetime->start = create_range_spec(true, ids);
        lifetime->start->consistency_key = strdup(lifetime->key);
        lifetime->start->life_type = lifetime->type;
        if (lifetime->type == LIFETIME_DISALLOWED) {
            lifetime->start->type = ccn_str_cpy("CCN_CHK_DISALLOWED");
        } else {
            lifetime->start->type = ccn_str_cpy("CCN_CHK_MANDATORY");
        }
        lifetime->start->owner = lifetime;
    }

    if (lifetime->end == NULL) {
        array *ids = array_init(1);
        array_append(ids, strdup(info->root_phase->id->orig));
        lifetime->owner = true;
        lifetime->end = create_range_spec(true, ids);
        lifetime->end->life_type = lifetime->type;
        lifetime->end->consistency_key = strdup(lifetime->key);
        if (lifetime->type == LIFETIME_DISALLOWED) {
            lifetime->end->type = ccn_str_cpy("CCN_CHK_DISALLOWED");
        } else {
            lifetime->end->type = ccn_str_cpy("CCN_CHK_MANDATORY");
        }
        lifetime->end->owner = lifetime;
    }

    if (lifetime->values != NULL) {
        lifetime->start->values = lifetime->values;
        lifetime->end->values = lifetime->values;
    }

    lifetime->start->push = true;
    lifetime->end->push = false;

    Range_spec_t *curr_spec = lifetime->start;
    Phase *root_phase = info->root_phase;
    if (lifetime->start != NULL && !lifetime->start->inclusive &&
        (ccn_str_equal(get_current_namespace(curr_spec),
                       root_phase->id->orig) ||
         ccn_str_equal(get_current_namespace(curr_spec),
                       root_phase->prefix->orig))) {

        if (is_last_namespace(curr_spec)) {
            print_error(curr_spec,
                        "Exclusive over the root phase will never be reached, "
                        "because nothing comes after the root phase.");
            print_note(curr_spec,
                       "Maybe, you meant to use the \'[\' operator.");
            return 1;
        }
    }

    bool found = check_lifetime_spec_root(lifetime, info, false, root_phase);
    if (!found) {
        print_error(lifetime->start,
                    "Specification could not be reached. Recheck your ranges.");
        return 1;
    }

    found = check_lifetime_spec_root(lifetime, info, true, root_phase);
    if (!found) {
        print_error(lifetime->end,
                    "Specification could not be reached. Recheck your ranges.");
        return 1;
    }
    if (lifetime->end->action_counter_id < lifetime->start->action_counter_id) {
        print_error(
            lifetime->end,
            "Specification could not be reached from specified starting "
            "action. Recheck your ranges.");
        return 1;
    }
    return 0;
}

static int check_lifetime(Lifetime_t *lifetime, struct Info *info) {
    int error = 0;

    if (lifetime->start != NULL &&
        !name_is_action(get_current_namespace(lifetime->start), info)) {
        print_error(lifetime->start,
                    "Id is not a reference to a valid action.");
        error++;
    }

    if (lifetime->end != NULL &&
        !name_is_action(get_current_namespace(lifetime->end), info)) {
        print_error(lifetime->end, "Id is not a reference to a valid action.");
        error++;
    }

    if (!error)
        error += check_lifetime_reach(lifetime, info);

    return error;
}

static int check_lifetimes_array(array *lifetimes, struct Info *info) {
    int error = 0;
    if (lifetimes == NULL)
        return 0;

    for (int i = 0; i < array_size(lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(lifetimes, i);
        error = check_lifetime(lifetime, info) == 0 ? error : 1;
    }
    return error;
}

static Enum *find_enum(array *enums, char *id) {
    for (int i = 0; i < array_size(enums); ++i) {
        Enum *e = array_get(enums, i);
        if (ccn_str_equal(e->id->orig, id))
            return e;
    }

    return NULL;
}

static int check_lifetimes_attribute_values(Attr *attr, struct Info *info) {
    bool found = false;
    bool error = false;

    for (int i = 0; i < array_size(attr->lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(attr->lifetimes, i);
        Enum *e = find_enum(info->config->enums, attr->type_id->orig);

        if (e == NULL)
            continue;

        for (int j = 0; j < array_size(lifetime->values); ++j) {
            char *id = array_get(lifetime->values, j);
            found = false;

            for (int k = 0; k < array_size(e->values); k++) {
                char *val = array_get(e->values, k);
                if (ccn_str_equal(val, id)) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                error = true;
                print_error(id,
                            "Could not find this value in the values of the "
                            "corresponding attribute.");
                print_note(e, "The corresponding attribute can be found here.");
            }
        }
    }

    return error ? 1 : 0;
}

static int check_lifetimes(struct Info *info) {
    array *nodes = info->config->nodes;
    int error = 0;
    for (int i = 0; i < array_size(nodes); ++i) {
        Node *node = array_get(nodes, i);
        error = check_lifetimes_array(node->lifetimes, info) == 0 ? error : 1;

        for (int j = 0; j < array_size(node->children); ++j) {
            Child *child = array_get(node->children, j);
            error += check_lifetimes_array(child->lifetimes, info);
        }

        for (int j = 0; j < array_size(node->attrs); ++j) {
            Attr *attr = array_get(node->attrs, j);

            if (attr->type != AT_enum) {
                if (attr->lifetimes != NULL) {
                    print_error(attr,
                                "This attribute is not allowed to have "
                                "lifetimes. Lifetimes are "
                                "only possible on enum or pointer attributes.");
                    error += 1;
                }
            } else {
                error += check_lifetimes_attribute_values(attr, info);
                error += check_lifetimes_array(attr->lifetimes, info);
            }
        }
    }

    return error;
}

/* The user can leave range open in the lifetimes, here we fill them up,
 * this allows to process all lifetimes the same. We also populate the
 * type in string form, as this is required for code generation. The string
 * form corresponds to the enum value that is generated.
 */
static void fill_lifetime(Lifetime_t *lifetime, char *key) {
    if (lifetime->type == LIFETIME_DISALLOWED) {
        if (lifetime->start != NULL) {
            lifetime->start->type = ccn_str_cpy("CCN_CHK_DISALLOWED");
        }
        if (lifetime->end != NULL) {
            lifetime->end->type = ccn_str_cpy("CCN_CHK_DISALLOWED");
        }
    } else if (lifetime->type == LIFETIME_MANDATORY) {
        if (lifetime->start != NULL) {
            lifetime->start->type = "CCN_CHK_MANDATORY";
        }
        if (lifetime->end != NULL) {
            lifetime->end->type = "CCN_CHK_MANDATORY";
        }
    }

    if (lifetime->start != NULL) {
        lifetime->start->consistency_key = strdup(key);
        lifetime->start->life_type = lifetime->type;
    }
    if (lifetime->end != NULL) {
        lifetime->end->life_type = lifetime->type;
        lifetime->end->consistency_key = strdup(key);
    }
}

static void create_lifetime_func_attrs(Attr *attr, char *node_id,
                                       struct Info *info) {
    for (int i = 0; i < array_size(attr->lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(attr->lifetimes, i);
        char *key = ccn_str_cat(node_id, attr->id->orig);
        lifetime->key = key; // Move ownership.
        fill_lifetime(lifetime, key);
    }
}

static void create_lifetime_func_child(Child *child, char *node_id) {
    for (int i = 0; i < array_size(child->lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(child->lifetimes, i);
        char *key = ccn_str_cat(node_id, child->id->orig);
        lifetime->key = key; // Move ownership.
        fill_lifetime(lifetime, key);
    }
}

static void create_lifetime_func(Node *node, struct Info *info) {
    for (int i = 0; i < array_size(node->lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(node->lifetimes, i);
        fill_lifetime(lifetime, node->id->orig);
        lifetime->key = ccn_str_cpy(node->id->orig);
    }

    for (int i = 0; i < array_size(node->children); ++i) {
        create_lifetime_func_child(array_get(node->children, i),
                                   node->id->orig);
    }

    for (int i = 0; i < array_size(node->attrs); ++i) {
        create_lifetime_func_attrs(array_get(node->attrs, i), node->id->orig,
                                   info);
    }
}

static void create_lifetime_funcs(struct Info *info) {
    for (int i = 0; i < array_size(info->config->nodes); ++i) {
        Node *node = array_get(info->config->nodes, i);
        create_lifetime_func(node, info);
    }
}

// TODO handle actions that are not being used.
uint32_t assign_id_to_phase_actions(Phase *phase, uint32_t id) {
    uint32_t val = id;
    for (int i = 0; i < array_size(phase->actions); ++i) {
        Action *act = array_get(phase->actions, i);
        act->id_counter = val;
        val++;
        if (act->type == ACTION_PHASE)
            val = assign_id_to_phase_actions(act->action, val);
    }
    return val;
}

void assign_id_to_action(struct Info *info) {
    Phase *root = info->root_phase;
    uint32_t id =
        2; // 0 is for undefined, 1 is for the root-phase, so start at 2.
    assign_id_to_phase_actions(root, id);
}

Lifetime_t *copy_lifetime(Lifetime_t *lifetime, char *key) {
    Lifetime_t *new =
        create_lifetime(lifetime->start, lifetime->end, lifetime->type, NULL);
    new->key = ccn_str_cpy(key);
    new->owner = false;
    return new;
}

void unpack_lifetime_value(array *new_lifetimes, Lifetime_t *lifetime) {
    for (int i = 0; i < array_size(lifetime->values) - 1; ++i) {
        char *val = array_get(lifetime->values, i);
        char *key = ccn_str_cat(lifetime->key, val);
        Lifetime_t *new = copy_lifetime(lifetime, key);
        new->original_value = ccn_str_cpy(val);
        mem_free(key);
        array_append(new_lifetimes, new);
    }
    char *val = array_get(lifetime->values, array_size(lifetime->values) - 1);
    char *key = ccn_str_cat(lifetime->key, val);
    lifetime->original_value = ccn_str_cpy(val);
    array_cleanup(lifetime->values, mem_free);
    lifetime->values = NULL;
    mem_free(lifetime->key);
    lifetime->key = key;
}

/* This maps a lifetime with multiple values in a list of lifetimes.
 * This allows to check conflicting lifetimes easier.
 */
void unpack_lifetime_values(struct Info *info, Attr *attr) {
    array *new_lifetimes = create_array();
    for (int i = 0; i < array_size(attr->lifetimes); ++i) {
        Lifetime_t *lifetime = array_get(attr->lifetimes, i);
        if (array_size(lifetime->values) > 0) {
            unpack_lifetime_value(new_lifetimes, lifetime);
        }
    }
    for (int i = 0; i < array_size(new_lifetimes); ++i) {
        Lifetime_t *new = array_get(new_lifetimes, i);
        array_append(attr->lifetimes, new);
    }
    array_clear(new_lifetimes);
    array_cleanup(new_lifetimes, NULL);
}

void unpack_lifetime_attrb_values(struct Info *info) {
    for (int i = 0; i < array_size(info->config->nodes); ++i) {
        Node *n = array_get(info->config->nodes, i);
        for (int j = 0; j < array_size(n->attrs); ++j) {
            Attr *attr = array_get(n->attrs, j);
            unpack_lifetime_values(info, attr);
        }
    }
}

int check_config(Config *config) {

    // Great name...
    int success = 0;
    struct Info *info = create_info(config);
    smap_t *phase_order = smap_init(16);
    Phase *cur_phase;
    bool start_phase_seen = false;
    bool phase_errors = false;

    unwrap_all_actions(config->phases, info);

    success += check_nodes(config->nodes, info);
    success += check_nodesets(config->nodesets, info);
    success += check_enums(config->enums, info);

    if (info->root_node == NULL) {
        print_error_no_loc("No root node specified.");
        success++;
    } else {
        config->root_node = info->root_node;
    }

    success += check_traversals(config->traversals, info);
    success += check_passes(config->passes, info);
    success += check_phases(config->phases, info);

    // subtree_generate_traversals(config);

    // Transform setExpr to array of node ptrs.
    success += evaluate_nodesets_expr(config->nodesets, info);
    success += evaluate_traversals_expr(config->traversals, info);

    if (success) {
        return success;
    }

    // Transform all nodeset expressions to arrays.
    success += nodesets_expr_to_array(config->nodesets, info);
    success += traversals_expr_to_array(config->traversals, info);

    // From here all SetExprs for nodes are transformed into arrays.

    for (int i = 0; i < array_size(config->nodes); ++i) {
        success += check_node(array_get(config->nodes, i), info);
    }

    for (int i = 0; i < array_size(config->nodesets); ++i) {
        success += check_nodeset(array_get(config->nodesets, i), info);
    }

    for (int i = 0; i < array_size(config->enums); ++i) {
        success += check_enum(array_get(config->enums, i), info);
    }

    for (int i = 0; i < array_size(config->traversals); ++i) {
        success += check_traversal(array_get(config->traversals, i), info);
    }
    for (int i = 0; i < array_size(config->passes); ++i) {
        success += check_pass(array_get(config->passes, i), info);
    }

    for (int i = 0; i < array_size(config->phases); ++i) {
        cur_phase = array_get(config->phases, i);

        if (cur_phase->start)
            start_phase_seen = true;

        int res = check_phase(cur_phase, info, phase_order);
        success += res;
        if (res)
            phase_errors = true;
    }

    if (phase_errors) {
        success++;
    }

    if (!start_phase_seen) {
        print_error_no_loc("No start phase specified.");
        success++;
    }

    // Lifetime setup and processing.
    assign_id_to_action(info);
    create_lifetime_funcs(info);
    unpack_lifetime_attrb_values(info);
    success += check_lifetimes(info);

    smap_free(phase_order);
    free_info(info);

    return success;
}
