#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
struct ccn_node *DELid(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_id->orig);
    MEMfree(arg_node->data.N_id->lwr);
    MEMfree(arg_node->data.N_id->upr);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_id);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELienum(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ienum->iinfo);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_ienum);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELvisit_sequence_dummy(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_visit_sequence_dummy);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELvisit_sequence_visit(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_visit_sequence_visit);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELvisit_sequence_eval(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_visit_sequence_eval);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELvisit(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_visit);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELvisit_arg_list(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_visit_arg_list);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELattribute_reference(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_attribute_reference);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELequation_dependency(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_equation_dependency);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELequation(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_equation);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELattribute(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_attribute);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELitravdata(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itravdata->include_file);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_itravdata);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetoperation(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_setoperation);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetliteral(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_setliteral);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetreference(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_setreference);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELste(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_ste);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELchild(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_child);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELlifetime_range(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_lifetime_range);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELilifetime(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_ilifetime);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELnodeset_child_entry(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_nodeset_child_entry);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELinodeset(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inodeset->iinfo);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_inodeset);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELinode(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inode->iifno);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_inode);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELipass(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ipass->iifno);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_ipass);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELitraversal(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itraversal->iinfo);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_itraversal);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELiphase(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_iphase->iinfo);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_iphase);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELiactions(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_iactions);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELast(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(NODE_FILENAME(arg_node));
    MEMfree(arg_node->data.N_ast);
    MEMfree(arg_node);
    return NULL;
}

