#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

void *DGNallocTravData()
{
    return 0;
}

void DGNfreeTravData(void *data)
{

}

static FILE *fp;

node_st *DGNast(node_st *node)
{
    fp = stdout;
    TRAVchildren(node);
    return node;
}

node_st *DGNiactions(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNiphase(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNitraversal(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNipass(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNinode(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNinodeset(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNchild(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNattribute(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNienum(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNid(node_st *node)
{

    TRAVchildren(node);
    return node;
}