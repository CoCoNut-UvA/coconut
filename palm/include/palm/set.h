#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef char *(*set_key_ft)(void *);
typedef void *(*cpy_val_ft)(const void *);
typedef void *(*fold_ft)(void *acc, void *item);

typedef struct set set_st;

set_st *SETnew(size_t size, set_key_ft key_f);
void SETdelete(set_st *set);
set_st *SETcpy(set_st *set);
bool SETinsert(set_st *set, void *item);
void SETremove(set_st *set, void *item);
bool SETisMember(set_st *set, void *item);
void SETfold(set_st *set, fold_ft fun, void *init_acc);
size_t SETcardinality(set_st *set);
set_st *SETintersect(set_st *set1, set_st *set2);
set_st *SETunion(set_st *set1, set_st *set2);
set_st *SETdifference(set_st *set1, set_st *set2);


set_st *SETnew_String(size_t size);
void SETprint_String(set_st *set);