/*
 *
 *  Look-Up-Table (LUT) for Pointers and Strings
 *  --------------------------------------------
 *
 *  Each entry of a LUT can hold a pair [old, new] where 'old' is either a
 *  pointer (void*) or a string (char*) and 'new' is always a pointer (void*).
 *
 *  To insert a pair [old, new] into the LUT where 'old' represents a pointer
 *  or a string, use the function  InsertIntoLUT_P( lut, old, new)  or
 *  InsertIntoLUT_S( lut, old, new)  respectively.
 *  Note here, that it is possible to put doubles (pairs with identical
 *  compare-data) into the LUT, e.g. pairs (a,b), (a,c).
 *  If doubles are not welcome, it may be better to use the function
 *  UpdateLUT_P() instead.
 *
 *  The function  UpdateLUT_?( lut, old, new, &act_new)  checks whether there
 *  is already a pair [old, act_new] in the LUT. If so, 'act_new' is replaced
 *  by 'new', otherwise  InsertIntoLUT_?()  is called to insert the pair into
 *  the LUT.
 *
 *  The function  SearchInLUT_?( lut, old)  searches the LUT for an entry
 *  [old, new]. If the LUT contains such an entry, a pointer to the associated
 *  data 'new' is returned. Otherwise the return value equals NULL.
 *  Note here, that always the *first* matching entry is returned. If the LUT
 *  contains multiple matching entries, use  SearchInLUT_Next?()  to get
 *  the next match.
 *  For example, let the LUT contain pairs (a,b) and (a,c). When looking-up 'a'
 *  now, the return value is 'b' for the first, 'c' for the second, and NULL
 *  for all subsequent look-ups.
 *  SearchInLUT_P()  searches for a pointer (pointer compare),
 *  SearchInLUT_S()  searches for a string (string compare).
 *
 *  If both values of the pairs [old, new] have the same type (both strings or
 *  both pointers), you may want to use the function  SearchInLUT_SS( lut, old)
 *  or  SearchInLUT_PP( lut, old)  instead.
 *  If the LUT contains an entry [old, new] the associated data 'new' itself
 *  is returned (not a pointer to it). Otherwise the return value equals 'old'
 *  (not NULL).
 *  SearchInLUT_PP()  searches for a pointer (pointer compare) and expects
 *                    to find a pointer,
 *  SearchInLUT_SS()  searches for a string (string compare) and expects
 *                    to find a string.
 *
 *  You can map a function 'fun' to all associated data present in the LUT
 *  by means of  MapLUT( lut, fun). Let  ass_t  be the type of the associated
 *  data in the LUT, then 'fun' should have the signature  ass_t -> ass_t  .
 *  Moreover, the associated data can be folded by using the function
 *  FoldLUT( lut, init, fun)  , where 'init' is the initial value for the
 *  fold operation. Let again  ass_t  be the type of the associated data in
 *  the LUT and let  init_t  be the type of 'init'. Then 'fun' should have
 *  the signature  ( init_t , ass_t ) -> init_t  and the return value of
 *  FoldLUT()  has the type  init_t  .
 *
 *  *** CAUTION ***
 *  - InsertIntoLUT_S()  copies the compare-string ('old') before inserting
 *    it into the LUT. But the associated data is never copied!!
 *    If the associated data is also a string, you may want to duplicate it
 *    with  StringCopy()  first.
 *  - RemoveLUT()  removes all the stored compare-strings from heap memory.
 *  - SearchInLUT_?()  returns a *pointer* to the found associated data. Thus,
 *    the returned pointer will be undefined if  RemoveLUT()  has been called.
 *    Therefore you should not forget to duplicate the data first ... :-/
 *  - The support for multiple entries with identical compare-data is
 *    implemented rather inefficiently :-(
 *
 *
 *  Implementation
 *  --------------
 *
 *  The pairs of data are stored in a hash table. Pairs with identical hash
 *  keys are stored in a collision table (i.e. collision resolution by
 *  chaining).
 *  The hash key spaces for pointers and strings are disjunct. The hash keys
 *  for pointers are elements of the intervall [ 0 .. LUT_KEYS_POINTERS [, the
 *  hash keys for strings are elements of [ LUT_KEYS_POINTERS .. LUT_KEYS [.
 *  Thus, we need an array of size LUT_KEYS which elements are the collision
 *  tables.
 *  The functions GetHashKey_Pointer() and GetHashKey_String() compute for a
 *  pointer and string respectively the index of the collision table the
 *  pointer/string belongs to.
 *  Each collision table is made up of linked table fragments of fixed size
 *  (LUT_SIZE) in order to minimize the data overhead. Initially each collision
 *  table consists of a single table fragment. Each time the last entry of a
 *  collision table has been occupied another fragment is allocated and appended
 *  to the table.
 *
 *  The amount of memory (in words) needed for the whole LUT equals
 *
 *            (K - 1)
 *    MEM  =   SumOf  ( ( ( C[i] + S ) div S ) ( 2 S + 1 ) + 3 ) ,
 *             i = 0
 *
 *    where  K := HASH_KEYS ,  S := LUT_SIZE  and
 *           C[i] denotes the number of pairs the i-th collision table contains.
 *
 *  Let N be the number of pairs stored in the LUT. Suppose that
 *    C[i]  =  N / K
 *  is hold, i.e. we have an optimal hash key function.
 *
 *  Then the value for MEM equals
 *    MEM  =  K ( ( ( N / K + S ) div S ) ( 2 S + 1 ) + 3 )
 *  Thus, for the worst case (S = 1) we get
 *    MEM  =  K ( ( N / K + 1 ) 3 + 3 )
 *         =  3 N + 6 K .
 *  and for the best case (S = N / K + 1) we get
 *    MEM  =  K ( ( ( N / K + N / K + 1 ) div ( N / K + 1 ) ) ( 2 S + 1 ) + 3 )
 *         =  K (                          1                  ( 2 S + 1 ) + 3 )
 *         =  K ( 2 S + 4 )
 *         =  K ( 2 ( N / K + 1 ) + 4 )
 *         =  2 N + 6 K .
 *
 *  The time complexity for searching in the LUT is proportional to
 *    TC  =  N / K .
 *
 *
 *  DBUG strings
 *  ------------
 *
 *  LUT:       debug output for lut
 *  LUT_CHECK: perform some runtime checks for lut
 *
 *  TODO: currently the dbug strings are disabled with an #if 0
 *      because the dbug.h does not implement these yet.
 *
 */

