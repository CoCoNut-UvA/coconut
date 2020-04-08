#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lib/imap.h"
#include "lib/memory.h"

imap_t *imap_init(int size) {
    imap_t *imap = (imap_t *)malloc(sizeof(imap_t));

    imap->size = size;
    imap->slots = (imap_entry_t **)calloc(sizeof(imap_entry_t *), size);

    return imap;
}

void imap_entry_free(imap_entry_t *entry) {
    if (!entry)
        return;

    imap_entry_free(entry->next);
    mem_free(entry);
}

void imap_free(imap_t *t) {
    for (int i = 0; i < t->size; i++)
        imap_entry_free(t->slots[i]);
    mem_free(t->slots);
    mem_free(t);
}

unsigned int imap_hash_fun(void *key) {
    /* The key is hashed using Thomas Wang's function
     * See: http://burtleburtle.net/bob/hash/integer.html
     */

    uintptr_t hash = (uintptr_t)key;

    hash += ~(hash << 15);
    hash ^= (hash >> 10);
    hash += (hash << 3);
    hash ^= (hash >> 6);
    hash += ~(hash << 11);
    hash ^= (hash >> 16);

    return hash;
}

unsigned int imap_hash(imap_t *t, void *key) {
    return imap_hash_fun(key) % t->size;
}

imap_entry_t *imap_entry_init(void *key, void *value) {
    imap_entry_t *entry = (imap_entry_t *)malloc(sizeof(imap_entry_t));

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    return entry;
}

void imap_insert(imap_t *t, void *key, void *value) {
    imap_entry_t *last;

    if (t->slots[imap_hash(t, key)]) {
        for (last = t->slots[imap_hash(t, key)]; last->next;
             last = last->next) {
            if (last->key == key) {
                last->value = value;
                return;
            }
        }

        last->next = imap_entry_init(key, value);
    } else
        t->slots[imap_hash(t, key)] = imap_entry_init(key, value);
}

void *imap_retrieve(imap_t *t, void *key) {
    if (t->slots[imap_hash(t, key)])
        for (imap_entry_t *last = t->slots[imap_hash(t, key)]; last;
             last = last->next)
            if (last->key == key)
                return last->value;

    return NULL;
}

void imap_map(imap_t *t, void *(f)(void *, void *)) {
    for (int i = 0; i < t->size; i++)
        for (imap_entry_t *last = t->slots[i]; last; last = last->next)
            f(last->key, last->value);
}
