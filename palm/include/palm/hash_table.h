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

/**
 * Create a new hash table.
 * @param size the number of buckets in a hash table.
 *  More buckets is more space, but les collisions.
 *  See pre-defined hash tables in the bottom of this file.
 * @param hash_func the function used for hashing keys.
 * @param is_equal_func the function to determine if two values are equal.
 */
htable_st *HTnew(size_t size, hash_key_ft hash_func, is_equal_ft is_equal_func);

/**
 * Insert the key, value pair from the table.
 *
 * @return true if insertion was successful.
 */
bool HTinsert(htable_st *table, void *key, void *value);

/**
 * Lookup a value in the hash table.
 *
 * @return the corresponding value or NULL if key is not found.
 */
void *HTlookup(htable_st *table, void *key);

/**
 * Remove the key, value pair from the table.
 *
 * @return the deleted value or NULL if key is not found.
 */
void *HTremove(htable_st *table, void *key);

/**
 * Clears all entries in the hash table.
 * Does not delete the table, so can be reused.
 */
void HTclear(struct htable *table);

/**
 * Delete all entries in the table and the table itself.
 * Table is not usable after this operation.
 */
void HTdelete(htable_st *table);


/**
 * Map functions that apply a user function to all entries in the hash tables.
 * The entry values are replaced by the return values of the user map function.
 * - HTmap only passes the entry values to the map function.
 * - HTmapWithKey passes the entry keys & values to the map functions.
 * - HTmapWithKey passes the entry keys & values and user-provided data to the
 *   map functions.
 */
void HTmap(htable_st *table, map_ft fun);
void HTmapWithKey(htable_st *table, mapk_ft fun);
void HTmapWithDataAndKey(htable_st *table, void *data, mapdk_ft fun);

void HTfold(htable_st *table, fold_ft fun, void *init_acc);
/**
 * @return the amount of entries in the hash table.
 */
size_t HTelementCount(htable_st *table);

/* Iteration functions that can be used to iterate over all keys and values in
   a hashtable.
   These functions are an alternative to the HTmap* functions.

   Example usage:
   for (htable_iter_st *iter = HTiterate(htable); iter;
        iter = HTiterateNext(iter)) {
       // Getter functions to extract htable elements
       void *key = HTiterKey(iter);
       void *value = HTiterValue(iter);

       // Use HTiterateCancel in early loop exits
       if (condition) {
           HTiterateCancel(iter);
       }
   }
*/
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
