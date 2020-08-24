/**
 * PREFIX: HT
 */

#include "palm/hash_table.h"


#include "palm/memory.h"
#include "palm/str.h"
#include "palm/dbug.h"

struct htable_entry {
    void *key;
    void *value;
    struct htable_entry *next;
};

struct htable {
    struct htable_entry **entries;
    hash_key_ft hash_f;
    is_equal_ft is_equal_f;
    cpy_key_ft cpy_key_f;
    free_key_ft free_key_f;
    size_t size;
    size_t elements;
};


struct htable *HTnew(size_t size, hash_key_ft hash_func, is_equal_ft is_equal_func, cpy_key_ft cpy_key, free_key_ft free_key)
{
    struct htable *table = MEMmalloc(sizeof(struct htable));
    table->size = size;
    table->elements = 0;
    table->entries = calloc(sizeof(struct htable_entry *), size);
    table->hash_f = hash_func;
    table->is_equal_f = is_equal_func;
    table->cpy_key_f = cpy_key;
    table->free_key_f = free_key;

    return table;
}

static
struct htable_entry *NewEntry(struct htable *table, void *key, void *value)
{
    struct htable_entry *entry = MEMmalloc(sizeof(struct htable_entry));
    entry->key = table->cpy_key_f ? table->cpy_key_f(key) : key;

    entry->value = value;
    entry->next = NULL;

    return entry;
}

static
void DeleteEntry(struct htable *table, struct htable_entry *entry)
{
    if (table->cpy_key_f) {
        DBUG_ASSERT(table->free_key_f, "Hash table copied keys but does not now how to free them.");
        table->free_key_f(entry->key);
    }
    entry = MEMfree(entry);
}

bool Insert(struct htable *table, void *key, void *value)
{
    struct htable_entry **target = NULL;

    size_t index = table->hash_f(key) % table->size;

    if (!table->entries[index]) {
        target = &(table->entries[index]);
    } else {
        struct htable_entry *last = table->entries[index];
        for (; last->next; last = last->next) {
            if (table->is_equal_f(last->key, key)) {
                return false;
            }
        }
        if (table->is_equal_f(last->key, key)) {
            return false;
        }

        target = &(last->next);
    }

    *target = NewEntry(table, key, value);
    table->elements++;

    return true;

}

bool HTinsert(struct htable *table, void *key, void *value)
{
    return Insert(table, key, value);
}

void *HTremove(htable_st *table, void *key)
{
    size_t index = table->hash_f(key) % table->size;

    void *value = NULL;
    struct htable_entry *previous = NULL;
    struct htable_entry *entry = table->entries[index];

    if (entry && table->is_equal_f(entry->key, key)) {
        value = entry->value;
        table->entries[index] = entry->next;
        DeleteEntry(table, entry);
        table->elements--;
        entry = NULL;
        return value;
    }

    previous = entry;
    entry = entry->next;

    while (entry) {
        if (table->is_equal_f(entry->key, key)) {
            value = entry->value;
            previous->next = entry->next;
            DeleteEntry(table, entry);
            table->elements--;
            entry = NULL;
            return value;
        }

        previous = entry;
        entry = entry->next;
    }

    return NULL;
}


void *HTlookup(struct htable *table, void *key, bool *found)
{
    size_t index = table->hash_f(key) % table->size;
    if (table->entries[index]) {
        struct htable_entry *last = table->entries[index];
        for (; last; last = last->next) {
            if (table->is_equal_f(last->key, key)) {
                *found = true;
                return last->value;
            }
        }
    }

    *found = false;
    return NULL;
}


void HTdelete(struct htable *table)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            struct htable_entry *next = entry->next;
            DeleteEntry(table, entry);
            entry = next;
        }
    }
    MEMfree(table->entries);
    MEMfree(table);
}

size_t HTelementCount(htable_st *table)
{
    return table->elements;
}

void HTfold(htable_st *table, fold_ft fun, void *init_acc)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            fun(init_acc, entry->value);
            entry = entry->next;
        }
    }
}


void HTmap(struct htable *table, map_ft fun)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            entry->value = fun(entry->value);
            entry = entry->next;
        }
    }
}




/* Implementations */

static
hash_key_vt StringHash(char *key)
{
    /* This is the djb2 algorithm as found for hashing strings.
     * Source: http://www.cse.yorku.ca/~oz/hash.html
     */

    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

hash_key_vt HThash_String(char *key)
{
    return StringHash(key);
}

struct htable *HTnew_String(size_t size)
{
    return HTnew(size, (hash_key_ft)StringHash, (is_equal_ft)STReq, (cpy_key_ft)STRcpy, (free_key_ft)MEMfree);
}

static
hash_key_vt PtrHash(void *ptr)
{
    hash_key_vt hash_key = (((hash_key_vt) ptr >> 5) & 0x1f);

    return hash_key;
}

static
bool PtrEqual(void *ptr1, void *ptr2)
{
    return ptr1 == ptr2;
}

struct htable *HTnew_Ptr(size_t size)
{
    return HTnew(size, (hash_key_ft)PtrHash, (is_equal_ft)PtrEqual, NULL, NULL);
}

static
int *IntCpy(int *val)
{
    int *cpy = MEMmalloc(sizeof(int));
    *cpy = *val;
}

static
hash_key_vt IntHash(int *val)
{
    return (hash_key_vt)(*val);
}

static
bool IntEqual(int *ptr1, int *ptr2)
{
    return *ptr1 == *ptr2;
}

struct htable *HTnew_Int(size_t size)
{
    return HTnew(size, (hash_key_ft)IntHash, (is_equal_ft)IntEqual, (cpy_key_ft)IntCpy, (free_key_ft)MEMfree);
}