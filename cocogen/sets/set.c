/**
 * @file
 *
 * This module provides a set of identifiers and uses corresponding
 * traversals for operations on sets. Literals are copied on insertions.
 */

#include "set.h"
#define MODULE "SET"
#include "palm/dbug.h"

static node_st *DoInsert(node_st *literal, node_st *id)
{
    if (literal == NULL) {
        return ASTsetliteral(TRAVstart(id, TRAV_cpy));
    }
    DBUG("Inserting: %s\n", ID_LWR(id));

    TRAVpush(TRAV_SLI);
    struct data_sli *data = DATA_SLI_GET();
    data->value = TRAVstart(id, TRAV_cpy);
    TRAVdo(literal);
    if (!data->inserted) {
        TRAVstart(data->value, TRAV_free);
    }
    TRAVpop();

    return literal;
}

/**
 * @brief Insert an identifier into the set.
 * @param literal  The set to insert into.
 * @param id The id to insert.
 * @return a new set containing the id.
 */
node_st *SETIDinsert(node_st *literal, node_st *id)
{
    return DoInsert(literal, id);
}

/**
 * @param literal The set literal
 * @param id id to search for in set.
 * @return true if id in set, else false.
 */
bool SETIDcontains(node_st *literal, node_st *id)
{
    if (literal == NULL) {
        return false;
    }
    bool found = false;
    printf("Checking: %s\n", ID_LWR(id));

    TRAVpush(TRAV_SLC);
    {
        struct data_slc *data = DATA_SLC_GET();
        data->lookup = id;
        TRAVdo(literal);
        if (data->contains) {
            found = true;
        }
    } TRAVpop();
    printf("Found: %d\n", found);
    return found;
}


node_st *SETIDdifference(node_st *left, node_st *right)
{
    if (left == NULL) {
        return NULL;
    }

    node_st *new = NULL;
    TRAVpush(TRAV_SLD);
    {
        struct data_sld *data = DATA_SLD_GET();
        data->right = right;
        TRAVdo(left);
        new = data->new;
    } TRAVpop();

    return new;
}

node_st *SETIDintersect(node_st *dst, node_st *src)
{
    if (dst == NULL) {
        return NULL;
    }
    node_st *new = NULL;
    TRAVpush(TRAV_SLIS);
    {
        struct data_slis *data = DATA_SLIS_GET();
        data->src = src;
        TRAVdo(dst);
        new = data->new;
    } TRAVpop();

    return new;
}

node_st *SETIDunion(node_st *set1, node_st *set2)
{
    node_st *new = NULL;
    TRAVpush(TRAV_SLU);
    {
        struct data_slu *data = DATA_SLU_GET();
        TRAVopt(set1);
        TRAVopt(set2);
        new = data->dst;
    } TRAVpop();

    return new;
}
