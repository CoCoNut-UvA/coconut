#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct htable htable_st;
typedef struct htable_iter htable_iter_st;
typedef size_t (*hash_key_ft)(void *);
typedef bool (*is_equal_ft)(void *, void *);
typedef void *(*map_ft)(void *);
typedef void *(*mapk_ft)(void *key, void *item);
typedef void *(*mapdk_ft)(void *data, void *key, void *item);
typedef void *(*fold_ft)(void *acc, void *item);

htable_st *HTnew(size_t size, hash_key_ft hash_func, is_equal_ft is_equal_func);
bool HTinsert(htable_st *table, void *key, void *value);
void *HTlookup(htable_st *table, void *key);
void *HTremove(htable_st *table, void *key);
void HTclear(struct htable *table);
void HTdelete(htable_st *table);
void HTmap(htable_st *table, map_ft fun);
void HTmapWithKey(htable_st *table, mapk_ft fun);
void HTmapWithDataAndKey(htable_st *table, void *data, mapdk_ft fun);
void HTfold(htable_st *table, fold_ft fun, void *init_acc);
size_t HTelementCount(htable_st *table);
htable_iter_st *HTiterate(htable_st *table);
htable_iter_st *HTiterateNext(htable_iter_st *iter);
void HTiterateCancel(htable_iter_st *iter);

void *HTiterValue(htable_iter_st *iter);
void *HTiterKey(htable_iter_st *iter);

/* Implementations */
/* Htable that maps char * -> void * */
htable_st *HTnew_String(size_t size);

/* Htable that maps void * -> void * */
htable_st *HTnew_Ptr(size_t size);

/* Map int * -> void * */
htable_st *HTnew_Int(size_t size);
