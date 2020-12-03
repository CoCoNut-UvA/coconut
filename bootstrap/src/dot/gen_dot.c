#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include <stdio.h>
#include <gen_helpers/out_macros.h>
#include "filesystem/gen_files.h"
#include "palm/hash_table.h"

static node_st *curr_node;
static node_st *curr_nodeset;
static htable_st *seen;
FILE *fp;

node_st *GDast(node_st *node)
{
    seen = HTnew_String(10);
    fp = FSgetSourceFile("ast.dot");
    int indent = 0;
    OUT("digraph Ast {\n");
    TRAVopt(AST_INODES(node));
    TRAVopt(AST_INODESETS(node));
    OUT("}\n");
    fclose(fp);
    HTdelete(seen);
    return node;
}

node_st *GDinode(node_st *node)
{
    HTclear(seen);
    int indent = 0;
    curr_node = node;
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *GDinodeset(node_st *node)
{
    int indent = 0;
    OUT("%s [shape=box];\n", ID_ORIG(INODESET_NAME(node)));
    curr_nodeset = node;
    TRAVopt(INODESET_EXPR(node));
    TRAVopt(INODESET_NEXT(node));
    return node;
}

node_st *GDsetliteral(node_st *node)
{
    int indent = 0;
    if (node && SETLITERAL_REFERENCE(node)) {
        OUT("%s -> %s\n", ID_ORIG(INODESET_NAME(curr_nodeset)), ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    //TRAVopt(SETLITERAL_NEXT(node));
    return node;
}


node_st *GDchild(node_st *node)
{
    if (!HTlookup(seen, ID_LWR(CHILD_TYPE_REFERENCE(node)))) {
        int indent = 0;
        OUT("%s -> %s\n", ID_ORIG(INODE_NAME(curr_node)), ID_ORIG(CHILD_TYPE_REFERENCE(node)));
        HTinsert(seen, ID_LWR(CHILD_TYPE_REFERENCE(node)), node);
    }

    TRAVopt(CHILD_NEXT(node));
    return node;
}