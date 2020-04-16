#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/genmacros.h"

#include "filegen/gen-util.h"

/** This is terrible but this is the frontend's fault. It is necessary because
 * we want to address nodes dynamically. The current frontend addresses links
 * with their respective "static" struct declaration, which is not what we want.
 * Instead, the resulting type should become "node *".
 */
bool type_is_link(Config *config, Attr *attr) {
    bool islink = false;
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        if (strcmp(attr->type_id, node->id) == 0) {
            islink = true;
            break;
        }
    }
    return islink;
}

char *get_attr_str(Config *config, Attr *attr) {
    char *type_str = NULL;
    switch (attr->type) {
    case AT_int:
        type_str = "int";
        break;
    case AT_uint:
        type_str = "uint";
        break;
    case AT_int8:
        type_str = "int8";
        break;
    case AT_int16:
        type_str = "int16";
        break;
    case AT_int32:
        type_str = "int32";
        break;
    case AT_int64:
        type_str = "int64";
        break;
    case AT_uint8:
        type_str = "uint8";
        break;
    case AT_uint16:
        type_str = "int8";
        break;
    case AT_uint32:
        type_str = "int32";
        break;
    case AT_uint64:
        type_str = "int64";
        break;
    case AT_float:
        type_str = "float";
        break;
    case AT_double:
        type_str = "double";
        break;
    case AT_bool:
        type_str = "bool";
        break;
    case AT_string:
        type_str = "char*";
        break;
    case AT_link_or_enum:
        if (type_is_link(config, attr)) {
            type_str = "Node*";
        } else {
            type_str = attr->type_id;
        }
        break;
    case AT_link:
        type_str = "Node*";
        break;
    case AT_enum:
        type_str = attr->type_id;
        break;
    }

    return type_str;
}

char *strupr(char *string) {
    char *upper = malloc(strlen(string) * sizeof(char));
    strcpy(upper, string);
    for (int i = 0; i < strlen(upper); ++i) {
        upper[i] = toupper(upper[i]);
    }
    return upper;
}

char *strlwr(char *string) {
    char *lower = malloc(strlen(string) * sizeof(char));
    strcpy(lower, string);
    for (int i = 0; i < strlen(lower); ++i) {
        lower[i] = tolower(lower[i]);
    }
    return lower;
}

/**
 * This this function goes over each node in the traversal and compares it to
 * the node given to the function. If the two nodes have an equal ID, then the
 * node is a valid node in the traversal.
 */
bool is_traversal_node(Config *config, FILE *fp, Traversal *trav, Node *node) {
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *travnode = array_get(trav->nodes, i);
        if (strcmp(travnode->id, node->id) == 0) {
            return true;
        }
    }
    return false;
}

/**
 */
bool is_pass_node(Config *config, FILE *fp, Traversal *trav, Node *node) {
    for (int i = 0; i < array_size(trav->pass_nodes); i++) {
        Node *travnode = array_get(trav->pass_nodes, i);
        if (strcmp(travnode->id, node->id) == 0) {
            return true;
        }
    }
    return false;
}