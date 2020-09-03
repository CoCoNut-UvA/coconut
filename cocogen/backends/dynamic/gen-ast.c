
#include "sys/queue.h"

#include "ast/ast.h"
#include "format/format.h"
#include "globals.h"
#include "filehandling/gen-driver.h"

static
void GenNodeStruct(struct format_settings *settings, struct node *node, FILE *fp)
{
    int indent = 0;
    OUT_STRUCT("NODE_DATA_%s", node->name->upr);
    {
        if (node->children) {
            OUT_UNION("NODE_CHILDREN_%s", node->name->upr);
            OUT_STRUCT("NODE_CHILDREN_%s_STRUCT", node->name->upr);
            struct child *child;
            SLIST_FOREACH(child, node->children, next) {
                OUT_FIELD("%s *%s", settings->basic_node_type, child->name->lwr);
            }
            OUT_TYPEDEF_STRUCT_END("%s_children_st", node->name->lwr);
            OUT_FIELD("%s *%s_children_at[%ld]", settings->basic_node_type, node->name->lwr, ASTnumOfChildren(node));
            OUT_TYPEDEF_STRUCT_END("%s_children", node->name->lwr);
        }

        if (node->attributes) {
            struct attribute *attribute = NULL;
            SLIST_FOREACH(attribute, node->attributes, next) {
                if (attribute->type == AT_link) {
                    OUT_FIELD("%s *%s", settings->basic_node_type, attribute->name->lwr);
                } else {
                    OUT_FIELD("%s %s", ASTattributeTypeToString(attribute), attribute->name->lwr);
                }
            }
        }
    }
    OUT_STRUCT_END();
}

static
void GenNodesUnion(struct format_settings *settings, struct ast *ast, FILE *fp)
{
    int indent = 0;
    OUT_COMMENT("Attributes");
    OUT_UNION("NODE_DATA");

    struct node *node;
    STAILQ_FOREACH(node, ast->nodes, next) {
        OUT_FIELD("struct NODE_DATA_%s *N_%s", node->name->upr, node->name->lwr);
    }
    OUT_STRUCT_END();
}

static
void GenNodeMacros(struct format_settings *settings, struct node *node, FILE *fp)
{
    int indent = 0;

    if (node->children) {
        struct child *child;
        SLIST_FOREACH(child, node->children, next) {
            OUT("#define ");
            if (settings->access_macro_uppercase) {
                OUT("%s_%s(n) ", node->name->upr, child->name->upr);
            } else {
                OUT("%s_%s(n) ", node->name->lwr, child->name->lwr);
            }
            OUT("((n)->data.N_%s->%s_children.%s_children_st.%s)\n", node->name->lwr, node->name->lwr, node->name->lwr, child->name->lwr);
        }
    }

    if (node->attributes) {
        struct attribute *attribute;
        SLIST_FOREACH(attribute, node->attributes, next) {
            OUT("#define ");
            if (settings->access_macro_uppercase) {
                OUT("%s_%s(n) ", node->name->upr, attribute->name->upr);
            } else {
                OUT("%s_%s(n) ", node->name->lwr, attribute->name->lwr);
            }
            OUT("((n)->data.N_%s->%s)\n", node->name->lwr, attribute->name->lwr);
        }
    }
    OUT("\n");
}


void GenInitFunction(struct format_settings *settings, struct node *node, FILE *fp) {
    int indent = 0;
    OUT("%s *%s%s(", settings->basic_node_type, settings->node_constructor_prefix, node->name->orig);

    int arg = 0;
    if (node->children) {
        struct child *child = NULL;
        SLIST_FOREACH(child, node->children, next) {
            if (child->in_constructor) {
                if (arg > 0) {
                    OUT(", ");
                }
                arg++;
                OUT("%s *%s", settings->basic_node_type, child->name->lwr);
            }
        }
    }

    if (node->attributes) {
        struct attribute *attribute = NULL;
        SLIST_FOREACH(attribute, node->attributes, next) {
            if (attribute->in_constructor) {
                if (arg > 0) {
                    OUT(", ");
                }
                arg++;
                if (attribute->type == AT_link) {
                    OUT("%s *%s", settings->basic_node_type, attribute->name->lwr);
                } else {
                    OUT("%s %s", ASTattributeTypeToString(attribute), attribute->name->lwr);
                }
            }
        }
    }
    OUT(")");
}