#include "lookup_table.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "palm/str.h"
#include "palm/memory.h"
#include "palm/dbug.h"
#include "palm/ctinfo.h"


#define CHECKMEM 0

/*
 * size of a collision table fragment
 * (== # pairs of data that can be stored in the table)
 */
#define LUT_SIZE 5

/*
 * number of different hash keys
 * (== size of hash table == # collision tables)
 */
#define HASH_KEYS_POINTER  32  /* 2^5 */
#define HASH_KEYS_STRING   17  /* should be a prime number */
#define HASH_KEYS          ((HASH_KEYS_POINTER) + (HASH_KEYS_STRING))


#define HASH_KEY_T unsigned long int
#define HASH_KEY_CONV "%lu"
#define HASH_KEY_CONVT "%4lu"

typedef int lut_size_vt;
typedef HASH_KEY_T hash_key_vt;

/*
 * collision table fragment:
 *
 * 'first' points to the first item of the table.
 * 'next' points to the next free position of the table.
 * All elements of the table lie between these two addresses. The pairs of data
 * are stored as two consecutive table entries.
 * The table is of static size. If the current table is filled, the address
 * of a newly allocated table is stored in the last entry of the current table.
 * 'size' contains the number of items stored in the whole chain of tables.
 */
struct LUT {
  void **first;
  void **next;
  lut_size_vt size;
};

typedef hash_key_vt (*hash_key_fun_t)( void *);
typedef bool (*is_equal_ft)( void *, void *);


/*
 * static functions
 */



/******************************************************************************
 *
 * function:
 *   hash_key_vt GetHashKey_Pointer( void *data)
 *
 * description:
 *   Calculates the hash key for a given pointer.
 *   For the return value the condition
 *     0  <=  return  <  HASH_KEYS_POINTER
 *   must hold.
 *
 ******************************************************************************/

static
hash_key_vt GetHashKey_Pointer( void *data)
{
  hash_key_vt hash_key;

  /*
   * hash key: bits 9 .. 5
   *  ->  0 <= key < 2^5
   */
  hash_key = (((hash_key_vt) data >> 5) & 0x1f);

  DBUG_ASSERT( ((hash_key >= 0) && (hash_key < (HASH_KEYS_POINTER))),
               "hash key for pointers out of bounds!");

  return hash_key;
}



/******************************************************************************
 *
 * function:
 *   hash_key_vt GetHashKey_String( void *data)
 *
 * description:
 *   Calculates the hash key for a given string.
 *   For the return value the condition
 *     HASH_KEYS_POINTERS  <=  return  <  HASH_KEYS
 *   must be hold.
 *
 ******************************************************************************/

static
hash_key_vt GetHashKey_String( void *data)
{
  char *str;
  hash_key_vt hash_key;


  hash_key = 0;
  if (data != NULL) {
    for (str = (char *) data; ((*str) != '\0'); str++) {
      hash_key += (*str);
    }
    hash_key %= (HASH_KEYS_STRING);
  }

  DBUG_ASSERT( ((hash_key >= 0) && (hash_key < (HASH_KEYS_STRING))),
               "hash key for strings out of bounds!");

  /*
   * use the offset HASH_KEYS_POINTERS in order to get disjoint hash key spaces
   * for pointers and strings
   */
  hash_key += (HASH_KEYS_POINTER);
    printf("%lu\n", hash_key);
  return hash_key;
}



/******************************************************************************
 *
 * Function:
 *   bool IsEqual_Pointer( void *data1, void *data2)
 *
 * Description:
 *   Compares the given pointers and returns true iff they contain the same
 *   address.
 *
 ******************************************************************************/

static
bool IsEqual_Pointer( void *data1, void *data2)
{
  bool ret;



  ret = (data1 == data2);

  return ret;
}



