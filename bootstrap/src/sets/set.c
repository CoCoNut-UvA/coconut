#include <assert.h>
#include "set.h"


static node_st *DoInsert(node_st *literal, node_st *id)
{
    if (literal == NULL) {
        return ASTsetliteral(TRAVstart(id, TRAV_cpy));
    }
    printf("Inserting: %s\n", ID_LWR(id));

    TRAVpush(TRAV_SETLITERALINSERT);
    struct trav_data_setliteralinsert *data = SETLITERALINSERT_DATA;
    data->value = TRAVstart(id, TRAV_cpy);
    TRAVdo(literal);
    if (!data->inserted) {
        TRAVstart(data->value, TRAV_free);
    }
    TRAVpop();

    return literal;
}

node_st *SETIDinsert(node_st *literal, node_st *id)
{
    return DoInsert(literal, id);
}

bool SETIDcontains(node_st *literal, node_st *id)
{
    if (literal == NULL) {
        return false;
    }
    bool found = false;
    printf("Checking: %s\n", ID_LWR(id));

    TRAVpush(TRAV_SETLITERALCONTAINS);
    {
        struct trav_data_setliteralcontains *data = SETLITERALCONTAINS_DATA;
        data->lookup = id;
        TRAVdo(literal);
        if (data->contains) {
            found = true;
        }
    } TRAVpop();
    printf("Found: %d\n", found);
    return found;
}

node_st *SETIDintersect(node_st *dst, node_st *src)
{
    if (dst == NULL) {
        return NULL;
    }
    node_st *new = NULL;
    TRAVpush(TRAV_SETLITERALINTERSECT);
    {
        struct trav_data_setliteralintersect *data = SETLITERALINTERSECT_DATA;
        data->src = src;
        TRAVdo(dst);
        new = data->new;
    } TRAVpop();

    return new;
}

node_st *SETIDunion(node_st *set1, node_st *set2)
{
    node_st *new = NULL;
    TRAVpush(TRAV_SETLITERALUNION);
    {
        struct trav_data_setliteralunion *data = SETLITERALUNION_DATA;
        TRAVopt(set1);
        TRAVopt(set2);
        new = data->dst;
    } TRAVpop();

    return new;
}
