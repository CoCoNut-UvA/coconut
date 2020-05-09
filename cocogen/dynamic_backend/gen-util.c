#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/genmacros.h"
#include "gen-functions.h"

char *strupr(char *string) {
    char *upper = malloc((strlen(string) + 1) * sizeof(char));
    strcpy(upper, string);
    for (int i = 0; i < strlen(upper); ++i) {
        upper[i] = toupper(upper[i]);
    }
    return upper;
}

char *strlwr(char *string) {
    char *lower = malloc((strlen(string) + 1) * sizeof(char));
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
bool is_traversal_node(Traversal *trav, Node *node) {
    for (int i = 0; i < array_size(trav->nodes); i++) {
        Node *travnode = array_get(trav->nodes, i);
        if (strcmp(travnode->id, node->id) == 0) {
            return true;
        }
    }
    return false;
}