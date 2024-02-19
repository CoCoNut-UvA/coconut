#include <assert.h>
#include "dynamic_backend/collect_children.h"
#include "palm/memory.h"
#include "palm/str.h"

void destroy_child_list(struct child_list *list) {
    struct child_list *entry, *next;
    for (entry = list; entry; entry = next) {
        next = entry->next;
        MEMfree(entry);
    }
}

static inline node_st *get_child(node_st *child_name, node_st *node) {
    for (node_st *candidate = INODE_ICHILDREN(node); candidate;
         candidate = CHILD_NEXT(candidate)) {
        if (STReq(ID_LWR(CHILD_NAME(candidate)), ID_LWR(child_name))) {
            return candidate;
        }
    }

    return NULL;
}

struct child_list *collect_children_equation_args(htable_st *htable,
                                                  node_st *node,
                                                  node_st *equation) {
    struct child_list *children = NULL;
    struct child_list *tail = NULL;
    for (node_st *arg = EQUATION_IARGS(equation); arg;
         arg = EQUATION_DEPENDENCY_NEXT(arg)) {
        node_st *attribute = EQUATION_DEPENDENCY_IATTRIBUTE(arg);
        if (ATTRIBUTE_REFERENCE_INODE(attribute) == NULL) { // not a child
            continue;
        }

        node_st *child = get_child(ATTRIBUTE_REFERENCE_INODE(attribute), node);
        assert(child != NULL);
        if (!CHILD_IS_MANDATORY(child) && !HTlookup(htable, child)) {
            HTinsert(htable, child, (void *)CH_FALSE);
            struct child_list *child_entry =
                MEMmalloc(sizeof(struct child_list));
            child_entry->child = child;
            child_entry->next = NULL;
            if (tail != NULL) {
                tail->next = child_entry;
                tail = child_entry;
            } else {
                children = child_entry;
                tail = child_entry;
            }
        }
    }

    return children;
}
