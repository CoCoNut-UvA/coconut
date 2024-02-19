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

static inline size_t lookup_partition(htable_st *partition_table, node_st *st,
                                      struct GRnode *node) {
    node_st *ref;
    if (NODE_TYPE(node->node) == NT_CHILD) {
        ref = STlookup(st, CHILD_TYPE_REFERENCE(node->node));
    } else {
        ref = node->node;
    }

    assert(NODE_TYPE(ref) == NT_INODE || NODE_TYPE(ref) == NT_INODESET);

    htable_st *subtable = (htable_st *)HTlookup(partition_table, ref);
    if (subtable == NULL) {
        return 0;
    }
    size_t part = (size_t)HTlookup(subtable,
                                   ID_LWR(ATTRIBUTE_NAME(node->attribute)));
    if (part == 0) {
        fprintf(stderr, "NO PARTITION %s.%s: %p.%p\n", ID_ORIG(get_node_name(node->node)), ID_ORIG(ATTRIBUTE_NAME(node->attribute)), (void *)node->node, (void *)node->attribute);
        for (htable_iter_st *iter = HTiterate(subtable); iter != NULL; iter = HTiterateNext(iter)) {
            node_st *attr = HTiterKey(iter);
            size_t part_attr = (size_t) HTiterValue(iter);
            fprintf(stderr, "Candidate %s.%s: %p.%p (%lu)\n", ID_ORIG(get_node_name(ref)), ID_ORIG(ATTRIBUTE_NAME(attr)), (void *)ref, (void *)attr, part_attr);
        }
        fflush(stderr);
    }

    return part;
}

void GDag_dot_add_graph(GDag_st *dot, node_st *st, graph_st *graph, char *name,
                        htable_st *partition_table) {
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
                OUT("attr_%s__%s__%s [label=<%s", name, ID_LWR(node_name),
                    ID_LWR(ATTRIBUTE_NAME(attr->attribute)),
                    ID_ORIG(ATTRIBUTE_NAME(attr->attribute)));

                if (partition_table != NULL) {
                    size_t partition =
                        lookup_partition(partition_table, st, attr);

                    if (partition == 0) {
                        OUT(" <I><SUB>NaN</SUB></I>");
                    } else if (partition % 2 == 0) {
                        OUT(" <I>I<SUB>%d</SUB></I>", partition / 2);
                    } else {
                        OUT(" <I>S<SUB>%d</SUB></I>", partition / 2);
                    }
                }
                OUT(">];\n");
            }
            OUT("}\n");
        }
    }

    for (struct GRedge_list *entry = graph->edges; entry != NULL;
         entry = entry->next) {
        struct GRedge *edge = entry->edge;
        struct GRnode *n1 = edge->first;
        struct GRnode *n2 = edge->second;

        if (edge->type == GRinduced && n1->node != n2->node) {
            continue;
        }

        OUT("attr_%s__%s__%s -> attr_%s__%s__%s", name,
            ID_LWR(get_node_name(n1->node)),
            ID_LWR(ATTRIBUTE_NAME(n1->attribute)), name,
            ID_LWR(get_node_name(n2->node)),
            ID_LWR(ATTRIBUTE_NAME(n2->attribute)));
        if (edge->type == GRinduced) {
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
