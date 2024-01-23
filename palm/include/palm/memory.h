#pragma once

#include <stdlib.h>

/**
 * Allocate memory. If memory can not be allocated this function
 * calls the CTIabortOufOfMemory function and exists.
 * @param size Amount to allocate.
 * @return A pointer to an allocated structure.
 */
extern void *MEMmalloc(size_t size);

/**
 * Reallocate memory. If memory can not be allocated this function
 * calls the CTIabortOufOfMemory function and exists.
 * @param address address to resize, can be NULL. Address can not be used again
 * after call.
 * @param size Amount to allocate.
 * @return A pointer to an allocated structure.
 */
extern void *MEMrealloc(void *address, size_t size);

/**
 * Free memory. Returns NULL, but allows to do assignment to freed structure.
 * @param address address to free.
 */
extern void *MEMfree(void *address);

/**
 * Copy memory to a new memory location.
 *
 * @param size amount of bytes to copy to new region.
 * @param mem Memory location to copy from.
 * @return New memory location of size bytes.
 */
extern void *MEMcopy(size_t size, void *mem);
