#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
static int indent = 0;
static void printIndent()
{
    for (int i = 0; i < indent; i++) {
        putchar(' ');
    }
}

node_st *CCNPRTast(node_st *node) {
    printIndent();
    printf("(ast\n");
    indent += 4;
    printIndent();
    printf("(attribute (node_st start_phase))\n");
    printIndent();
    printf("(attribute (node_st root_node))\n");
    printIndent();
    printf("(attribute (int num_nodes))\n");
    printIndent();
    printf("(attribute (int num_traversals))\n");
    AST_IPHASES(node) = TRAVopt(AST_IPHASES(node));
    AST_ITRAVERSALS(node) = TRAVopt(AST_ITRAVERSALS(node));
    AST_IPASSES(node) = TRAVopt(AST_IPASSES(node));
    AST_INODES(node) = TRAVopt(AST_INODES(node));
    AST_INODESETS(node) = TRAVopt(AST_INODESETS(node));
    AST_ENUMS(node) = TRAVopt(AST_ENUMS(node));
    AST_STABLE(node) = TRAVopt(AST_STABLE(node));
    indent -= 4;
    printIndent();
    printf(")\n");
return node;
}
node_st *CCNPRTiactions(node_st *node) {
    printIndent();
    printf("(iactions\n");
    indent += 4;
    printIndent();
    printf("(attribute (int action_id))\n");
    printIndent();
    printf("(attribute (node_st reference))\n");
    indent -= 4;
    IACTIONS_NEXT(node) = TRAVopt(IACTIONS_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTiphase(node_st *node) {
    printIndent();
    printf("(iphase\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    printIndent();
    printf("(attribute (int is_cycle))\n");
    printIndent();
    printf("(attribute (int is_start))\n");
    IPHASE_NAME(node) = TRAVopt(IPHASE_NAME(node));
    IPHASE_IPREFIX(node) = TRAVopt(IPHASE_IPREFIX(node));
    IPHASE_GATE_FUNC(node) = TRAVopt(IPHASE_GATE_FUNC(node));
    IPHASE_IACTIONS(node) = TRAVopt(IPHASE_IACTIONS(node));
    indent -= 4;
    IPHASE_NEXT(node) = TRAVopt(IPHASE_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTitraversal(node_st *node) {
    printIndent();
    printf("(itraversal\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    printIndent();
    printf("(attribute (int index))\n");
    ITRAVERSAL_NAME(node) = TRAVopt(ITRAVERSAL_NAME(node));
    ITRAVERSAL_IPREFIX(node) = TRAVopt(ITRAVERSAL_IPREFIX(node));
    ITRAVERSAL_INODES(node) = TRAVopt(ITRAVERSAL_INODES(node));
    ITRAVERSAL_DATA(node) = TRAVopt(ITRAVERSAL_DATA(node));
    indent -= 4;
    ITRAVERSAL_NEXT(node) = TRAVopt(ITRAVERSAL_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTipass(node_st *node) {
    printIndent();
    printf("(ipass\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iifno))\n");
    IPASS_NAME(node) = TRAVopt(IPASS_NAME(node));
    IPASS_IPREFIX(node) = TRAVopt(IPASS_IPREFIX(node));
    IPASS_TARGET_FUNC(node) = TRAVopt(IPASS_TARGET_FUNC(node));
    indent -= 4;
    IPASS_NEXT(node) = TRAVopt(IPASS_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTinode(node_st *node) {
    printIndent();
    printf("(inode\n");
    indent += 4;
    printIndent();
    printf("(attribute (int index))\n");
    printIndent();
    printf("(attribute (int is_root))\n");
    printIndent();
    printf("(attribute (char * iifno))\n");
    INODE_NAME(node) = TRAVopt(INODE_NAME(node));
    INODE_ICHILDREN(node) = TRAVopt(INODE_ICHILDREN(node));
    INODE_IATTRIBUTES(node) = TRAVopt(INODE_IATTRIBUTES(node));
    INODE_LIFETIMES(node) = TRAVopt(INODE_LIFETIMES(node));
    indent -= 4;
    INODE_NEXT(node) = TRAVopt(INODE_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTinodeset(node_st *node) {
    printIndent();
    printf("(inodeset\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    INODESET_NAME(node) = TRAVopt(INODESET_NAME(node));
    INODESET_EXPR(node) = TRAVopt(INODESET_EXPR(node));
    INODESET_UNPACKED(node) = TRAVopt(INODESET_UNPACKED(node));
    indent -= 4;
    INODESET_NEXT(node) = TRAVopt(INODESET_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTilifetime(node_st *node) {
    printIndent();
    printf("(ilifetime\n");
    indent += 4;
    printIndent();
    printf("(attribute (enum type))\n");
    ILIFETIME_BEGIN(node) = TRAVopt(ILIFETIME_BEGIN(node));
    ILIFETIME_END(node) = TRAVopt(ILIFETIME_END(node));
    indent -= 4;
    ILIFETIME_NEXT(node) = TRAVopt(ILIFETIME_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTlifetime_range(node_st *node) {
    printIndent();
    printf("(lifetime_range\n");
    indent += 4;
    printIndent();
    printf("(attribute (int action_id))\n");
    LIFETIME_RANGE_TARGET(node) = TRAVopt(LIFETIME_RANGE_TARGET(node));
    indent -= 4;
    printIndent();
    printf(")\n");
return node;
}
node_st *CCNPRTchild(node_st *node) {
    printIndent();
    printf("(child\n");
    indent += 4;
    printIndent();
    printf("(attribute (int is_mandatory))\n");
    printIndent();
    printf("(attribute (int in_constructor))\n");
    printIndent();
    printf("(attribute (enum type))\n");
    printIndent();
    printf("(attribute (node_st type_reference))\n");
    CHILD_NAME(node) = TRAVopt(CHILD_NAME(node));
    CHILD_LIFETIMES(node) = TRAVopt(CHILD_LIFETIMES(node));
    indent -= 4;
    CHILD_NEXT(node) = TRAVopt(CHILD_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTste(node_st *node) {
    printIndent();
    printf("(ste\n");
    indent += 4;
    printIndent();
    printf("(attribute (node_st value))\n");
    printIndent();
    printf("(attribute (node_st key))\n");
    indent -= 4;
    STE_NEXT(node) = TRAVopt(STE_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTsetreference(node_st *node) {
    printIndent();
    printf("(setreference\n");
    indent += 4;
    SETREFERENCE_REFERENCE(node) = TRAVopt(SETREFERENCE_REFERENCE(node));
    indent -= 4;
    printIndent();
    printf(")\n");
return node;
}
node_st *CCNPRTsetliteral(node_st *node) {
    printIndent();
    printf("(setliteral\n");
    indent += 4;
    SETLITERAL_REFERENCE(node) = TRAVopt(SETLITERAL_REFERENCE(node));
    SETLITERAL_LEFT(node) = TRAVopt(SETLITERAL_LEFT(node));
    SETLITERAL_RIGHT(node) = TRAVopt(SETLITERAL_RIGHT(node));
    indent -= 4;
    printIndent();
    printf(")\n");
return node;
}
node_st *CCNPRTsetoperation(node_st *node) {
    printIndent();
    printf("(setoperation\n");
    indent += 4;
    printIndent();
    printf("(attribute (enum type))\n");
    SETOPERATION_LEFT(node) = TRAVopt(SETOPERATION_LEFT(node));
    SETOPERATION_RIGHT(node) = TRAVopt(SETOPERATION_RIGHT(node));
    indent -= 4;
    printIndent();
    printf(")\n");
return node;
}
node_st *CCNPRTitravdata(node_st *node) {
    printIndent();
    printf("(itravdata\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * include_file))\n");
    printIndent();
    printf("(attribute (enum type))\n");
    ITRAVDATA_NAME(node) = TRAVopt(ITRAVDATA_NAME(node));
    ITRAVDATA_TYPE_REFERENCE(node) = TRAVopt(ITRAVDATA_TYPE_REFERENCE(node));
    indent -= 4;
    ITRAVDATA_NEXT(node) = TRAVopt(ITRAVDATA_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTattribute(node_st *node) {
    printIndent();
    printf("(attribute\n");
    indent += 4;
    printIndent();
    printf("(attribute (int in_constructor))\n");
    printIndent();
    printf("(attribute (enum type))\n");
    ATTRIBUTE_NAME(node) = TRAVopt(ATTRIBUTE_NAME(node));
    ATTRIBUTE_TYPE_REFERENCE(node) = TRAVopt(ATTRIBUTE_TYPE_REFERENCE(node));
    indent -= 4;
    ATTRIBUTE_NEXT(node) = TRAVopt(ATTRIBUTE_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTienum(node_st *node) {
    printIndent();
    printf("(ienum\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    IENUM_VALS(node) = TRAVopt(IENUM_VALS(node));
    IENUM_NAME(node) = TRAVopt(IENUM_NAME(node));
    IENUM_IPREFIX(node) = TRAVopt(IENUM_IPREFIX(node));
    indent -= 4;
    IENUM_NEXT(node) = TRAVopt(IENUM_NEXT(node));
    printf(")\n");
return node;
}
node_st *CCNPRTid(node_st *node) {
    printIndent();
    printf("(id\n");
    indent += 4;
    printIndent();
    printf("(attribute (int col_end))\n");
    printIndent();
    printf("(attribute (int col_begin))\n");
    printIndent();
    printf("(attribute (int row))\n");
    printIndent();
    printf("(attribute (char * upr))\n");
    printIndent();
    printf("(attribute (char * lwr))\n");
    printIndent();
    printf("(attribute (char * orig))\n");
    indent -= 4;
    ID_NEXT(node) = TRAVopt(ID_NEXT(node));
    printf(")\n");
return node;
}
