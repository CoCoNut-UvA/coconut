#include "ccn/phase_driver.h"
#include "ccngen/ast.h"
#include "ccngen/ag.h"
#include "ccngen/enum.h"
#include "ccngen/visit.h"
#include "palm/ctinfo.h"

static inline char *nodetypeToName(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_ID:
            return "id";
        case NT_IENUM:
            return "ienum";
        case NT_VISIT_SEQUENCE_DUMMY:
            return "visit_sequence_dummy";
        case NT_VISIT_SEQUENCE_VISIT:
            return "visit_sequence_visit";
        case NT_VISIT_SEQUENCE_EVAL:
            return "visit_sequence_eval";
        case NT_VISIT:
            return "visit";
        case NT_VISIT_ARG_LIST:
            return "visit_arg_list";
        case NT_ATTRIBUTE_REFERENCE:
            return "attribute_reference";
        case NT_EQUATION_DEPENDENCY:
            return "equation_dependency";
        case NT_EQUATION:
            return "equation";
        case NT_ATTRIBUTE:
            return "attribute";
        case NT_ITRAVDATA:
            return "itravdata";
        case NT_SETOPERATION:
            return "setoperation";
        case NT_SETLITERAL:
            return "setliteral";
        case NT_SETREFERENCE:
            return "setreference";
        case NT_STE:
            return "ste";
        case NT_CHILD:
            return "child";
        case NT_LIFETIME_RANGE:
            return "lifetime_range";
        case NT_ILIFETIME:
            return "ilifetime";
        case NT_NODESET_CHILD_ENTRY:
            return "nodeset_child_entry";
        case NT_INODESET:
            return "inodeset";
        case NT_INODE:
            return "inode";
        case NT_IPASS:
            return "ipass";
        case NT_ITRAVERSAL:
            return "itraversal";
        case NT_IPHASE:
            return "iphase";
        case NT_IACTIONS:
            return "iactions";
        case NT_AST:
            return "ast";
        default:
            return "Unknown";
    }

}

void CCNevaluateAttributes(node_st *node) {
    switch (NODE_TYPE(node)) {
        default:
            break;
    }

    CTI(CTI_ERROR, true, "CCNevaluateAttributes called with node of invalid node type '%s'.", nodetypeToName(node));
    CTIabortCompilation();
}

