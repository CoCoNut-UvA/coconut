

#include "ast/ast.h"
#include "format/out_macros.h"
#include "filehandling/gen-driver.h"

static
void GenFreeFunc(struct ast *ast, FILE *fp, struct node *node) {
    int indent = 0;
    OUT_START_FUNC("struct ccn_node *DEL%s(struct ccn_node *arg_node)", node->name->lwr);
    if (!SLIST_EMPTY(node->children)) {
        OUT_FIELD("arg_node = TRAVchildren(arg_node)");
    }

    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, node->attributes, next) {
        // TODO: Should we free string attributes?
    }
    OUT_FIELD("MEMfree(arg_node->data.N_%s)", node->name->lwr);
    OUT_FIELD("arg_node = MEMfree(arg_node)");
    OUT_FIELD("return arg_node");
    OUT_END_FUNC();
}

struct ast *GFdoGen(struct ast *ast)
{
    FILE *fp = FHgetSourceFile("ccn_free.c");
    int indent = 0;
    OUT("#include <stdlib.h>\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    OUT("\n");

    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        GenFreeFunc(ast, fp, node);
    }

    return ast;
}
