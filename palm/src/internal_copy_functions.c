#include <assert.h>
#include <string.h>

#include "lib/str.h"

void *id_copy(const void *item) {
    assert(item != NULL);
    char *original = (char *)item;
    char *copy = ccn_str_cpy(original);
    return copy;
}
