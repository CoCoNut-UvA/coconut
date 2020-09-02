#include "ast/ast.h"

#include "sys/queue.h"

#include "palm/memory.h"
#include "palm/dbug.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/set.h"

unsigned long ASTnumOfChildren(struct node *node)
{
    unsigned long children = 0;
    struct child *child = NULL;
    SLIST_FOREACH(child, node->children, next) {
        children++;
    }

    return children;
}

char *ASTattributeTypeToString(struct attribute *attr)
{
    char *link_type;

    switch (attr->type) {
    case AT_int:
        return "int";
    case AT_uint:
        return "unsigned int";
    case AT_int8:
        return "int8_t";
    case AT_int16:
        return "int16_t";
    case AT_int32:
        return "int32_t";
    case AT_int64:
        return "int64_t";
    case AT_uint8:
        return "uint8_t";
    case AT_uint16:
        return "uint16_t";
    case AT_uint32:
        return "uint32_t";
    case AT_uint64:
        return "uint64_t";
    case AT_float:
        return "float";
    case AT_double:
        return "double";
    case AT_bool:
        return "bool";
    case AT_string:
        return "char *";
    case AT_link_or_enum:
    case AT_enum:
        link_type = MEMmalloc(STRlen(attr->type_reference->orig + 6));
        sprintf(link_type, "enum %s", attr->type_reference->orig);
        return link_type;
    case AT_link:
        link_type = MEMmalloc(STRlen(attr->type_reference->orig) + 10);
        sprintf(link_type, "struct %s *", attr->type_reference->orig);
        return link_type;
    }
    DBUG_ASSERT(false, "No string for attribute type");
    return NULL;
}


char *ASTattributeDefaultValue(struct attribute *attribute) {
    switch (attribute->type) {
    case AT_int:
    case AT_int8:
    case AT_int16:
    case AT_int32:
    case AT_int64:
    case AT_enum:
    case AT_uint:
    case AT_uint8:
    case AT_uint16:
    case AT_uint32:
    case AT_uint64:
        return "0";
        break;
    case AT_float:
    case AT_double:
        return "0.0";
        break;
    case AT_bool:
        return "false";
        break;
    case AT_string:
    case AT_link:
        return "NULL";
        break;
    case AT_link_or_enum:
        DBUG_ASSERT(false, "Attribute type is not converted to link or enum");
        return "NULL";
        break;
    }
}

static
char *Key_ID(struct id *id)
{
    return STRcpy(id->orig);
}


set_st *ASTnewSet_Id(size_t size)
{
    return SETnew(size, (set_key_ft)Key_ID);
}


size_t ASTnumOfTraversals(struct ast *ast)
{
    DBUG_ASSERT((ast->traversals != NULL), "Num of traversals on NULL is undefined");

    size_t len = 0;
    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        len++;
    }

    return len;
}

size_t ASTnumOfNodes(struct ast *ast)
{
    DBUG_ASSERT(ast->nodes != NULL, "Num of nodes on NULL is undefined");

    size_t len = 0;
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        len++;
    }

    return len;

}