void GenNodeMembers(struct format_settings *settings, struct node *node, FILE *fp, int indent)
{
    if (node->children) {
        struct child *child = NULL;
        SLIST_FOREACH(child, node->children, next) {
            if (child->in_constructor) {
                if (settings->access_macro_uppercase) {
                    OUT_FIELD("%s_%s(node) = %s", node->name->upr, child->name->upr, child->name->lwr);
                } else {
                    OUT_FIELD("%s_%s(node) = %s", node->name->lwr, child->name->lwr, child->name->lwr);
                }
            } else {
                if (settings->access_macro_uppercase) {
                    OUT_FIELD("%s_%s(node) = NULL", node->name->upr, child->name->upr);
                } else {
                    OUT_FIELD("%s_%s(node) = NULL", node->name->lwr, child->name->lwr);
                }
            }
        }
    }

    if (node->attributes) {
        struct attribute *attribute = NULL;
        SLIST_FOREACH(attribute, node->attributes, next) {
            if (attribute->in_constructor) {
                if (settings->access_macro_uppercase) {
                    OUT_FIELD("%s_%s(node) = %s", node->name->upr, attribute->name->upr, attribute->name->lwr);
                } else {
                    OUT_FIELD("%s_%s(node) = %s", node->name->lwr, attribute->name->lwr, attribute->name->lwr);
                }
            } else {
                if (settings->access_macro_uppercase) {
                    OUT_FIELD("%s_%s(node) = %s", node->name->upr, attribute->name->upr, ASTattributeDefaultValue(attribute));
                } else {
                    OUT_FIELD("%s_%s(node) = %s", node->name->lwr, attribute->name->lwr, ASTattributeDefaultValue(attribute));
                }
            }
        }
    }
}

void GenNodeConstructor(struct format_settings *settings, struct node *node, FILE *fp) {
    int indent = 0;
    GenInitFunction(settings, node, fp);
    OUT_START_FUNC_FIELD();
    OUT_FIELD("%s *node = NewNode()", settings->basic_node_type);
    OUT_FIELD("node->data.N_%s = MEMmalloc(sizeof(struct NODE_DATA_%s))", node->name->lwr, node->name->upr);
    OUT_FIELD("NODE_TYPE(node) = %s%s", settings->nodetype_enum_prefix, node->name->upr);
    if (node->children) {
        OUT_FIELD("NODE_CHILDREN(node) = node->data.N_%s->%s_children.%s_children_at", node->name->lwr, node->name->lwr, node->name->lwr);
        OUT_FIELD("NODE_NUMCHILDREN(node) = %ld", ASTnumOfChildren(node));
    }
    GenNodeMembers(settings, node, fp, indent);
    OUT_FIELD("return node");
    OUT_END_FUNC();
}

static
void GenBaseNodeInit(struct format_settings *settings, FILE *fp)
{
    int indent = 0;
    OUT_START_FUNC("%s *NewNode()", settings->basic_node_type);
    OUT_FIELD("%s *node = MEMmalloc(sizeof(%s))", settings->basic_node_type, settings->basic_node_type);
    OUT_FIELD("NODE_TYPE(node) = NT_NULL");
    OUT_FIELD("NODE_CHILDREN(node) = NULL");
    OUT_FIELD("NODE_NUMCHILDREN(node) = 0");
    OUT_FIELD("return node");
    OUT_END_FUNC();
}

static
void GenBaseNode(struct format_settings *settings, FILE *fp)
{
    int indent = 0;
    OUT("#define NODE_TYPE(n) ((n)->nodetype)\n");
    OUT("#define NODE_CHILDREN(n) ((n)->children)\n");
    OUT("#define NODE_NUMCHILDREN(n) ((n)->num_children)\n");
    OUT_STRUCT("ccn_node");
    OUT_FIELD("enum ccn_nodetype nodetype");
    OUT_FIELD("union NODE_DATA data");
    OUT_FIELD("struct ccn_node **children");
    OUT_FIELD("long int num_children");
    OUT_STRUCT_END();
}


struct ast *GASTdoGen(struct ast *ast)
{
    struct format_settings *settings = globals.settings;
    struct node *node;

    FILE *fp = FHgetIncludeFile("ast.h");

    int indent = 0;
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("typedef struct ccn_node %s;\n", settings->basic_node_type);
    STAILQ_FOREACH(node, ast->nodes, next) {
        GenNodeStruct(settings, node, fp);
        GenNodeMacros(settings, node, fp);
        GenInitFunction(settings, node, fp);
        OUT(";\n");
    }

    GenNodesUnion(settings, ast, fp);
    GenBaseNode(settings, fp);

    fp = FHgetSourceFile("ast.c");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/memory.h\"\n");

    GenBaseNodeInit(settings, fp);

    STAILQ_FOREACH(node, ast->nodes, next) {
        GenNodeConstructor(settings, node, fp);
    }

    return ast;
}