#include <assert.h>
#include <stdio.h>

#include "dot/gen_dot_ag.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

struct GDag {
    void *_unused;
};

static inline node_st *get_node_name(node_st *node) {
    if (NODE_TYPE(node) == NT_INODE) {
        return INODE_NAME(node);
    } else if (NODE_TYPE(node) == NT_CHILD) {
        return CHILD_NAME(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        return INODESET_NAME(node);
    }
}

GDag_st *GDag_dot_init(char *filename) {
    GDag_st *dot = MEMmalloc(sizeof(struct GDag));
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenAGDotFile(ctx, filename);
    OUT("digraph G {\n");
    return dot;
}

void GDag_dot_add_graph(GDag_st *dot, node_st *st, graph_st *graph,
                        char *name) {
    (void)dot;
    GeneratorContext *ctx = globals.gen_ctx;
    htable_st *seen = HTnew_String(10);
    OUT("subgraph cluster_%s {\n", name);
    OUT("style=\"filled\";\n");
    OUT("color=\"lightgrey\";\n");
    OUT("label=<Production of <B>%s</B>>;\n", name);
    for (struct GRnode *node = graph->nodes; node != NULL; node = node->next) {
        bool child = NODE_TYPE(node->node) == NT_CHILD;
        node_st *node_name = get_node_name(node->node);
        if (HTlookup(seen, ID_LWR(node_name)) == NULL) {
            HTinsert(seen, ID_LWR(node_name), node_name);
            OUT("subgraph cluster_%s__%s {\n", name, ID_LWR(node_name));
            OUT("style=\"filled\";\n");
            OUT("color=\"ivory\";\n");
            if (child) {
                OUT("label=<<B>%s</B> %s>;\n",
                    ID_ORIG(get_node_name(
                        STlookup(st, CHILD_TYPE_REFERENCE(node->node)))),
                    ID_ORIG(node_name));
            } else {
                OUT("label=<<B>%s</B>>;\n", ID_ORIG(node_name));
            }
            for (struct GRnode *attr = graph->nodes; attr != NULL;
                 attr = attr->next) {
                if (attr->node != node->node) {
                    continue;
                }
                OUT("attr_%s__%s__%s [label=\"%s\"]\n", name, ID_LWR(node_name),
                    ID_LWR(ATTRIBUTE_NAME(attr->attribute)),
                    ID_ORIG(ATTRIBUTE_NAME(attr->attribute)));
            }
            OUT("}\n");
        }
    }

    for (struct GRedge_list *entry = graph->edges; entry != NULL;
         entry = entry->next) {
        struct GRedge *edge = entry->edge;
        struct GRnode *n1 = edge->first;
        struct GRnode *n2 = edge->second;

        if (n1 == NULL) {
            fprintf(stderr, "n2: %s.%s %d\n", ID_ORIG(get_node_name(n2->node)),
                    ID_ORIG(ATTRIBUTE_NAME(n2->attribute)), edge->induced);
            fflush(stderr);
        }

        OUT("attr_%s__%s__%s -> attr_%s__%s__%s", name,
            ID_LWR(get_node_name(n1->node)),
            ID_LWR(ATTRIBUTE_NAME(n1->attribute)), name,
            ID_LWR(get_node_name(n2->node)),
            ID_LWR(ATTRIBUTE_NAME(n2->attribute)));
        if (edge->induced) {
            OUT(" [style=\"dotted\"]");
        }
        OUT(";\n");
    }

    OUT("}\n");
    HTdelete(seen);
}
void GDag_dot_finish(GDag_st *dot) {
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("}\n");
    MEMfree(dot);
}
