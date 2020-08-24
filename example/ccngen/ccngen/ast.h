#include "ccn/ccn_types.h"
#include "ccngen/enum.h"
typedef struct ccn_node node_st;
struct NODE_DATA_BINOP {
    union NODE_CHILDREN_BINOP {
        struct NODE_CHILDREN_BINOP_STRUCT {
            node_st *left;
            node_st *right;
        } binop_children_st;

        node_st *binop_children_at[2];
    } binop_children;

};

#define BINOP_LEFT(n) ((n)->data.N_binop->binop_children.binop_children_st.left)
#define BINOP_RIGHT(n) ((n)->data.N_binop->binop_children.binop_children_st.right)

node_st *ASTnewBinOp(node_st *left, node_st *right);
struct NODE_DATA_NUM {
    union NODE_CHILDREN_NUM {
        struct NODE_CHILDREN_NUM_STRUCT {
        } num_children_st;

        node_st *num_children_at[0];
    } num_children;

    int value;
};

#define NUM_VALUE(n) ((n)->data.N_num->value)

node_st *ASTnewNum(int value);
// Attributes
union NODE_DATA {
    struct NODE_DATA_BINOP *N_binop;
    struct NODE_DATA_NUM *N_num;
};

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_CHILDREN(n) ((n)->children)
#define NODE_NUMCHILDREN(n) ((n)->num_children)
struct ccn_node {
    enum ccn_nodetype nodetype;
    union NODE_DATA data;
    struct ccn_node **children;
    long int num_children;
};

