#pragma once

#include <stddef.h>

struct QUitem {
    void *item;
    struct QUitem *next;
};

struct QUqueue {
    struct QUitem *top;
    struct QUitem *back;
    size_t size;
};

typedef struct QUitem item_st;
typedef struct QUqueue queue_st;

queue_st *QUcreate();
void QUdelete(queue_st *queue);
void QUinsert(queue_st *queue, void *item);
void *QUpeek(queue_st *queue);
void *QUpop(queue_st *queue);