/******************************************************************************
 *
 * Function:
 *   bool IsEqual_String( void *data1, void *data2)
 *
 * Description:
 *   Compares the given strings and returns true iff they are equal.
 *
 ******************************************************************************/

static
bool IsEqual_String( void *data1, void *data2)
{


  return STReq( data1, data2);
}



#ifndef DBUG_OFF

/******************************************************************************
 *
 * Function:
 *   void ComputeHashStat( lut_st *lut, char *note,
 *                         hash_key_vt min_key, hash_key_vt max_key)
 *
 * Description:
 *   This function is used from DBUG_EXECUTE only!
 *
 ******************************************************************************/

static
void ComputeHashStat( lut_st *lut, char *note,
                      hash_key_vt min_key, hash_key_vt max_key)
{
  hash_key_vt min_k, max_k, k;
  lut_size_vt size, sum_size;
  lut_size_vt min_size, max_size, diff_size;
  double mean_size, sdev_size, sdev_mean;

  if (lut != NULL) {
#if 0
    DBUG_PRINT( "LUT", ("lut " F_PTR ", %s ---", lut, note));
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  key:  ");
      for (k = min_key; k < max_key; k++) {
        fprintf( stderr, HASH_KEY_CONVT" ",  k);
      }
      fprintf( stderr, "\n");
      fprintf( stderr, "  size: ");
      for (k = min_key; k < max_key; k++) {
        DBUG_EXECUTE( "LUT",
          fprintf( stderr, "%4i ", lut[k].size);
        );
      }
      fprintf( stderr, "\n");
    );
#endif
    /*
     * compute sum_size, min_size, max_size, diff_size, mean_size
     */
    sum_size = 0;
    min_size = max_size = lut[min_key].size;
    min_k = max_k = min_key;
    for (k = min_key; k < max_key; k++) {
      sum_size += size = lut[k].size;
      if (min_size > size) {
        min_size = size;
        min_k = k;
      }
      if (max_size < size) {
        max_size = size;
        max_k = k;
      }
    }
    diff_size = max_size - min_size;
    mean_size = ((double) sum_size) / (max_key - min_key);

    /*
     * compute sdev_size
     */
    sdev_size = 0;
    for (k = min_key; k < max_key; k++) {
      double diff_size = lut[k].size - mean_size;
      sdev_size += (diff_size * diff_size);
    }
    sdev_size = sqrt( sdev_size / (max_key - min_key));
    sdev_mean = (sum_size > 0) ? (sdev_size / mean_size) : 0;

#if 0
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  sum = %i, LUTsize = %i\n", sum_size, LUT_SIZE);
      fprintf( stderr, "  min (key "HASH_KEY_CONV") = %i, max (key "HASH_KEY_CONV") = %i,"
                       " mean = %1.1f, sdev = %1.1f, sdev/mean^2 = %1.2f\n",
                       min_k, min_size, max_k, max_size,
                       mean_size, sdev_size, sdev_mean);
    );
# endif

    if ((diff_size > LUT_SIZE) && (sdev_mean > 0.8)) {
      CTIwarn( "LUT: unbalanced lut (%s) detected:\n"
               "(range = %i..%i,\n"
               " mean = %1.1f, sdev = %1.1f, sdev/mean^2 = %1.2f)",
               note, min_size, max_size, mean_size, sdev_size, sdev_mean);
    }
  }
  else {
    diff_size = 0;
  }

}

#endif /* !DBUG_OFF */



/******************************************************************************
 *
 * Function:
 *   void **SearchInLUT_( lut_size_vt size, lut_size_vt *i, void **entry,
 *                        void *old_item,
 *                        hash_key_vt hash_key,
 *                        is_equal_ft is_equal_fun,
 *                        char *old_format, char *new_format)
 *
 * Description:
 *
 *
 ******************************************************************************/

static
void **SearchInLUT_( lut_size_vt size, lut_size_vt *store_i, void **entry,
                     void *old_item,
                     hash_key_vt hash_key,
                     is_equal_ft is_equal_fun,
                     char *old_format, char *new_format)
{
  void **new_item_p = NULL;


  /* search in the collision table for 'old_item' */
  lut_size_vt i = *store_i;
  for ( ; i < size; i++) {
    if (is_equal_fun( entry[0], old_item)) {
      new_item_p = entry + 1;
      break;
    }
    entry += 2;
    if ((i + 1) % (LUT_SIZE) == 0) {
      /* last table entry is reached -> enter next table of the chain */
      entry = *entry;
    }
  }
  *store_i = i;

# if 0
  if (new_item_p == NULL) {
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  data (hash key "HASH_KEY_CONV") *not* found: ", hash_key);
      fprintf( stderr, old_format, old_item);
      fprintf( stderr, "\n");
    );
  }
  else {
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  data (hash key "HASH_KEY_CONV") found: [ ", hash_key);
      fprintf( stderr, old_format, old_item);
      fprintf( stderr, " -> ");
      fprintf( stderr, new_format, *new_item_p);
      fprintf( stderr, " ]\n");
    );
  }
#endif

  return new_item_p;
}



