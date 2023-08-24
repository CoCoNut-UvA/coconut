#include "palm/memory.h"
#include "frontend/attribute_scheduler/queue.h"

queue_st *QUcreate() {
    queue_st *queue = MEMmalloc(sizeof(queue_st));
    queue->back = NULL;
    queue->top = NULL;
    queue->size = 0UL;

    return queue;
}

void QUdelete(queue_st *queue) {
    for (item_st *item = queue->top; item != NULL; item = item->next) {
        MEMfree(item);
    }

    MEMfree(queue);
}

void QUinsert(queue_st *queue, void *item) {
    item_st *queue_item = MEMmalloc(sizeof(item_st));
    queue_item->item = item;
    queue_item->next = NULL;

    if (queue->size == 0) {
        queue->top = queue_item;
        queue->back = queue_item;
    } else {
        queue->back->next = queue_item;
        queue->back = queue_item;
    }

    queue->size += 1;
}

void *QUpeek(queue_st *queue) {
    return queue->top;
}

void *QUpop(queue_st *queue) {
    if (queue->size == 0) {
        return NULL;
    }

    item_st *item = queue->top;
    if (queue->size == 1) { // popping last item
        queue->top = NULL;
        queue->back = NULL;
    } else {
        queue->top = item->next;
    }
    queue->size -= 1;

    void *content = item->item;
    MEMfree(item);
    return content;
}
