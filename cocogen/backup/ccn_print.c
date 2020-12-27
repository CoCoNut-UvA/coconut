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
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTiactions(node_st *node) {
    printIndent();
    printf("(iactions\n");
    indent += 4;
    printIndent();
    printf("(attribute (int action_id))\n");
    printIndent();
    printf("(attribute (node_st reference))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
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
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTitraversal(node_st *node) {
    printIndent();
    printf("(itraversal\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    printIndent();
    printf("(attribute (int index))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTipass(node_st *node) {
    printIndent();
    printf("(ipass\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iifno))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
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
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTinodeset(node_st *node) {
    printIndent();
    printf("(inodeset\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTilifetime(node_st *node) {
    printIndent();
    printf("(ilifetime\n");
    indent += 4;
    printIndent();
    printf("(attribute (enum type))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTlifetime_range(node_st *node) {
    printIndent();
    printf("(lifetime_range\n");
    indent += 4;
    printIndent();
    printf("(attribute (int action_id))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
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
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTste(node_st *node) {
    printIndent();
    printf("(ste\n");
    indent += 4;
    printIndent();
    printf("(attribute (node_st value))\n");
    printIndent();
    printf("(attribute (node_st key))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTsetreference(node_st *node) {
    printIndent();
    printf("(setreference\n");
    indent += 4;
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTsetliteral(node_st *node) {
    printIndent();
    printf("(setliteral\n");
    indent += 4;
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTsetoperation(node_st *node) {
    printIndent();
    printf("(setoperation\n");
    indent += 4;
    printIndent();
    printf("(attribute (enum type))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTitravdata(node_st *node) {
    printIndent();
    printf("(itravdata\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * include_file))\n");
    printIndent();
    printf("(attribute (enum type))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTattribute(node_st *node) {
    printIndent();
    printf("(attribute\n");
    indent += 4;
    printIndent();
    printf("(attribute (int in_constructor))\n");
    printIndent();
    printf("(attribute (enum type))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
node_st *CCNPRTienum(node_st *node) {
    printIndent();
    printf("(ienum\n");
    indent += 4;
    printIndent();
    printf("(attribute (char * iinfo))\n");
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
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
    TRAVchildren(node);
    indent -= 4;
    printIndent();
    printf(")");
}