/******************************************************************************
 *
 * Function:
 *   void **SearchInLUT( lut_st *lut, void *old_item,
 *                       hash_key_vt hash_key,
 *                       is_equal_ft is_equal_fun,
 *                       char *old_format, char *new_format)
 *
 * Description:
 *
 *
 ******************************************************************************/

static
void **SearchInLUT( lut_st *lut, void *old_item,
                    hash_key_vt hash_key,
                    is_equal_ft is_equal_fun,
                    char *old_format, char *new_format)
{
  void **new_item_p = NULL;


# if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif
  if (lut != NULL) {
    if (old_item != NULL) {
      int i = 0;
      new_item_p = SearchInLUT_( lut[ hash_key].size,
                                 &i,
                                 lut[ hash_key].first,
                                 old_item,
                                 hash_key, is_equal_fun,
                                 old_format, new_format);
#if 0
      DBUG_PRINT( "LUT", ("< finished"));
#endif
    }
    else {
#if 0
      DBUG_PRINT( "LUT", ("< finished: data is NULL"));
#endif
    }
  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
  }

  return new_item_p;
}



/******************************************************************************
 *
 * Function:
 *   void **SearchInLUT_state( lut_st *lut, void *old_item,
 *                             hash_key_vt hash_key,
 *                             is_equal_ft is_equal_fun,
 *                             bool init,
 *                             char *old_format, char *new_format)
 *
 * Description:
 *
 *
 ******************************************************************************/

