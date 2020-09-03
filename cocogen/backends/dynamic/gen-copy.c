#include "ast/ast.h"

#include "format/out_macros.h"
#include "filehandling/gen-driver.h"

static
void GenInitArgs(struct ast *ast, FILE *fp, struct node *node)
{
    struct child *child = NULL;
    int arg_num = 0;
    int indent = 0;
    SLIST_FOREACH(child, node->children, next) {
        if (child->in_constructor) {
            if (arg_num > 0) {
                OUT(", ");
            }
            OUT("NULL");
            arg_num++;
        }
    }
    bool has_children = arg_num > 0;
    arg_num = 0;

    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, node->attributes, next) {
        if (attr->in_constructor) {
            if (has_children || arg_num > 0) {
                OUT(", ");
            }
            arg_num++;
            OUT("%s", ASTattributeDefaultValue(attr));
        }
    }
}

static
void GenCopyFunc(struct ast *ast, FILE *fp, struct node *node)
{
    int indent = 0;
    OUT_START_FUNC("struct ccn_node *CPY%s(struct ccn_node *arg_node)", node->name->lwr);
    OUT("struct ccn_node *new_node = ASTnew%s(", node->name->orig);
    GenInitArgs(ast, fp, node);
    OUT(");\n");

    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, node->attributes, next) {
        char *copyfunc = NULL;
        if (attr->type == AT_string) {
            copyfunc = "STRcpy";
        } else {
            OUT_FIELD("%s_%s(new_node) = %s_%s(arg_node)", node->name->upr,
                      attr->name->upr, node->name->upr, attr->name->upr);
            continue;
        }

        if (copyfunc != NULL) {
            OUT_FIELD("%s_%s(new_node) = %s(%s_%s(arg_node))", node->name->upr,
                attr->name->upr, copyfunc, node->name->upr, attr->name->upr);
        }
    }

    if (!SLIST_EMPTY(node->children)) {
        OUT_FIELD("arg_node = TRAVchildren(arg_node)");
    }
    OUT_FIELD("return new_node");
    OUT_END_FUNC();
}

struct ast *GCdoGen(struct ast *ast)
{
    FILE *fp = FHgetSourceFile("ccn_copy.c");
    int indent = 0;
    OUT("#include <stdlib.h>\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/str.h\"\n");
    OUT("\n");

    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        GenCopyFunc(ast, fp, node);
    }

    return ast;
}