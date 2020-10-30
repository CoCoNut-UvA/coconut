#include "ccngen/ast.h"
#include <stddef.h>
struct ccn_node *CPYid(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTid(    NULL    ,     NULL    ,     NULL);
    return new_node;
}

struct ccn_node *CPYienum(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTienum(    NULL    ,     NULL    ,     NULL    ,     NULL);
    return new_node;
}

struct ccn_node *CPYattribute(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTattribute();
    return new_node;
}

struct ccn_node *CPYitravdata(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTitravdata(    NULL);
    return new_node;
}

struct ccn_node *CPYsetoperation(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetoperation(    NULL    ,     NULL    ,     0);
    return new_node;
}

struct ccn_node *CPYsetliteral(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetliteral();
    return new_node;
}

struct ccn_node *CPYsetreference(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetreference();
    return new_node;
}

struct ccn_node *CPYste(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTste();
    return new_node;
}

struct ccn_node *CPYchild(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTchild(    NULL);
    return new_node;
}

struct ccn_node *CPYinodeset(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTinodeset();
    return new_node;
}

struct ccn_node *CPYinode(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTinode(    NULL    ,     NULL);
    return new_node;
}

struct ccn_node *CPYipass(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTipass(    NULL    ,     NULL    ,     NULL);
    return new_node;
}

struct ccn_node *CPYitraversal(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTitraversal(    NULL);
    return new_node;
}

struct ccn_node *CPYiphase(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTiphase(    NULL    ,     0);
    return new_node;
}

struct ccn_node *CPYiactions(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTiactions();
    return new_node;
}

struct ccn_node *CPYast(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTast();
    return new_node;
}

