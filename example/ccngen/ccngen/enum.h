#pragma once
enum ccn_nodetype {
    NT_NULL,
    NT_BINOP,
    NT_NUM,
    _NT_SIZE,
};

 enum ccn_nodesettype {
    NS_NULL,
    NS_Constants,
    NS_Expr,
    _NS_SIZE,
};

 enum ccn_pass_type {
    PASS_NULL,
    PASS_P,
    _PASS_SIZE,
};

 enum ccn_traversal_type {
    TRAV_NULL,
    TRAV_CHANGE,
    TRAV_TRAV,
    TRAV_free,
    TRAV_copy,
    TRAV_check,
    _TRAV_SIZE,
};

 enum ccn_phase_type {
    PHASE_NULL,
    PHASE_MAIN,
    PHASE_AGAIN,
    _PHASE_SIZE,
};

 enum binaryoperation {
    BO_NULL,
    BO_add,
    BO_neg,
    BO_mult,
    _BO_SIZE,
};

 