#include "palm/set.h"

#include <stdio.h>

#include "palm/hash_table.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "palm/dbug.h"


typedef char *(*set_key_ft)(void *);
typedef void *(*cpy_val_ft)(const void *);

struct set {
    htable_st *table;
    set_key_ft key_f;
    cpy_val_ft cpy_val_f;
};


struct set *SETnew(size_t size, set_key_ft key_f)
{
    struct set *s = MEMmalloc(sizeof(struct set));
    s->table = HTnew_String(size);
    s->key_f = key_f;

    return s;
}

void SETdelete(struct set *set)
{
    HTdelete(set->table);
    set = MEMfree(set);
}

bool SETinsert(struct set *set, void *item)
{
    char *key = set->key_f(item);

    bool res = HTinsert(set->table, key, item);
    MEMfree(key);

    return res;
}

void SETremove(struct set *set, void *item)
{
    char *key = set->key_f(item);
    HTremove(set->table, key);
}


bool SETisMember(struct set *set, void *item)
{
    char *key = set->key_f(item);

    bool is_member;
    HTlookup(set->table, key, &is_member);
    MEMfree(key);

    return is_member;
}

size_t SETcardinality(struct set *set)
{
    return HTelementCount(set->table);
}

void SETfold(struct set *set, fold_ft fun, void *init_acc)
{
    DBUG_ASSERT(set, "SET IS NULL");
    HTfold(set->table, fun, init_acc);
}

static
void CpyHelper(struct set *set, void *value)
{
    SETinsert(set, value);
}

struct set *SETcpy(struct set *set)
{
    struct set *new = SETnew(SETcardinality(set), set->key_f);
    SETfold(set, (fold_ft)CpyHelper, new);

    return new;
}

struct intersect_wrapper {
    struct set *set2;
    struct set *dest;
};

static
void IntersectHelper(struct intersect_wrapper *wrapper, void *item)
{
    if (SETisMember(wrapper->set2, item)) {
        SETinsert(wrapper->dest, item);
    }
}

struct set *SETintersect(struct set *set1, struct set *set2)
{
    DBUG_ASSERT((set1->key_f == set2->key_f), "Intersect on different types of sets");
    struct set *new = SETnew(SETcardinality(set1), set1->key_f);
    struct intersect_wrapper wrapper;
    wrapper.set2 = set2;
    wrapper.dest = new;
    SETfold(set1, (fold_ft)IntersectHelper, &wrapper);

    return new;
}

struct set *SETunion(struct set *set1, struct set *set2)
{
    DBUG_ASSERT((set1->key_f == set2->key_f), "Union on different types of sets");
    struct set *new = SETcpy(set1);
    SETfold(set2, (fold_ft)CpyHelper, new);

    return new;
}

static
void DifferenceHelper(struct intersect_wrapper *wrapper, void *item)
{
    if (!SETisMember(wrapper->set2, item)) {
        SETinsert(wrapper->dest, item);
    }
}

struct set *SETdifference(struct set *set1, struct set *set2)
{
    DBUG_ASSERT((set1->key_f == set2->key_f), "Difference on different types of sets");
    struct set *new = SETnew(SETcardinality(set1), set1->key_f);
    struct intersect_wrapper wrapper;
    wrapper.set2 = set2;
    wrapper.dest = new;
    SETfold(set1, (fold_ft)DifferenceHelper, &wrapper);

    return new;
}


struct set *SETnew_String(size_t size)
{
    return SETnew(size, (set_key_ft)STRcpy);
}

void PrintHelper_String(struct set *st, char *item)
{
    printf("%s, ", item);
}

void SETprint_String(struct set *set)
{
    printf("{ ");
    SETfold(set, (fold_ft)PrintHelper_String, NULL);
    printf("}\n");
}