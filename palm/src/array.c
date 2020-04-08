#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "lib/array.h"
#include "lib/memory.h"

struct array {
    void **data;
    size_t size;
    size_t capacity;
};

array *create_array(void) {
    return array_init(32);
}

struct array *array_init(long initial_capacity) {
    assert(initial_capacity > 0);

    struct array *a = mem_alloc(sizeof(struct array));
    a->data = (void **)mem_alloc(initial_capacity * sizeof(void *));
    a->size = 0;
    a->capacity = initial_capacity;
    return a;
}

static void noop(void *p) {}

void array_cleanup(struct array *a, void free_func(void *)) {
    if (a == NULL)
        return;
    void *e;
    if (free_func == NULL) {
        free_func = noop;
    }
    while ((e = array_pop(a))) {
        free_func(e);
    }
    free(a->data);
    free(a);
}

// TODO: make conditional a contract.
int array_set(struct array *a, int index, void *p) {
    if (index >= a->size)
        return -1;

    a->data[index] = p;
    return 0;
}

// TODO: probably make the if in a in_contract.
void *array_get(const struct array *a, int index) {
    if (index < a->size)
        return a->data[index];
    else
        return NULL;
}

int array_append(struct array *a, void *p) {
    if (a->size == a->capacity) {
        a->capacity *= 2;
        a->data = realloc(a->data, a->capacity * sizeof(void *));
    }
    a->size++;
    return array_set(a, a->size - 1, p);
}

void *array_pop(struct array *a) {
    if (a->size == 0)
        return NULL;

    void *last = a->data[a->size - 1];
    a->size--;
    return last;
}

size_t array_size(const struct array *a) {
    if (a == NULL)
        return 0;
    return a->size;
}

void array_clear(struct array *a) {
    a->size = 0;
}

void array_sort(struct array *a, int (*compare)(const void *, const void *)) {
    qsort(a->data, a->size, sizeof(void *), compare);
}

void array_map(struct array *a, void (*func)(const void *)) {
    assert(a != NULL && func != NULL);
    const size_t size = array_size(a);
    for (int i = 0; i < size; ++i) {
        func(array_get(a, i));
    }
}

void *array_last(const struct array *a) {
    const int size = array_size(a);
    if (size == 0)
        return NULL;

    return array_get(a, size - 1);
}
