#include <assert.h>
#include <stdio.h>

#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "generator/generator.h"

static char *basic_node_type = "node_st";
static int child_num = 0;
static node_st *ste = NULL;
static node_st *ast = NULL;
static node_st *curr_node = NULL;

node_st *DGNSast(node_st *node)
{
    ste = AST_STABLE(node);
    ast = node;
    TRAVinodes(node);
    return node;
}

node_st *DGNSinode(node_st *node)
{
    char *name_upr = ID_UPR(INODE_NAME(node));
    char *name_lwr = ID_LWR(INODE_NAME(node));
    GeneratorContext *ctx = globals.gen_ctx;

    OUT_STRUCT("NODE_DATA_%s", name_upr);
    {
        if (INODE_ICHILDREN(node)) {
            OUT_UNION("NODE_CHILDREN_%s", name_upr);
            OUT_STRUCT("NODE_CHILDREN_%s_STRUCT", name_upr);
            child_num = 0;
            TRAVichildren(node);
            OUT_TYPEDEF_STRUCT_END("%s_children_st", name_lwr);
            OUT_FIELD("%s *%s_children_at[%d]", basic_node_type, name_lwr, child_num);
            OUT_TYPEDEF_STRUCT_END("%s_children", name_lwr);
        }
        curr_node = node;
        TRAViattributes(node);
        curr_node = NULL;
    }
    OUT_STRUCT_END();

    TRAVnext(node);
    return node;
}

node_st *DGNSchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    child_num++;
    OUT_FIELD("%s *%s", basic_node_type, ID_LWR(CHILD_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGNSattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    assert(curr_node != NULL);
    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        OUT_STRUCT("NODE_%s_%s_STRUCT", ID_UPR(INODE_NAME(curr_node)), ID_UPR(ATTRIBUTE_NAME(node)));
        OUT_FIELD("bool ccn_val_set");

    }

    if (ATTRIBUTE_TYPE(node) == AT_link) {
        OUT_FIELD("%s *%s", basic_node_type, ID_LWR(ATTRIBUTE_NAME(node)));
    } else if (ATTRIBUTE_TYPE(node) == AT_link_or_enum) {
        node_st *ref = STlookup(ste, ATTRIBUTE_TYPE_REFERENCE(node));
        // Should be handled by check_existence.
        assert(ref);
        assert(NODE_TYPE(ref) == NT_IENUM);
        OUT_FIELD("enum %s %s", ID_ORIG(IENUM_NAME(ref)), ID_LWR(ATTRIBUTE_NAME(node)));
    } else if (ATTRIBUTE_TYPE(node) == AT_user) {
        OUT_FIELD("%s %s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)), ID_LWR(ATTRIBUTE_NAME(node)));
        AST_USES_UNSAFE(ast) = true;
    } else {
        OUT_FIELD("%s %s", FMTattributeTypeToString(ATTRIBUTE_TYPE(node)), ID_LWR(ATTRIBUTE_NAME(node)));
    }

    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        OUT_TYPEDEF_STRUCT_END("_%s", ID_LWR(ATTRIBUTE_NAME(node)));
    }
    TRAVchildren(node);
    return node;
}
