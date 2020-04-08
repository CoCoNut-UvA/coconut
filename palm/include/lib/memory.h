#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdlib.h>

void *mem_alloc(size_t size);

void mem_free(void *ptr);

void *mem_copy(const void *src, size_t len);

void set_allocator(void *(*_allocator)(size_t));

void set_deallocator(void (*_deallocator)(void*));

#endif /* _MEMORY_H_ */
