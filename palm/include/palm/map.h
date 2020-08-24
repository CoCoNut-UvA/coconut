#pragma once

#include <stdbool.h>

typedef struct MAP_ST map_st;

extern map_st *MAPcreate(bool (*eq)(void *, void *));
extern map_st *MAPcopy(map_st *map_st);
extern map_st *MAPdelete(map_st *map_st);
extern map_st *MAPclear(map_st *map_st);
extern bool MAPisEmpty(map_st *map_st);
extern map_st *MAPinsert(map_st *map_st, void *key, void *item);
extern void **MAPlookup(map_st *map_st, void *key);
extern map_st *MAPupdate(map_st *map_st, void *key, void *item);
extern map_st *MAPswap(map_st *map_st, void *key, void *item, void **found);
extern void MAPmap_st(map_st *map_st, void *(*map_stfun)(void *));
extern void *MAPfold(map_st *map_st, void *start, void *(*foldfun)(void *, void *));
