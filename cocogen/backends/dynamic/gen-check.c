
#include "ast/ast.h"
#include "format/out_macros.h"
#include "filehandling/gen-driver.h"

void GenNodesetFunc(struct ast *ast, FILE *fp, struct nodeset *nodeset)
{
    int indent = 0;
    OUT_START_FUNC("static bool NodeIsIn%s(struct ccn_node *arg_node)", nodeset->name->orig);
    OUT("return (");
    struct node_wrapper *wrapper = NULL;
    SLIST_FOREACH(wrapper, nodeset->nodes, next) {
        struct node *node = wrapper->node;
        OUT_NO_INDENT("NODE_TYPE(arg_node) == NT_%s || ", node->name->upr);
    }
    OUT_NO_INDENT("false);\n"); // We add a false to finish the || statement.
    OUT_END_FUNC();
}

void GenCheckFunc(struct ast *ast, FILE *fp, struct node *node)
{
    int indent = 0;
    OUT_START_FUNC("struct ccn_node *CHK%s(struct ccn_node *arg_node)", node->name->lwr);

    struct child *child = NULL;
    SLIST_FOREACH(child, node->children, next) {
        OUT_BEGIN_IF("%s_%s(arg_node)", node->name->upr, child->name->upr);
        if (child->type == CT_NODE) {
            OUT_BEGIN_IF("NODE_TYPE(%s_%s(arg_node)) != NT_%s", node->name->upr, child->name->upr, child->type_reference->upr);
        } else if (child->type == CT_NODESET) {
            OUT_BEGIN_IF("!NodeIsIn%s(%s_%s(arg_node))", child->type_reference->orig, node->name->upr, child->name->upr);
        }

        // TODO: improve error message.
        OUT_FIELD("CTIerror(\"Inconsistency found!\")");
        OUT_END_IF();
        OUT_END_IF();
    }

    if (!SLIST_EMPTY(node->children)) {
        OUT_FIELD("arg_node = TRAVchildren(arg_node)");
    }
    OUT_FIELD("return arg_node");
    OUT_END_FUNC();
}

struct ast *GCHKdoGen(struct ast *ast)
{
    FILE *fp = FHgetSourceFile("ccn_check.c");
    int indent = 0;
    OUT("#include <stdlib.h>\n");
    OUT("#include <stdbool.h>\n");
    OUT("\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    OUT("#include \"palm/ctinfo.h\"\n");
    OUT("\n");

    struct nodeset *nset = NULL;
    STAILQ_FOREACH(nset, ast->nodesets, next) {
        GenNodesetFunc(ast, fp, nset);
    }

    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        GenCheckFunc(ast, fp, node);
    }

    return ast;
}