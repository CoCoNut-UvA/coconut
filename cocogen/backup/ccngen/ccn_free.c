#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
struct ccn_node *DELid(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_id->orig);
    MEMfree(arg_node->data.N_id->lwr);
    MEMfree(arg_node->data.N_id->upr);
    MEMfree(arg_node->data.N_id);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELienum(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ienum->iinfo);
    MEMfree(arg_node->data.N_ienum);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELattribute(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_attribute);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELitravdata(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itravdata->include_file);
    MEMfree(arg_node->data.N_itravdata);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetoperation(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setoperation);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetliteral(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setliteral);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELsetreference(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setreference);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELste(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ste);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELchild(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_child);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELlifetime_range(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_lifetime_range);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELilifetime(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ilifetime);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELinodeset(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inodeset->iinfo);
    MEMfree(arg_node->data.N_inodeset);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELinode(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inode->iifno);
    MEMfree(arg_node->data.N_inode);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELipass(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ipass->iifno);
    MEMfree(arg_node->data.N_ipass);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELitraversal(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itraversal->iinfo);
    MEMfree(arg_node->data.N_itraversal);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELiphase(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_iphase->iinfo);
    MEMfree(arg_node->data.N_iphase);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELiactions(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_iactions);
    MEMfree(arg_node);
    return NULL;
}

struct ccn_node *DELast(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ast);
    MEMfree(arg_node);
    return NULL;
}

