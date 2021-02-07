/**
 * This exposes some helper functions to work with the symbol table entries
 * desrcibed in the AST.
 *
 * Since this is a node, the free and copy functions can be used.
 */
#include "symboltable.h"
#include <stddef.h>

#include "palm/dbug.h"
#include "palm/str.h"

/**
 * Add a key/val pair to an existing symbol table by creating a new entry.
 * This returns a new symbol table entry, which is the start of the new symbol
 * table. Hence, an assignment on add is required.
 * Important: This does not check if a key is already present, instead it will
 * shadow the already existing key(Simulating an update).
 * @param ste The existing symbol table
 * @param key The key to add
 * @param val The value to add.
 * @return A new symbol table with the key/val pair added.
 */
node_st *STadd(node_st *ste, node_st *key, node_st *val)
{
    node_st *new = ASTste();
    STE_KEY(new) = key;
    STE_VALUE(new) = val;
    if (ste != NULL) {
        STE_NEXT(new) = ste;
    }
    return new;
}

/**
 *
 * @param ste  Symbol table
 * @param node Key to find.
 * @return The value corresponding to the key or NULL if not found.
 */
node_st *STlookup(node_st *ste, node_st *node)
{
    DBUG_ASSERT(NODE_TYPE(node) == NT_ID, "Keys can only be IDs");
    if (node == NULL) {
        return NULL;
    }
    node_st *curr = ste;
    while (curr) {
        if (STReq(ID_LWR(STE_KEY(curr)), ID_LWR(node))) {
            return STE_VALUE(curr);
        }
        curr = STE_NEXT(curr);
    }
    return NULL;
}


