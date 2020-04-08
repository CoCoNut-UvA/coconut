#pragma once

#include "lib/array.h"
#include "lib/memory.h"

typedef struct smap_entry_t {
    char *key;
    void *value;
    struct smap_entry_t *next;
} smap_entry_t;

typedef struct smap_t {
    size_t size;
    struct smap_entry_t **slots;
} smap_t;

smap_t *smap_init(size_t);
void smap_free_values(smap_t *map, void (*func)(void *));
void smap_free(smap_t *map);
unsigned int smap_hash_fun(const char *);
unsigned int smap_hash(smap_t *, const char *);
smap_entry_t *smap_entry_init(char *, void *);
void smap_insert(smap_t *, char *, void *);
void *smap_retrieve(smap_t *, const char *);
void *smap_remove_element(smap_t *map, char *key);
void smap_map(smap_t *, void *(f)(char *, void *));
array *smap_values(smap_t *map);
array *smap_keys(smap_t *map);