static
void **SearchInLUT_state( lut_st *lut, void *old_item,
                          hash_key_vt hash_key,
                          is_equal_ft is_equal_fun,
                          bool init,
                          char *old_format, char *new_format)
{
  static lut_st *store_lut = NULL;
  static void *store_old_item = NULL;
  static hash_key_vt store_hash_key = 0;
  static lut_size_vt store_size = 0;
  static lut_size_vt store_i = 0;
  static void **store_entry = NULL;

  void **new_item_p = NULL;



  if (init) {
    /*
     * store current parameters for subsequent calls
     */
    store_lut = lut;
#if 0
    DBUG_PRINT( "LUT", ("> lut (" F_PTR "), initial search", store_lut));
#endif
    if (store_lut != NULL) {
      store_old_item = old_item;
      if (store_old_item != NULL) {
        store_hash_key = hash_key;
        store_size = store_lut[ hash_key].size;
        DBUG_ASSERT( (store_size >= 0), "illegal LUT size found!");
        store_i = 0;
        store_entry = store_lut[ hash_key].first;

        new_item_p = SearchInLUT_( store_size, &store_i, store_entry,
                                   store_old_item, hash_key,
                                   is_equal_fun,
                                   old_format, new_format);

        if (new_item_p != NULL) {
          /* Advance pointer where to look from at next search call */
          store_entry = new_item_p - 1;
        }
#if 0
        DBUG_PRINT( "LUT", ("< finished"));
#endif
      }
      else {
#if 0
        DBUG_PRINT( "LUT", ("< finished: data is NULL"));
#endif
      }
    }
    else {
#if 0
      DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
    }
  }
  else {
    /*
     * go to next entry in LUT
     */
#if 0
    DBUG_PRINT( "LUT", ("> lut (" F_PTR "), search for doubles", store_lut));
#endif

    if (store_lut != NULL) {
      if (store_old_item != NULL) {
        store_entry += 2;
        if ((store_i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          store_entry = *store_entry;
        }
        store_i++;

        new_item_p = SearchInLUT_( store_size, &store_i, store_entry,
                                   store_old_item, store_hash_key,
                                   is_equal_fun, old_format, new_format);

        if (new_item_p != NULL) {
          /* Advance pointer where to look from at next search call */
          store_entry = new_item_p - 1;
        }
#if 0
        DBUG_PRINT( "LUT", ("< finished"));
#endif
      }
      else {
#if 0
        DBUG_PRINT( "LUT", ("< finished: data is NULL"));
#endif
      }
    }
    else {
#if 0
      DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
    }
  }

  return new_item_p;
}



/******************************************************************************
 *
 * function:
 *   lut_st *InsertIntoLUT_noDBUG( lut_st *lut, void *old_item, void *new_item,
 *                                hash_key_vt hash_key)
 *
 * description:
 *
 *
 ******************************************************************************/

static
lut_st *InsertIntoLUT_noDBUG( lut_st *lut, void *old_item, void *new_item,
                             hash_key_vt hash_key)
{


  DBUG_ASSERT( (lut != NULL), "no LUT found!");

  *(lut[ hash_key].next++) = old_item;
  *(lut[ hash_key].next++) = new_item;
  lut[ hash_key].size++;
  DBUG_ASSERT( (lut[ hash_key].size >= 0), "illegal LUT size found!");

  if (lut[ hash_key].size % (LUT_SIZE) == 0) {
    /* the last table entry has been used -> allocate a new one */
    *lut[ hash_key].next = (void **) MEMmalloc( (2*(LUT_SIZE) + 1)
                         * sizeof( void *));
#if 0
    DBUG_PRINT( "LUT", ("new LUT segment created: " F_PTR,
                        lut[ hash_key].next));
#endif
    /* move 'next' to the first entry of the new table */
    lut[ hash_key].next = *lut[ hash_key].next;
  }

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *InsertIntoLUT( lut_st *lut, void *old_item, void *new_item,
 *                         hash_key_vt hash_key,
 *                         char *old_format, char *new_format)
 *
 * description:
 *
 *
 ******************************************************************************/

static
lut_st *InsertIntoLUT( lut_st *lut, void *old_item, void *new_item,
                      hash_key_vt hash_key,
                      char *old_format, char *new_format)
{

#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif

  if (lut != NULL) {
    DBUG_ASSERT( (old_item != NULL), "NULL not allowed in LUT");
    lut = InsertIntoLUT_noDBUG( lut, old_item, new_item, hash_key);

#if 0
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  new data inserted: [ ");
      fprintf( stderr, old_format, old_item);
      fprintf( stderr, " -> ");
      fprintf( stderr, new_format, new_item);
      fprintf( stderr, " ]\n");
    );

    DBUG_PRINT( "LUT", ("< finished: new LUT size (hash key %i) == %i",
                        hash_key, lut[ hash_key].size));

    DBUG_EXECUTE( "LUT_CHECK",
      /* check quality of hash key function */
      ComputeHashStat( lut, "pointers", 0,                 HASH_KEYS_POINTER);
      ComputeHashStat( lut, "strings",  HASH_KEYS_POINTER, HASH_KEYS);
    );
#endif
  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
  }

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *UpdateLUT( lut_st *lut, void *old_item, void *new_item,
 *                     hash_key_vt hash_key,
 *                     is_equal_ft is_equal_fun,
 *                     char *old_format, char *new_format,
 *                     void **found_item)
 *
 * description:
 *
 *
 ******************************************************************************/

lut_st *UpdateLUT( lut_st *lut, void *old_item, void *new_item,
                  hash_key_vt hash_key,
                  is_equal_ft is_equal_fun,
                  char *old_format, char *new_format,
                  void **found_item)
{
  void **found_item_p;

#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif

  found_item_p = SearchInLUT( lut, old_item, hash_key, is_equal_fun,
                              old_format, new_format);

  if (found_item_p == NULL) {
    lut = InsertIntoLUT( lut,
                         (hash_key < (HASH_KEYS_POINTER))
                             ? old_item
                             : STRcpy( old_item),
                         new_item, hash_key,
                         old_format, new_format);

    if (found_item != NULL) {
      (*found_item) = NULL;
    }
  }
  else {
# if 0
    DBUG_EXECUTE( "LUT",
      fprintf( stderr, "  data replaced: [ ");
      fprintf( stderr, old_format, old_item);
      fprintf( stderr, " -> ");
      fprintf( stderr, new_format, *found_item_p);
      fprintf( stderr, " ] =>> [ ");
      fprintf( stderr, old_format, old_item);
      fprintf( stderr, " -> ");
      fprintf( stderr, new_format, new_item);
      fprintf( stderr, " ]\n");
    );
#endif
    (*found_item_p) = new_item; /* TODO: Stijn moved this line to below the if. Ask him why */

    if (found_item != NULL) {
      (*found_item) = (*found_item_p);
    }
  }

#if 0
  DBUG_PRINT( "LUT", ("< finished"));
#endif
  return lut;
}



/******************************************************************************
 *
 * Function:
 *   lut_st *MapLUT( lut_st *lut, void *(*fun)( void *),
 *                  hash_key_vt start, hash_key_vt stop)
 *
 * Description:
 *
 *
 ******************************************************************************/

static
lut_st *MapLUT( lut_st *lut, void *(*fun)( void *),
               hash_key_vt start, hash_key_vt stop)
{
  void **tmp;
  hash_key_vt k;
  lut_size_vt i;


#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif

  if (lut != NULL) {
    for (k = start; k < stop; k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      tmp = lut[k].first;
      for (i = 0; i < lut[k].size; i++) {
        tmp[1] = fun( tmp[1]);
        tmp += 2;
        if ((i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          tmp = *tmp;
        }
      }
    }
#if 0
    DBUG_PRINT( "LUT", ("< finished"));
#endif
  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
  }

  return lut;
}



/******************************************************************************
 *
 * Function:
 *   void *FoldLUT( lut_st *lut, void *init, void *(*fun)( void *, void *),
 *                  hash_key_vt start, hash_key_vt stop)
 *
 * Description:
 *
 *
 ******************************************************************************/

static
void *FoldLUT( lut_st *lut, void *init, void *(*fun)( void *, void *),
               hash_key_vt start, hash_key_vt stop)
{
  void **tmp;
  hash_key_vt k;
  lut_size_vt i;


#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif

  if (lut != NULL) {
    for (k = start; k < stop; k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      tmp = lut[k].first;
      for (i = 0; i < lut[k].size; i++) {
        init = fun( init, tmp[1]);
        tmp += 2;
        if ((i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          tmp = *tmp;
        }
      }
    }
#if 0
    DBUG_PRINT( "LUT", ("< finished"));
#endif
  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif
  }

  return init;
}



/*
 * exported functions
 */



/******************************************************************************
 *
 * function:
 *   lut_st *LUTgenerateLut()
 *
 * description:
 *   Generates a new LUT: All the needed collision tables are created and the
 *   internal data structures are initialized.
 *
 ******************************************************************************/

lut_st *LUTgenerateLut( void)
{
  lut_st *lut;
  hash_key_vt k;

  lut = (lut_st *) MEMmalloc( (HASH_KEYS) * sizeof( lut_st));

#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif

  for (k = 0; k < (HASH_KEYS); k++) {
    lut[k].first = (void **) MEMmalloc( (2*(LUT_SIZE) + 1) * sizeof( void *));
    lut[k].next = lut[k].first;
    lut[k].size = 0;
  }
#if 0
  DBUG_PRINT( "LUT", ("< finished"));
#endif

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTduplicateLut( lut_st *lut)
 *
 * description:
 *   Duplicates the given LUT.
 *
 ******************************************************************************/

lut_st *LUTduplicateLut( lut_st *lut)
{
  lut_st *new_lut;
  void **tmp;
  hash_key_vt k;
  lut_size_vt i;


#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif


  if (lut != NULL) {
    new_lut = LUTgenerateLut();

    for (k = 0; k < (HASH_KEYS_POINTER); k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      tmp = lut[k].first;
      for (i = 0; i < lut[k].size; i++) {
        new_lut = InsertIntoLUT_noDBUG( new_lut,
                                        tmp[0],
                                        tmp[1],
                                        k);
        tmp += 2;
        if ((i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          tmp = *tmp;
        }
      }
    }
    for (k = (HASH_KEYS_POINTER); k < (HASH_KEYS); k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      tmp = lut[k].first;
      for (i = 0; i < lut[k].size; i++) {
        new_lut = InsertIntoLUT_noDBUG( new_lut,
                                        STRcpy( (char *) (tmp[0])),
                                        tmp[1],
                                        k);
        tmp += 2;
        if ((i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          tmp = *tmp;
        }
      }
    }
#if 0
    DBUG_PRINT( "LUT", ("< finished"));
#endif

  }
  else {
    new_lut = NULL;
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif

  }

  return new_lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTremoveContentLut( lut_st *lut)
 *
 * description:
 *   Removes the content of the given LUT from memory.
 *
 ******************************************************************************/

lut_st *LUTremoveContentLut( lut_st *lut)
{
  void **first, **tmp;
  hash_key_vt k;
  lut_size_vt i;


#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif


  if (lut != NULL) {
    /* init LUT for pointers */
    for (k = 0; k < (HASH_KEYS_POINTER); k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      /* remove all but the first collision table fragments */
      for (i = 1; i <= lut[k].size / (LUT_SIZE); i++) {
        tmp = lut[k].first;
        lut[k].first = lut[k].first[ 2*(LUT_SIZE)];
        tmp = MEMfree( tmp);
      }
      lut[k].next = lut[k].first;
      lut[k].size = 0;
    }
    /* init LUT for strings */
    for (k = (HASH_KEYS_POINTER); k < (HASH_KEYS); k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");
      tmp = lut[k].first;
      first = tmp;
      /* remove all strings and all but the first collision table fragments */
      for (i = 0; i < lut[k].size; i++) {
        tmp[0] = MEMfree( tmp[0]);
        tmp += 2;
        if ((i + 1) % (LUT_SIZE) == 0) {
          /* last table entry is reached -> enter next table of the chain */
          tmp = *tmp;
          first = MEMfree( first);
          first = tmp;
        }
      }
      lut[k].first = lut[k].next = first;
      lut[k].size = 0;
    }
#if 0
    DBUG_PRINT( "LUT", ("< finished"));
#endif

  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif

  }

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTremoveLut( lut_st *lut)
 *
 * description:
 *   Removes the given LUT from memory.
 *
 ******************************************************************************/

lut_st *LUTremoveLut( lut_st *lut)
{
  hash_key_vt k;


#if 0
  DBUG_PRINT( "LUT", ("> lut (" F_PTR ")", lut));
#endif


  if (lut != NULL) {
    /* remove content of LUT */
    lut = LUTremoveContentLut( lut);

    /* remove empty LUT */
    for (k = 0; k < (HASH_KEYS); k++) {

      DBUG_ASSERT( (lut[k].size == 0), "LUT not empty!");
      lut[k].first = MEMfree( lut[k].first);
    }
    lut = MEMfree( lut);
#if 0
    DBUG_PRINT( "LUT", ("< finished"));
#endif

  }
  else {
#if 0
    DBUG_PRINT( "LUT", ("< FAILED: lut is NULL"));
#endif

  }

  return lut;
}




/******************************************************************************
 *
 * Function:
 *   bool LUTisEmptyLut( lut_st *lut)
 *
 * Description:
 *   Returns true iff the given LUT is empty.
 *
 ******************************************************************************/

bool LUTisEmptyLut( lut_st *lut)
{
  hash_key_vt k;
  bool empty = true;



  if (lut != NULL) {
    for (k = 0; k < (HASH_KEYS); k++) {
      DBUG_ASSERT( (lut[k].size >= 0), "illegal LUT size found!");

      if (lut[k].size > 0) {
        empty = false;
        break;
      }
    }
  }

  return empty;
}



/******************************************************************************
 *
 * function:
 *   void **LUTsearchInLutP( lut_st *lut, void *old_item)
 *
 * description:
 *   Searches for the given pointer in the LUT.
 *   If the pointer is not found in the LUT, NULL is returned.
 *   Otherwise the *address* of the data associated with the found pointer is
 *   returned.
 *
 * caution:
 *   If the LUT contains multiple entries for the given string, the *first*
 *   match is returned.
 *   To get in touch with the other entries, the function SearchInLUT_NextS()
 *   should be used.
 *
 ******************************************************************************/

void **LUTsearchInLutP( lut_st *lut, void *old_item)
{
  void **new_item_p;



  new_item_p = SearchInLUT_state( lut, old_item,
                                  GetHashKey_Pointer( old_item),
                                  IsEqual_Pointer,
                                  true,
                                  F_PTR, F_PTR);

  return new_item_p;
}



/******************************************************************************
 *
 * function:
 *   void **LUTsearchInLutS( lut_st *lut, char *old_item)
 *
 * description:
 *   Searches for the given string in the LUT.
 *   If the string is not found in the LUT, NULL is returned.
 *   Otherwise the *address* of the data associated with the found string is
 *   returned.
 *
 * caution:
 *   If the LUT contains multiple entries for the given string, the *first*
 *   match is returned.
 *   To get in touch with the other entries, the function SearchInLUT_NextS()
 *   should be used.
 *
 ******************************************************************************/

void **LUTsearchInLutS( lut_st *lut, char *old_item)
{
  void **new_item_p;



  new_item_p = SearchInLUT_state( lut, old_item,
                                  GetHashKey_String( old_item),
                                  IsEqual_String,
                                  true,
                                  "\"%s\"", F_PTR);

  return new_item_p;
}



/******************************************************************************
 *
 * function:
 *   void **LUTsearchInLutNextP( void)
 *
 * description:
 *   Searches for the next entry matching for the given pointer.
 *   Iff no more matching entries are found in the LUT, NULL is returned.
 *
 * caution:
 *   The function LUTsearchInLutP() must be called first!!
 *
 ******************************************************************************/

void **LUTsearchInLutNextP( void)
{
  void **new_item_p;



  new_item_p = SearchInLUT_state( NULL, NULL, 0,
                                  IsEqual_Pointer,
                                  false,
                                  F_PTR, F_PTR);

  return new_item_p;
}



/******************************************************************************
 *
 * function:
 *   void **LUTsearchInLutNextS( void)
 *
 * description:
 *   Searches for the next entry matching for the given string.
 *   Iff no more matching entries are found in the LUT, NULL is returned.
 *
 * caution:
 *   The function LUTsearchInLutS() must be called first!!
 *
 ******************************************************************************/

void **LUTsearchInLutNextS( void)
{
  void **new_item_p;



  new_item_p = SearchInLUT_state( NULL, NULL, 0,
                                  IsEqual_String,
                                  false,
                                  "\"%s\"", F_PTR);

  return new_item_p;
}



/******************************************************************************
 *
 * function:
 *   void *LUTsearchInLutPp( lut_st *lut, void *old_item)
 *
 * description:
 *   Searches for the given pointer in the LUT.
 *   If the pointer is not found in the LUT, the same pointer is returned.
 *   Otherwise the pointer associated with the found pointer is returned.
 *
 * caution:
 *   If the LUT contains multiple entries for the given string, the *first*
 *   match is returned.
 *   To get in touch with the other entries, the function SearchInLUT_NextP()
 *   should be used.
 *
 ******************************************************************************/

void *LUTsearchInLutPp( lut_st *lut, void *old_item)
{
  void **new_item_p;
  void *new_item;



  new_item_p = LUTsearchInLutP( lut, old_item);

  new_item = (new_item_p == NULL) ? old_item : *new_item_p;

  return new_item;
}



/******************************************************************************
 *
 * function:
 *   char *LUTsearchInLutSs( lut_st *lut, char *old_item)
 *
 * description:
 *   Searches for the given string in the LUT.
 *   If the string is *not* found in the LUT, the same string is returned.
 *   Otherwise the string associated with the found string is returned.
 *
 * caution:
 *   If the LUT contains multiple entries for the given string, the *first*
 *   match is returned.
 *   To get in touch with the other entries, the function SearchInLUT_NextS()
 *   should be used.
 *
 *   This function should only be used iff the associated data is indeed a
 *   string!!
 *
 ******************************************************************************/

char *LUTsearchInLutSs( lut_st *lut, char *old_item)
{
  char **new_item_p;
  char *new_item;



  new_item_p = (char **) SearchInLUT_state( lut, old_item,
                                            GetHashKey_String( old_item),
                                            IsEqual_String,
                                            true,
                                            "\"%s\"", "\"%s\"");

  new_item = (new_item_p == NULL) ? old_item : *new_item_p;

  return new_item;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTinsertIntoLutP( lut_st *lut, void *old_item, void *new_item)
 *
 * description:
 *   Inserts the given pair of pointers (old_item, new_item) into the correct
 *   collision table of the LUT.
 *
 * remark:
 *   It is possible to put doubles (pairs with identical compare-data) into the
 *   LUT, e.g. pairs (a,b), (a,c). When looking-up 'a' now, the return value
 *   is 'b' for the first, 'c' for the second, and NULL for all subsequent
 *   look-ups.
 *   If doubles are not welcome, it may be better to use the function
 *   UpdateLUT_P() instead!
 *
 *****************************************************************************/

lut_st *LUTinsertIntoLutP( lut_st *lut, void *old_item, void *new_item)
{


  lut = InsertIntoLUT( lut, old_item, new_item,
                       GetHashKey_Pointer( old_item),
                       F_PTR, F_PTR);

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTinsertIntoLutS( lut_st *lut, char *old_item, void *new_item)
 *
 * description:
 *   Inserts the given pair of strings (old_item, new_item) into the correct
 *   collision table of the LUT.
 *
 * remark:
 *   It is possible to put doubles (pairs with identical compare-data) into the
 *   LUT, e.g. pairs (a,b), (a,c). When looking-up 'a' now, the return value
 *   is 'b' for the first, 'c' for the second, and NULL for all subsequent
 *   look-ups.
 *   If doubles are not welcome, it may be better to use the function
 *   UpdateLUT_S() instead!
 *
 ******************************************************************************/

lut_st *LUTinsertIntoLutS( lut_st *lut, char *old_item, void *new_item)
{


  lut = InsertIntoLUT( lut, STRcpy( old_item), new_item,
                       GetHashKey_String( old_item),
                       "\"%s\"", F_PTR);

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTupdateLutP( lut_st *lut, void *old_item, void *new_item,
 *                         void **found_item)
 *
 * description:
 *   Inserts the given pair of pointers (old_item, new_item) into the correct
 *   collision table of the LUT.
 *   If a pair (old_item, old_new_item) is already present in the table, the
 *   data represented by 'old_new_item' is saved in '*found_item' and then
 *   overwritten.
 *   Otherwise a new entry is put into the collision table and NULL is stored
 *   in '*found_item'.
 *
 *****************************************************************************/

lut_st *LUTupdateLutP( lut_st *lut, void *old_item, void *new_item,
              void **found_item)
{


  lut = UpdateLUT( lut, old_item, new_item,
                   GetHashKey_Pointer( old_item),
                   IsEqual_Pointer,
                   F_PTR, F_PTR, found_item);

  return lut;
}



/******************************************************************************
 *
 * function:
 *   lut_st *LUTupdateLutS( lut_st *lut, char *old_item, void *new_item,
 *                         void **found_item)
 *
 * description:
 *   Inserts the given pair of strings (old_item, new_item) into the correct
 *   collision table of the LUT.
 *   If a pair (old_item, old_new_item) is already present in the table, the
 *   data represented by 'old_new_item' is saved in '*found_item' and then
 *   overwritten.
 *   Otherwise a new entry is put into the collision table and NULL is stored
 *   in '*found_item'.
 *
 ******************************************************************************/

lut_st *LUTupdateLutS( lut_st *lut, char *old_item, void *new_item,
              void **found_item)
{


  lut = UpdateLUT( lut, old_item, new_item,
                   GetHashKey_String( old_item),
                   IsEqual_String,
                   "\"%s\"", F_PTR, found_item);

  return lut;
}



/******************************************************************************
 *
 * Function:
 *   lut_st *LUTmapLutS( lut_st *lut, void *(*fun)( void *))
 *
 * Description:
 *   Applies 'fun' to all data found in the LUT which is associated to strings.
 *
 ******************************************************************************/

lut_st *LUTmapLutS( lut_st *lut, void *(*fun)( void *))
{


  lut = MapLUT( lut, fun, HASH_KEYS_POINTER, HASH_KEYS);

  return lut;
}



/******************************************************************************
 *
 * Function:
 *   lut_st *LUTmapLutP( lut_st *lut, void *(*fun)( void *))
 *
 * Description:
 *   Applies 'fun' to all data found in the LUT which is associated to pointers.
 *
 ******************************************************************************/

lut_st *LUTmapLutP( lut_st *lut, void *(*fun)( void *))
{


  lut = MapLUT( lut, fun, 0, HASH_KEYS_POINTER);

  return lut;
}



/******************************************************************************
 *
 * Function:
 *   void *LUTfoldLutS( lut_st *lut, void *init, void *(*fun)( void *, void *))
 *
 * Description:
 *   Applies 'fun' to all data found in the LUT which is associated to strings.
 *
 ******************************************************************************/

void *LUTfoldLutS( lut_st *lut, void *init, void *(*fun)( void *, void *))
{


  init = FoldLUT( lut, init, fun, HASH_KEYS_POINTER, HASH_KEYS);

  return init;
}



/******************************************************************************
 *
 * Function:
 *   void *LUTfoldLutP( lut_st *lut, void *init, void *(*fun)( void *, void *))
 *
 * Description:
 *   Applies 'fun' to all data found in the LUT which is associated to pointers.
 *
 ******************************************************************************/

void *LUTfoldLutP( lut_st *lut, void *init, void *(*fun)( void *, void *))
{


  init = FoldLUT( lut, init, fun, 0, HASH_KEYS_POINTER);

  return init;
}