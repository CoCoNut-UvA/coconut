#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/array.h"
#include "lib/memory.h"
#include "lib/smap.h"

// TODO: improve cleanup of hash map.

smap_t *smap_init(size_t size) {
    smap_t *smap = (smap_t *)malloc(sizeof(smap_t));

    smap->size = size;
    smap->slots = (smap_entry_t **)calloc(sizeof(smap_entry_t *), size);

    return smap;
}

void smap_entry_free(smap_entry_t *entry) {
    if (!entry)
        return;

    smap_entry_free(entry->next);
    mem_free(entry->key);
    mem_free(entry);
}

void smap_free(smap_t *t) {
    if (t == NULL)
        return;
    for (int i = 0; i < t->size; i++)
        smap_entry_free(t->slots[i]);
    mem_free(t->slots);
    mem_free(t);
}

void smap_free_values(smap_t *map, void (*func)(void *)) {
    assert(map != NULL);

    for (int i = 0; i < map->size; i++) {
        for (smap_entry_t *elem = map->slots[i]; elem; elem = elem->next) {
            func(elem->value);
        }
    }
}

unsigned int smap_hash_fun(const char *key) {
    /* This is the djb2 algorithm as found for hashing strings.
     * Source: http://www.cse.yorku.ca/~oz/hash.html
     */

    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

unsigned int smap_hash(smap_t *t, const char *key) {
    return smap_hash_fun(key) % t->size;
}

smap_entry_t *smap_entry_init(char *key, void *value) {
    smap_entry_t *entry = (smap_entry_t *)malloc(sizeof(smap_entry_t));

    entry->key = strdup(key);
    entry->value = value;
    entry->next = NULL;

    return entry;
}

void smap_insert(smap_t *t, char *key, void *value) {
    smap_entry_t *last;

    if (t->slots[smap_hash(t, key)]) {
        for (last = t->slots[smap_hash(t, key)]; last->next;
             last = last->next) {
            if (strcmp(last->key, key) == 0) {
                last->value = value;
                return;
            }
        }

        last->next = smap_entry_init(key, value);
    } else
        t->slots[smap_hash(t, key)] = smap_entry_init(key, value);
}

void *smap_retrieve(smap_t *t, const char *key) {
    if (t->slots[smap_hash(t, key)])
        for (smap_entry_t *last = t->slots[smap_hash(t, key)]; last;
             last = last->next)
            if (strcmp(last->key, key) == 0)
                return last->value;

    return NULL;
}

void *smap_remove_element(smap_t *map, char *key) {
    const unsigned int hash_val = smap_hash(map, key);
    if (map->slots[hash_val]) {
        smap_entry_t *previous = NULL;
        smap_entry_t *current = map->slots[hash_val];
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                if (previous == NULL) {
                    smap_entry_t *next = current->next;
                    current->next = NULL;
                    void *val = current->value;
                    smap_entry_free(current);
                    map->slots[hash_val] = next;
                    return val;
                } else {
                    smap_entry_t *next = current->next;
                    current->next = NULL;
                    previous->next = next;
                    void *val = current->value;
                    smap_entry_free(current);
                    return val;
                }
            }
            current = current->next;
        }
    }
    return NULL;
}

void smap_map(smap_t *t, void *(f)(char *, void *)) {
    for (int i = 0; i < t->size; i++)
        for (smap_entry_t *last = t->slots[i]; last; last = last->next) {
            last->value = f(last->key, last->value);
        }
}

array *smap_values(smap_t *map) {
    assert(map != NULL);
    if (map->size < 1) {
        return NULL;
    }
    array *values = array_init(map->size);
    for (int i = 0; i < map->size; i++) {
        for (smap_entry_t *elem = map->slots[i]; elem; elem = elem->next) {
            array_append(values, elem->value);
        }
    }
    return values;
}

array *smap_keys(smap_t *map) {
    assert(map != NULL);
    if (map->size < 1) { // Maybe assert or give empty array?
        return NULL;
    }
    array *values = array_init(map->size);
    for (int i = 0; i < map->size; i++) {
        for (smap_entry_t *elem = map->slots[i]; elem; elem = elem->next) {
            array_append(values, elem->key);
        }
    }
    return values;
}
