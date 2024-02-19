#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"
#include "gen_helpers/out_macros.h"
#include <assert.h>
#include "globals.h"
#include "generator/generator.h"

static bool decl_round;

char *SGNSnodeName(node_st *node)
{
    assert(NODE_TYPE(node) == NT_INODE);
    return ID_ORIG(INODE_NAME(node));
}

node_st *SGNSast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "ast.h");
    decl_round = true;
    TRAVinodes(node);
    TRAVinodesets(node);
    decl_round = false;
    OUT_STRUCT("ccn_node");
    OUT_FIELD("enum nodetype type");
    OUT_STRUCT_END();
    OUT_FIELD("typedef struct ccn_node ccn_node");
    TRAVinodes(node);
    TRAVinodesets(node);
    return node;
}

node_st *SGNSinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (decl_round) {
        OUT_FIELD("struct %s", SGNSnodeName(node));
    } else {
        OUT_STRUCT("%s", SGNSnodeName(node));
        OUT_FIELD("ccn_node ccn_base");
        TRAVichildren(node);
        TRAViattributes(node);
        OUT_STRUCT_END();
    }
    TRAVnext(node);
    return node;
}

node_st *SGNSinodeset(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (decl_round) {
        OUT_FIELD("struct %s", ID_ORIG(INODESET_NAME(node)));
    } else {
        OUT_STRUCT("%s", ID_ORIG(INODESET_NAME(node)));
        OUT_FIELD("ccn_node ccn_base");
        OUT_FIELD("enum nodetype type");
        OUT_UNION("node");
        TRAVexpr(node);
        OUT_STRUCT_END();
        OUT_STRUCT_END();
    }
    TRAVnext(node);
    return node;
}

node_st *SGNSsetliteral(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_FIELD("%s *%s", ID_ORIG(SETLITERAL_REFERENCE(node)), ID_ORIG(SETLITERAL_REFERENCE(node)));
    TRAVleft(node);
    TRAVright(node);
    return node;
}

node_st *SGNSchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_FIELD("struct %s *%s", ID_ORIG(CHILD_TYPE_REFERENCE(node)), ID_ORIG(CHILD_NAME(node)));
    TRAVnext(node);
    return node;
}

node_st *SGNSattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(node) == AT_link) {
        OUT_FIELD("struct %s *%s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)), ID_ORIG(ATTRIBUTE_NAME(node)));
    } else {
        OUT_FIELD("%s %s", FMTattributeTypeToString(ATTRIBUTE_TYPE(node)), ID_ORIG(ATTRIBUTE_NAME(node)));
    }
    TRAVnext(node);
    return node;
}

