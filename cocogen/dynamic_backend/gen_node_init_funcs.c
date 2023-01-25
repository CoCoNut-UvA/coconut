#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "dynamic_backend/gen_helpers.h"

bool dgif_print_semicolon = false;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int arg_num = 0;

node_st *DGIFast(node_st *node)
{
    dgif_print_semicolon = true;
    TRAVchildren(node);
    return node;
}

node_st *DGIFinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    curr_node_name = ID_LWR(INODE_NAME(node));
    curr_node_name_upr = ID_UPR(INODE_NAME(node));
    OUT("%s *%s%s(", basic_node_type, "AST", ID_LWR(INODE_NAME(node)));
    arg_num = 0;
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    curr_node_name_upr = NULL;
    curr_node_name = NULL;
    if (arg_num == 0) {
        OUT("void");
    }
    OUT(")");
    if (dgif_print_semicolon) {
        OUT(";\n");
    }
    TRAVopt(INODE_NEXT(node));
    return node;
}


node_st *DGIFchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (CHILD_IN_CONSTRUCTOR(node)) {
        if (arg_num > 0) {
            OUT(", ");
        }
        arg_num++;
        OUT("%s *%s", basic_node_type, ID_LWR(CHILD_NAME(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *DGIFattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_IN_CONSTRUCTOR(node)) {
        if (arg_num > 0) {
            OUT(", ");
        }
        arg_num++;
        if (ATTRIBUTE_TYPE(node) == AT_link) {
            OUT("%s *%s", basic_node_type, DGHattributeField(node));
        } else if (ATTRIBUTE_TYPE(node) == AT_link_or_enum) {
            OUT("enum %s %s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)), DGHattributeField(node));
        } else {
            OUT("%s %s", FMTattributeTypeToString(ATTRIBUTE_TYPE(node)), DGHattributeField(node));
        }
    }
    TRAVchildren(node);
    return node;
}
