/**
 * PREFIX: HT
 */

#include <assert.h>
#include <stdio.h>

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
    size_t size;
    size_t elements;
};

struct htable_iter {
    htable_st *table;
    size_t current_element;
    struct htable_entry *current_entry;
};

/**
 * Create a new hash table.
 * @param size the number of buckets in a hash table.
 *  More buckets is more space, but les collisions.
 *  See pre-defined hash tables in the bottom of this file.
 * @param hash_func the function used for hashing keys.
 * @param is_equal_func the function to determine if two values are equal.
 */
struct htable *HTnew(size_t size, hash_key_ft hash_func, is_equal_ft is_equal_func)
{
    struct htable *table = MEMmalloc(sizeof(struct htable));
    table->size = size;
    table->elements = 0;
    table->entries = calloc(sizeof(struct htable_entry *), size);
    table->hash_f = hash_func;
    table->is_equal_f = is_equal_func;

    return table;
}

struct htable *HTcpy(struct htable *table)
{
    struct htable *new_table = HTnew(table->size, table->hash_f,
                                     table->is_equal_f);

    for (htable_iter_st *iter = HTiterate(table); iter;
         iter = HTiterateNext(iter)) {
        HTinsert(new_table, HTiterKey(iter), HTiterValue(iter));
    }

    return new_table;
}

struct htable *HTdeep_copy(struct htable *table, cpy_ft key_copy_func,
                           cpy_ft val_copy_func)
{
    struct htable *new_table = HTnew(table->size, table->hash_f,
                                     table->is_equal_f);

    for (htable_iter_st *iter = HTiterate(table); iter;
         iter = HTiterateNext(iter)) {
        HTinsert(new_table, key_copy_func(HTiterKey(iter)),
                            val_copy_func(HTiterValue(iter)));
    }

    return new_table;
}

static
struct htable_entry *NewEntry(struct htable *table, void *key, void *value)
{
    struct htable_entry *entry = MEMmalloc(sizeof(struct htable_entry));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    return entry;
}

static
void DeleteEntry(struct htable *table, struct htable_entry *entry)
{
    MEMfree(entry);
}

static
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
                last->value = value;
                return true;
            }
        }
        if (table->is_equal_f(last->key, key)) {
            last->value = value;
            return true;
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

/**
 * Remove the key, value pair from the table.
 *
 * @return the deleted value or NULL if key is not foound.
 */
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

/**
 * @return value found or NULL if key is not present.
 */
void *HTlookup(struct htable *table, void *key)
{
    size_t index = table->hash_f(key) % table->size;
    if (table->entries[index]) {
        struct htable_entry *last = table->entries[index];
        for (; last; last = last->next) {
            if (table->is_equal_f(last->key, key)) {
                return last->value;
            }
        }
    }
    return NULL;
}

/**
 * Clears all entries in the hash table.
 * Does not delete the table, so can be reused.
 */
void HTclear(struct htable *table)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            struct htable_entry *next = entry->next;
            DeleteEntry(table, entry);
            entry = next;
        }
        table->entries[i] = NULL;
    }
}

/**
 * Delete all entries in the table and the table itself.
 * Table is not usable after this operation.
 */
void HTdelete(struct htable *table)
{
    HTclear(table);
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

void HTmapWithKey(struct htable *table, mapk_ft fun)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            entry->value = fun(entry->key, entry->value);
            entry = entry->next;
        }
    }
}

/**
 * Map function that passes an extra data parameter to the
 * map function
 */
void HTmapWithDataAndKey(htable_st *table, void *data, mapdk_ft fun)
{
    for (size_t i = 0; i < table->size; i++) {
        struct htable_entry *entry = table->entries[i];
        while (entry) {
            entry->value = fun(data, entry->key, entry->value);
            entry = entry->next;
        }
    }
}

/* Iteration functions
   See palm/hash_table.h for example usage */

/* Create new iterator */
htable_iter_st *HTiterate(htable_st *table) {
    if (table->elements == 0) {
        return NULL;
    }

    htable_iter_st *iterator = MEMmalloc(sizeof(htable_iter_st));
    iterator->table = table;
    iterator->current_entry = NULL;
    for (size_t i = 0; i < table->size; i++) {
        if (table->entries[i]) {
            iterator->current_entry = table->entries[i];
            iterator->current_element = i;
            break;
        }
    }

    // Should have failed at table->elements check
    assert(iterator->current_entry != NULL);
    return iterator;
}

/* Iterate to next element. Destroys iterator if there are no more elements */
htable_iter_st *HTiterateNext(htable_iter_st *iter) {
    struct htable_entry *next = iter->current_entry->next;
    if (next != NULL) {
        iter->current_entry = next;
        return iter;
    }

    for (size_t i = iter->current_element + 1; i < iter->table->size; ++i) {
        if (iter->table->entries[i] != NULL) {
            iter->current_entry = iter->table->entries[i];
            iter->current_element = i;
            return iter;
        }
    }

    // No more elements
    MEMfree(iter);
    return NULL;
}

/* Destroy iterator early */
void HTiterateCancel(htable_iter_st *iter) {
    MEMfree(iter);
}

/* Get iterator value */
void *HTiterValue(htable_iter_st *iter) {
    assert(iter->current_entry != NULL);
    return iter->current_entry->value;
}

/* Set iterator value */
void HTiterSetValue(htable_iter_st *iter, void *value) {
    assert(iter->current_entry != NULL);
    iter->current_entry->value = value;
}

/* Get iterator key */
void *HTiterKey(htable_iter_st *iter) {
    assert(iter->current_entry != NULL);
    return iter->current_entry->key;
}


/* Implementations */

/* String char * -> void * */
static
size_t StringHash(char *key)
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

/**
 * Create a new hash table for strings.
 * @param size determines the number of buckets in the table.
 */
struct htable *HTnew_String(size_t size)
{
    return HTnew(size, (hash_key_ft)StringHash, (is_equal_ft)STReq);
}

//Ptr htable: void * -> void *
static
size_t PtrHash(void *ptr)
{
    size_t hash_key = (((size_t) ptr >> 5) & 0x1f);

    return hash_key;
}

static
bool PtrEqual(void *ptr1, void *ptr2)
{
    return ptr1 == ptr2;
}

/**
 * Create a new hash table for pointers.
 * @param size determines the number of buckets in the table.
 */
struct htable *HTnew_Ptr(size_t size)
{
    return HTnew(size, (hash_key_ft)PtrHash, (is_equal_ft)PtrEqual);
}

// Int htable: int * -> void *;
static
size_t IntHash(int *val)
{
    return (size_t)(*val);
}

static
bool IntEqual(int *ptr1, int *ptr2)
{
    return *ptr1 == *ptr2;
}

struct htable *HTnew_Int(size_t size)
{
    return HTnew(size, (hash_key_ft)IntHash, (is_equal_ft)IntEqual);
}
