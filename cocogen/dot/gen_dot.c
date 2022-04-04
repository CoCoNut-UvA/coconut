/**
 * Generates a DOT diagram of the AST. Attributes of type node are
 * displayed via red eges. Elements from nodesets are displayed as blue and.
 */

#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "palm/hash_table.h"
#include <gen_helpers/out_macros.h>
#include <globals.h>
#include <stdio.h>

static node_st *curr_node;
static node_st *curr_nodeset;
static htable_st *seen;

node_st *GDast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    seen = HTnew_String(10);
    GNopenSourceFile(ctx, "ast.dot");
    OUT("digraph Ast {\n");
    TRAVopt(AST_INODES(node));
    TRAVopt(AST_INODESETS(node));
    OUT("}\n");
    HTdelete(seen);
    return node;
}

node_st *GDinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    HTclear(seen);
    curr_node = node;
    OUT("%s;\n", ID_ORIG(INODE_NAME(node)));
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *GDinodeset(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("%s;\n", ID_ORIG(INODESET_NAME(node)));
    curr_nodeset = node;
    TRAVopt(INODESET_EXPR(node));
    TRAVopt(INODESET_NEXT(node));
    return node;
}

node_st *GDsetliteral(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (node && SETLITERAL_REFERENCE(node)) {
        OUT("%s -> %s [color=blue]\n", ID_ORIG(INODESET_NAME(curr_nodeset)), ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    TRAVopt(SETLITERAL_RIGHT(node));
    TRAVopt(SETLITERAL_LEFT(node));
    return node;
}

node_st *GDattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(node) == AT_link) {
        OUT("%s -> %s[color=red, label=\"%s\"]\n",
            ID_ORIG(INODE_NAME(curr_node)), ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)),
            ID_ORIG(ATTRIBUTE_NAME(node)));
    }

    TRAVopt(ATTRIBUTE_NEXT(node));
    return node;
}

node_st *GDchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (!HTlookup(seen, ID_LWR(CHILD_TYPE_REFERENCE(node)))) {
        OUT("%s -> %s[label=\"%s\"]\n", ID_ORIG(INODE_NAME(curr_node)), ID_ORIG(CHILD_TYPE_REFERENCE(node)), ID_ORIG(CHILD_NAME(node)));
        //HTinsert(seen, ID_LWR(CHILD_TYPE_REFERENCE(node)), node);
    }

    TRAVopt(CHILD_NEXT(node));
    return node;
}
