#pragma once

/** Set implementation that allows for basic functions on all type of elements.
 * It is a generic set, however every set does not a key function and a copy function.
 * They key function should return a string and be unique for the elements.
 * The element returned by the key function should be allocated.
 * The copy function should be able to make a deep copy of an element.
 */


#include <stdbool.h>
#include <stdlib.h>

#include "lib/smap.h"
#include "lib/memory.h"

typedef struct ccn_set {
    size_t size;
    smap_t *hash_map;
    char *(*key_func)(void *);
    void *(*copy_func)(const void *);
} ccn_set_t;

/** Function to create a set.
 *  The key function should returned a allocated char *.
 *  The copy func should create a deep copy of the item in the set.
 *  
 *  The copy function is optional, set it to NULL if you never want
 *  to create copies. When the copy_func is set to NULL, the
 *  ccn_set_insert function will NOT create a copy.
 *  This is useful for sets that have large objects with expensive
 *  copy operations.
 */
ccn_set_t *ccn_set_create(char *(*key_func)(void *),
                         void *(*copy_func)(const void *));


ccn_set_t *ccn_set_create_with_size(char *(*key_func)(void *),
                                   void *(*copy_func)(const void *),
                                   size_t size);

/* Insert item into set, if item is already in set, return false
 * Creates a copy of the item when inserting.
 * else true.
 * 
 * CONTRACT:
 *    - set can not be NULL
 *    - item can not be NULL
 */
bool ccn_set_insert(ccn_set_t *set, void *item);

/* Insert item into set, same returns and contracts as ccn_set_insert.
 * This function does not copy the item before inserting.
 */
bool ccn_set_insert_noncopy(ccn_set_t *set, void *item);


size_t ccn_set_size(ccn_set_t *set);

/* Checks if the item is contained in the set. 
 *
 * CONTRACTS:
 *   set can not be NULL,
 *   item can not be NULL.
*/
bool ccn_set_contains(ccn_set_t *set, void *item);

// NOT IMPLEMENTED, yet!
void ccn_set_remove_item(ccn_set_t *set, void *item);

/* Creates a deep copy of the target set. */
ccn_set_t *ccn_set_copy(ccn_set_t *target);

/* Function to free a set with a special free function
 * for the elements in the set. If the free function
 * is NULL, the elements will not be freed, but the 
 * set will be destroyed.
 * 
 * CONTRACTS:
 *    - requires set to be NOT NULL.
 */
void ccn_set_free_with_func(ccn_set_t *set, void (*free_func)(void *));

/* Wrapper around using the mem_free function for you set free. 
 * 
 * CONTRACTS: same as the function being wrapped.
*/
inline void ccn_set_free(ccn_set_t *set) {
    ccn_set_free_with_func(set, mem_free);
}

/* Returns an array of all the values in the set.
 * The values are NOT copied, so no ownership
 * over values.
 */
array *ccn_set_values(ccn_set_t *set);

void *ccn_set_get(ccn_set_t *set, void *item);

/* Returns a new set that is the intersection of a and b. 
 *
 * CONTRACTS:
 *  - both a and b can NOT be NULL.
*/
ccn_set_t *ccn_set_intersect(ccn_set_t *a, ccn_set_t *b);

/* Returns a new set that is the union of a and b.
 * 
 * CONTRACTS:
 *  - both a and b can NOT be NULL.
*/
ccn_set_t *ccn_set_union(ccn_set_t *a, ccn_set_t *b);

/* Return a new set that is the difference (a-b) of a and b. 
 * 
 * CONTRACTS:
 *  - both a and b can NOT be NULL.
*/
ccn_set_t *ccn_set_difference(ccn_set_t *a, ccn_set_t *b);
