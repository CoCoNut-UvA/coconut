#include "ccngen/ast.h"
struct ccn_node *DELid(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_id->orig);
    MEMfree(arg_node->data.N_id);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELienum(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ienum->iinfo);
    MEMfree(arg_node->data.N_ienum);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELattribute(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_attribute);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELitravdata(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itravdata);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELsetoperation(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setoperation);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELsetliteral(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setliteral);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELsetreference(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_setreference);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELste(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ste);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELchild(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_child);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELlifetime_range(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_lifetime_range);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELilifetime(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ilifetime);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELinodeset(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inodeset->iinfo);
    MEMfree(arg_node->data.N_inodeset);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELinode(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_inode->iifno);
    MEMfree(arg_node->data.N_inode);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELipass(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ipass);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELitraversal(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_itraversal);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELiphase(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_iphase);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELiactions(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_iactions);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELast(struct ccn_node *arg_node) {
    TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_ast);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

