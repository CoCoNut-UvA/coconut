#include <stdio.h>

#include "ccngen/trav.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "ccn/phase_driver.h"
#include "globals.h"


extern int **reachability_matrix;

static int opt_counter = 0;
static int indent = 0;

#define INDENT indent += 4
#define UNINDENT indent -= 4

static void PrintIndent()
{
    for (int i = 0; i < indent; i++) {
        putchar(' ');
    }
}

node_st *doOpts(node_st *ast)
{
    printf("Opt: %d\n", opt_counter);
    if (opt_counter < 50) {
        CCNcycleNotify();
    }
    opt_counter++;
    return ast;
}

node_st *PRTast(node_st *ast)
{
    if (!globals.show_ast) {
        return ast;
    }
    TRAVchildren(ast);

    if (reachability_matrix) {
        printf("Reachability table:\n");
        for (int i = 0; i < AST_NUM_TRAVERSALS(ast); i++) {
            printf("%d: ", i);
            for (int j = 0; j < AST_NUM_NODES(ast); j++) {
                printf("%d, ", reachability_matrix[i][j]);
            }
            printf("\n");
        }
    }
    return ast;
}

node_st *PRTiphase(node_st *node)
{
    printf("Phase %s {\n", ID_ORIG(IPHASE_NAME(node)));
    INDENT;
    PrintIndent();
    printf("Actions {\n");
    INDENT;
    TRAViactions(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    UNINDENT;
    printf("}\n");
    TRAVnext(node);
    return node;
}

node_st *PRTitraversal(node_st *node)
{
    printf("Trav %s {\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    INDENT;
    PrintIndent();
    printf("Traversal index: %d\n", ITRAVERSAL_INDEX(node));
    PrintIndent();
    printf("nodes {\n");
    INDENT;
    TRAVinodes(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    UNINDENT;
    printf("}\n");
    TRAVnext(node);
    return node;
}

node_st *PRTitravdata(node_st *node)
{
    PrintIndent();
    printf("Travdata %s\n", ID_ORIG(ITRAVDATA_NAME(node)));
    return node;
}

node_st *PRTipass(node_st *node)
{
    printf("Pass: %s\n", ID_ORIG(IPASS_NAME(node)));
    return node;
}


node_st *PRTiactions(node_st *node)
{
    PrintIndent();
    printf("%s: %d\n", ID_ORIG(IACTIONS_REFERENCE(node)), IACTIONS_ACTION_ID(node));
    TRAVnext(node);
    return node;
}


node_st *PRTinode(node_st *node)
{
    printf("NODE: %s(%d) {\n", ID_ORIG(INODE_NAME(node)), INODE_INDEX(node));
    INDENT;
    PrintIndent();
    printf("Children {\n");
    INDENT;
    TRAVichildren(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    PrintIndent();
    printf("Attributes {\n");
    INDENT;
    TRAViattributes(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    PrintIndent();
    printf("Equations {\n");
    INDENT;
    TRAViequations(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    UNINDENT;
    printf("}\n");
    PrintIndent();
    printf("Visits {\n");
    INDENT;
    TRAVvisit(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");
    TRAVnext(node);

    return node;
}

node_st *PRTchild(node_st *node)
{
    TRAVname(node);
    TRAVnext(node);
    return node;
}

void print_attribute_type(enum attribute_type type, node_st *type_reference) {
    switch (type)
    {
        case AT_user:
        case AT_link:
        case AT_link_or_enum:
            printf("%s", ID_ORIG(type_reference));
            break;
        case AT_int:
        printf("int");
        break;
        case AT_string:
        printf("string");
        break;
        case AT_bool:
        printf("bool");
        break;
        case AT_int8:
        printf("int8");
        break;
        case AT_int16:
        printf("int16");
        break;
        case AT_int32:
        printf("int32");
        break;
        case AT_int64:
        printf("int64");
        break;
        case AT_float:
        printf("float");
        break;
        case AT_double:
        printf("double");
        break;
        case AT_uint:
        printf("uint");
        break;
        case AT_uint8:
        printf("uint8");
        break;
        case AT_uint16:
        printf("uint16");
        break;
        case AT_uint32:
        printf("uint32");
        break;
        case AT_uint64:
        printf("uint64");
        break;
    default:
        printf("<unknown type>");
        break;
    }
}

node_st *PRTattribute(node_st *node)
{
    bool prev = false;
    PrintIndent();
    print_attribute_type(ATTRIBUTE_TYPE(node), ATTRIBUTE_TYPE_REFERENCE(node));
    printf(" %s {", ID_ORIG(ATTRIBUTE_NAME(node)));
    if (ATTRIBUTE_IN_CONSTRUCTOR(node)) {
        printf("constructor");
        prev = true;
    }
    if (ATTRIBUTE_IS_SYNTHESIZED(node)) {
        if (prev) {
            printf(", ");
        }
        printf("synthesized");
        prev = true;
    }
    if (ATTRIBUTE_IS_INHERITED(node)) {
        if (prev) {
            printf(", ");
        }
        printf("inherited");
        prev = true;
    }
    if (ATTRIBUTE_LIFETIMES(node)) {
        if (prev) {
            printf(", ");
        }
        printf("lifetimes {\n");
        INDENT;
        TRAVlifetimes(node);
        UNINDENT;
        PrintIndent();
        printf("}");
    }
    printf("}\n");
    TRAVnext(node);
    return node;
}

/**
 * @fn PRTequation
 */
node_st *PRTequation(node_st *node)
{
    PrintIndent();
    TRAVrule(node);
    printf(" = {\n");
    INDENT;
    PrintIndent();
    printf("args = {");
    TRAViargs(node);
    printf("}\n");
    UNINDENT;
    PrintIndent();
    printf("}\n");
    TRAVnext(node);
    return node;
}

/**
 * @fn PRTequation_dependency
 */
node_st *PRTequation_dependency(node_st *node)
{
    TRAViattribute(node);
    if (EQUATION_DEPENDENCY_NEXT(node)) {
        printf(", ");
        TRAVnext(node);
    }
    return node;
}

/**
 * @fn PRTattribute_reference
 */
node_st *PRTattribute_reference(node_st *node)
{
    if (!ATTRIBUTE_REFERENCE_INODE(node)) {
        printf("<this>");
    } else {
        printf("%s", ID_ORIG(ATTRIBUTE_REFERENCE_INODE(node)));
        if (ATTRIBUTE_REFERENCE_NODE_TYPE(node)) {
            printf("<%s>", ID_ORIG(ATTRIBUTE_REFERENCE_NODE_TYPE(node)));
        }
    }
    printf(".%s", ID_ORIG(ATTRIBUTE_REFERENCE_IATTRIBUTE(node)));
    return node;
}

node_st *PRTsetreference(node_st *node)
{
    PrintIndent();
    printf("(ref) %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    return node;
}

node_st *PRTsetliteral(node_st *node)
{
    if (SETLITERAL_REFERENCE(node)) {
        PrintIndent();
        printf("%s\n", ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    TRAVleft(node);
    TRAVright(node);
    return node;
}

node_st *PRTsetoperation(node_st *node)
{
    PrintIndent();
    printf("(\n");
    INDENT;
    TRAVleft(node);
    UNINDENT;
    PrintIndent();
    switch (SETOPERATION_TYPE(node)) {
        case SO_difference:
            printf("-");
            break;
        case SO_intersect:
            printf("&");
            break;
        case SO_iunion:
            printf("|");
            break;
        default:
            printf("<unknown operator>");
            break;
    }
    printf("\n");
    INDENT;
    TRAVright(node);
    UNINDENT;
    PrintIndent();
    printf(")\n");
    return node;
}

node_st *PRTinodeset(node_st *node)
{
    printf("nodeset %s ", ID_ORIG(INODESET_NAME(node)));
    if (INODESET_ILLEGAL_SETEXPR_ATTR(node)) {
        printf("(illegal attr setexpr) ");
    }
    printf("{\n");
    INDENT;
    TRAVexpr(node);

    PrintIndent();
    printf("Child nodesets {\n");
    INDENT;
    TRAVchildren_table(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");

    PrintIndent();
    printf("Attributes {\n");
    INDENT;
    TRAViattributes(node);
    UNINDENT;
    PrintIndent();
    printf("}\n");

    UNINDENT;
    PrintIndent();
    printf("}\n");

    TRAVnext(node);
    return node;
}

node_st *PRTnodeset_child_entry(node_st *node) {
    PrintIndent();
    printf("%s\n", ID_ORIG(INODESET_NAME(NODESET_CHILD_ENTRY_REFERENCE(node))));
    TRAVnext(node);
    return node;
}

node_st *PRTid(node_st *node)
{
    PrintIndent();
    printf("%s\n", ID_ORIG(node));
    return node;
}

node_st *PRTienum(node_st *node)
{
    return node;
}

node_st *PRTste(node_st *node)
{
    printf("STE: %s, %d\n", ID_ORIG(STE_KEY(node)), NODE_TYPE(STE_VALUE(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTilifetime(node_st *node)
{
    PrintIndent();
    if (ILIFETIME_TYPE(node) == LT_mandatory) {
        printf("mandatory");
    } else {
        printf("disallowed");
    }
    printf("(");
    TRAVbegin(node);
    printf(" -> ");
    TRAVend(node);
    printf(")\n");

    TRAVnext(node);
    return node;
}

node_st *PRTlifetime_range(node_st *node)
{
    printf("%d", LIFETIME_RANGE_ACTION_ID(node));
    return node;
}

/**
 * @fn PRTvisit_arg_list
 */
node_st *PRTvisit_arg_list(node_st *node)
{
    TRAVattribute(node);

    if (VISIT_ARG_LIST_NEXT(node)) {
        printf(", ");
        TRAVnext(node);
    }

    return node;
}

/**
 * @fn PRTvisit
 */
node_st *PRTvisit(node_st *node)
{
    PrintIndent();
    printf("%s_%lu (in: {", ID_LWR(INODE_NAME(VISIT_INODE(node))),
                            VISIT_INDEX(node));
    TRAVinputs(node);
    printf("} out: {");
    TRAVoutputs(node);
    printf("}) {\n");
    INDENT;
    TRAVsequence(node);
    UNINDENT;
    printf("}\n");
    TRAVnext(node);
    return node;
}

/**
 * @fn PRTvisit_sequence_eval
 */
node_st *PRTvisit_sequence_eval(node_st *node)
{
    PrintIndent();
    printf("eval ");
    TRAVattribute(node);
    printf("\n");
    TRAVnext(node);
    return node;
}

/**
 * @fn PRTvisit_sequence_dummy
 */
node_st *PRTvisit_sequence_dummy(node_st *node)
{
    PrintIndent();
    printf("<dummy visit for %s>", ID_ORIG(INODE_NAME(VISIT_SEQUENCE_DUMMY_INODE(node))));
    if (VISIT_SEQUENCE_DUMMY_ALT(node)) {
        printf(" or\n");
        INDENT;
        TRAValt(node);
        UNINDENT;
    } else {
        printf("\n");
    }
    return node;
}

/**
 * @fn PRTvisit_sequence_visit
 */
node_st *PRTvisit_sequence_visit(node_st *node)
{
    node_st *visit = VISIT_SEQUENCE_VISIT_VISIT(node);
    PrintIndent();
    if (visit) {
    printf("visit %s %s_%lu", ID_LWR(CHILD_NAME(VISIT_SEQUENCE_VISIT_CHILD(node))),
                            ID_LWR(INODE_NAME(VISIT_INODE(visit))), VISIT_INDEX(visit));
    }

    if (VISIT_SEQUENCE_VISIT_ALT(node)) {
        printf(" or\n");
        INDENT;
        TRAValt(node);
        UNINDENT;
    } else {
        printf("\n");
    }

    TRAVnext(node);
    return node;
}
