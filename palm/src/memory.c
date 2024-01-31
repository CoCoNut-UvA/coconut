#include "palm/memory.h"

#include <stdlib.h>
#include <string.h>

#include "palm/dbug.h"
#include "palm/ctinfo.h"


/**
 * Allocate memory. If memory can not be allocated this function
 * calls the CTIabortOufOfMemory function and exists.
 * @param size Amount to allocate.
 * @return A pointer to an allocated structure.
 */
void *MEMmalloc(size_t size)
{
    void *ptr;

    DBUG_ASSERT((size >= 0), "called with negative size!");

    if (size > 0) {
        /*
        * Since some UNIX system (e.g. ALPHA) do return NULL for size 0 as well
        * we do complain for ((NULL == tmp) && (size > 0)) only!!
        */
        ptr = malloc(size);

        if (ptr == NULL) {
            CTIabortOutOfMemory(size);
        }
    }
    else {
        ptr = NULL;
    }

    return ptr;
}

/**
 * Reallocate memory. If memory can not be allocated this function
 * calls the CTIabortOufOfMemory function and exists.
 * @param address address to resize, can be NULL. Address can not be used again
 * after call.
 * @param size Amount to allocate.
 * @return A pointer to an allocated structure.
 */
void *MEMrealloc(void *address, size_t size)
{
    if (address == NULL) {
        return MEMmalloc(size);
    }

    DBUG_ASSERT((size >= 0), "called with negative size!");

    void *ptr;

    if (size > 0) {
        /*
        * Since some UNIX system (e.g. ALPHA) do return NULL for size 0 as well
        * we do complain for ((NULL == tmp) && (size > 0)) only!!
        */
        ptr = realloc(address, size);

        if (ptr == NULL) {
            CTIabortOutOfMemory(size);
        }
    }
    else {
        ptr = NULL;
        MEMfree(address);
    }

    return ptr;
}

/**
 * Free memory. Returns NULL, but allows to do assignment to freed structure.
 * @param address address to free.
 */
void *MEMfree(void *address)
{
    if(address != NULL) {
        free(address);
        address = NULL;
    }

    return address;
}

void *MEMcopy(size_t size, void *mem)
{
    void *result;

    result = MEMmalloc(size);
    result = memcpy(result, mem, size);

    return result;
}
