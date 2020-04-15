#include <stdlib.h>

void *mem_alloc(int size) {
    void *ptr;

    if (size > 0) {
        ptr = malloc(size);
        if (ptr == NULL) {
            CTIabortOutOfMemory(size);
        }
    } else {
        ptr = NULL;
    }
    return ptr;
}

void *mem_free(void *address) {
    if (address != NULL) {
        free(address);
        address = NULL;
    }
    return address;
}

void *mem_copy(int size, void *mem) {
    void *result;
    result = mem_alloc(size);
    result = memcpy(result, mem, size);
    return result;
}
