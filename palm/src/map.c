/*
 *  MAP implements a map also known as associative memory.
 *
 *  map *MAPcreate( bool (*eq)(void *, void *))
 *   creates a new map given a key comparison function.
 *   For now we only accept STReq for string equality and NULL for shallow
 *   pointer equality.
 *
 *  map *MAPcopy( map *map)
 *   creates a copy of the given map.
 *
 *  map *MAPdelete( map *map)
 *   deletes the given map and yields NULL.
 *
 *  map *MAPclear( map *map)
 *   removes all data within the map and yields the empty itself.
 *
 *  bool MAPisEmpty( map *map)
 *   checks whether or not the given map is empty.
 *
 *  map *MAPinsert( map *map, void *key, void *item)
 *   inserts the given data item under the given key into the given map;
 *   yields the updated map.
 *   The key must be compatible with the initially chosen equality function
 *   and must not be present in the map before.
 *
 *  void *MAPlookup(  map *map, void *key)
 *   looks up the given key in the given map and yields the data item stored
 *   under that key or NULL if the key is not present in the map.
 *
 *  map *MAPupdate( map *map, void *key, void *item)
 *   replaces the data associated with the given key in the given map by the
 *   given data item and yields the updated map. If the key is not found in
 *   the map, the given data item is inserted under that key. T
 *
 *  map *MAPswap( map *map, void *key, void *item, void **found)
 *   replaces the data associated with the given key in the given map by the
 *   given data item and yields the updated map. The original value is stored
 *   where indicated by the 4th parameter. If the key is not found in
 *   the map, the given data item is inserted under that key and NULL is
 *   written to the address pointed to by the 4th parameter.
 *
 *  void MAPmap( map *map, void *(*mapfun)( void *))
 *   maps a map function over all entries in the map. The order is undefined.
 *
 *  void *MAPfold( map *map, void *start, void *(*foldfun)( void *, void *))
 *   folds all entries in the given map using the given fold function. The
 *   second parameter acts as a start value for the folding process.
 *
 *  Note:
 *  Character string keys are copied before inserted into the map.
 */

#include "palm/map.h"

#include <stdlib.h>

#include "lookup_table.h"
#include "palm/dbug.h"
#include "palm/memory.h"
#include "palm/str.h"

struct MAP_ST {
    lut_st *lut;
    bool (*eq)(void *, void *);
};

map_st *MAPcreate(bool (*eq)(void *, void *))
{
    map_st *map;

    DBUG_ASSERT((eq == NULL || eq == (bool (*)(void *, void *))STReq),
        "Unsupported key equality function: "
        "we only support STReq() and NULL for pointer comparison");

    map = MEMmalloc(sizeof(map_st));

    map->lut = LUTgenerateLut();
    map->eq = eq;

    return map;
}

map_st *MAPcopy(map_st *old)
{
    map_st *map = MEMmalloc(sizeof(map_st));

    map->lut = LUTduplicateLut(old->lut);
    map->eq = old->eq;

    return map;
}

map_st *MAPdelete(map_st *map)
{
    map->lut = LUTremoveLut(map->lut);
    map = MEMfree(map);

    return map;
}

map_st *MAPclear(map_st *map)
{
    map->lut = LUTremoveContentLut(map->lut);

    return map;
}

bool MAPisEmpty(map_st *map)
{
    LUTisEmptyLut(map->lut);
}

map_st *MAPinsert(map_st *map, void *key, void *item)
{
    if (map->eq == NULL) {
        map->lut = LUTinsertIntoLutP(map->lut, key, item);
    } else {
        map->lut = LUTinsertIntoLutS(map->lut, key, item);
    }

    return map;
}

void **MAPlookup(map_st *map, void *key)
{
    void *item;

    if (map->eq == NULL) {
        item = LUTsearchInLutP(map->lut, key);
    } else {
        item = LUTsearchInLutS(map->lut, key);
    }

    return item;
}

map_st *MAPupdate(map_st *map, void *key, void *item)
{
    void *found;

    if (map->eq == NULL) {
        map->lut = LUTupdateLutP(map->lut, key, item, &found);
    } else {
        map->lut = LUTupdateLutS(map->lut, key, item, &found);
    }

    return map;
}

map_st *MAPswap(map_st *map, void *key, void *item, void **found)
{
    if (map->eq == NULL) {
        map->lut = LUTupdateLutP(map->lut, key, item, found);
    } else {
        map->lut = LUTupdateLutS(map->lut, key, item, found);
    }

    return map;
}

void MAPmap(map_st *map, void *(*mapfun)(void *))
{
    if (map->eq == NULL) {
        LUTmapLutP(map->lut, mapfun);
    } else {
        LUTmapLutS(map->lut, mapfun);
    }
}

void *MAPfold(map_st *map, void *start, void *(*foldfun)(void *, void *))
{
    void *res;

    if (map->eq == NULL) {
        res = LUTfoldLutP(map->lut, start, foldfun);
    } else {
        res = LUTfoldLutS(map->lut, start, foldfun);
    }

    res;
}
