#include <stdlib.h>
#include <stddef.h>
#include "lib/smap.h"
#include "lib/memory.h"

typedef struct allocation_frame {
    char *file;
    char *function;
    size_t line;
    size_t allocated;

} allocation_frame_t;

smap_t *allocation_tracker = NULL;

void init_allocation_tracker(size_t size) {
    allocation_tracker = smap_init(size);
}

void *_ccn_wrapper_malloc(size_t size, char *file, size_t line, char *function) {
    void *item = malloc(size);
    if (item == NULL) // Possible error?
        return NULL;
    return item;
}

void _ccn_wrapper_free(void* item) {
    //phase_driver.total_freed += malloc_usable_size(item); // glib deps, so not crossplatform.
    free(item);
}