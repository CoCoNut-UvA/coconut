#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "dynamic_backend/gen_helpers.h"
#include "frontend/symboltable.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
static char *basic_node_type = "node_st";
static node_st *ste = NULL;
static node_st *curr_node = NULL;

static inline void print_type(node_st *attribute) {
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(attribute) == AT_link) {
        OUT_NO_INDENT("%s *", basic_node_type);
    } else if (ATTRIBUTE_TYPE(attribute) == AT_link_or_enum) {
        node_st *ref = STlookup(ste, ATTRIBUTE_TYPE_REFERENCE(attribute));
        // Should be handled by check_existence.
        assert(ref);
        assert(NODE_TYPE(ref) == NT_IENUM);
        OUT("enum %s", ID_ORIG(IENUM_NAME(ref)));
    } else if (ATTRIBUTE_TYPE(attribute) == AT_user) {
        OUT_NO_INDENT("%s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(attribute)));
    } else {
        OUT_NO_INDENT("%s",
                      FMTattributeTypeToString(ATTRIBUTE_TYPE(attribute)));
    }
}

node_st *DGNAFast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVopt(AST_INODES(node));
    return node;
}

node_st *DGNAFinode(node_st *node)
{
    curr_node = node;
    TRAVopt(INODE_IATTRIBUTES(node));
    curr_node = NULL;
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGNAFattribute(node_st *node)
{
    assert(curr_node);
    GeneratorContext *ctx = globals.gen_ctx;

    if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
        OUT("static inline ");
        print_type(node);
        OUT_NO_INDENT(" CCNaccess_readonly_%s_%s(%s *node) {\n", ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)), basic_node_type);
        GNindentIncrease(ctx);
        OUT("DBUG_ASSERT(node->data.N_%s->_%s.ccn_val_set == true,\n", ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)));
        OUT("            \"Trying to access attribute before it has been evaluated.\");\n");
        OUT("return node->data.N_%s->_%s.%s;\n", ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)), ID_LWR(ATTRIBUTE_NAME(node)));
        OUT_END_FUNC();
    }
    TRAVnext(node);
    return node;
}
