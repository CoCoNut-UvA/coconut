#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct htable htable_st;
typedef size_t hash_key_vt;
typedef hash_key_vt (*hash_key_ft)(void *);
typedef bool (*is_equal_ft)(void *, void *);
typedef void *(*cpy_key_ft)(void *);
typedef void (*free_key_ft)(void *);
typedef void *(*map_ft)(void *);
typedef void *(*fold_ft)(void *acc, void *item);


htable_st *HTnew(size_t size, hash_key_ft hash_func, is_equal_ft is_equal_func, cpy_key_ft cpy_key, free_key_ft free_key);
bool HTinsert(htable_st *table, void *key, void *value);
void *HTlookup(htable_st *table, void *key, bool *found);
void *HTremove(htable_st *table, void *key);
void HTdelete(htable_st *table);
void HTmap(htable_st *table, map_ft fun);
void HTfold(htable_st *table, fold_ft fun, void *init_acc);
size_t HTelementCount(htable_st *table);

hash_key_vt HThash_String(char *key);


/* Implementations */
htable_st *HTnew_String(size_t size);
htable_st *HTnew_Ptr(size_t size);
htable_st *HTnew_Int(size_t size);
