#include <stdio.h>

#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "ccn/phase_driver.h"


extern int **reachability_matrix;


static int opt_counter = 0;
node_st *doOpts(node_st *ast)
{
    printf("Opt: %d\n", opt_counter);
    if (opt_counter < 50) {
        CCNcycleNotify();
    }
    opt_counter++;
    return ast;
}

node_st *PRTast(node_st *ast)
{
    printf("AST:\n");
    printf("Child: %d\n", NODE_TYPE(AST_IPHASES(ast)));
    TRAVchildren(ast);

    for (int i = 0; i < AST_NUM_TRAVERSALS(ast); i++) {
        printf("%d: ", i);
        for (int j = 0; j < AST_NUM_NODES(ast); j++) {
            printf("%d, ", reachability_matrix[i][j]);
        }
        printf("\n");
    }
    return ast;
}

node_st *PRTiphase(node_st *node)
{
    printf("Phase: %s\n", ID_ORIG(IPHASE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTitraversal(node_st *node)
{
    printf("Trav: %s(%d)\n", ID_ORIG(ITRAVERSAL_NAME(node)), ITRAVERSAL_INDEX(node));
    TRAVchildren(node);
    return node;
}

node_st *PRTitravdata(node_st *node)
{
    printf("Travdata: %s\n", ID_ORIG(ITRAVDATA_NAME(node)));
    return node;
}

node_st *PRTipass(node_st *node)
{
    printf("Pass: %s\n", ID_ORIG(IPASS_NAME(node)));
    TRAVchildren(node);
    return node;
}


node_st *PRTiactions(node_st *node)
{
    printf("Action: %s: %d\n", ID_ORIG(IACTIONS_REFERENCE(node)), IACTIONS_ACTION_ID(node));
    TRAVchildren(node);
    return node;
}


node_st *PRTinode(node_st *node)
{
    printf("NODE: %s(%d)\n", ID_ORIG(INODE_NAME(node)), INODE_INDEX(node));
    TRAVchildren(node);
    return node;
}

node_st *PRTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTattribute(node_st *node)
{
    printf("Attr: %s\n", ID_ORIG(ATTRIBUTE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTsetreference(node_st *node)
{
    printf("Set refence: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTsetliteral(node_st *node)
{
    if (SETLITERAL_REFERENCE(node)) {
        printf("%s\n", ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *PRTinodeset(node_st *node)
{
    printf("NODESET {\n");
    TRAVopt(INODESET_EXPR(node));
    printf("} END\n");
    TRAVopt(INODESET_NEXT(node));
    return node;
}

node_st *PRTid(node_st *node)
{
    printf("Ref: %s\n", ID_ORIG(node));
    return node;
}

node_st *PRTsetoperation(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTienum(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTste(node_st *node)
{
    printf("STE: %s, %d\n", ID_ORIG(STE_KEY(node)), NODE_TYPE(STE_VALUE(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTilifetime(node_st *node)
{
    if (ILIFETIME_TYPE(node) == LT_mandatory) {
        printf("mandatory");
    } else {
        printf("disallowed");
    }
    printf("(");
    TRAVopt(ILIFETIME_BEGIN(node));
    printf(" -> ");
    TRAVopt(ILIFETIME_END(node));
    printf(")\n");

    TRAVopt(ILIFETIME_NEXT(node));
    return node;
}

node_st *PRTlifetime_range(node_st *node)
{
    printf("%d", LIFETIME_RANGE_ACTION_ID(node));
    return node;
}