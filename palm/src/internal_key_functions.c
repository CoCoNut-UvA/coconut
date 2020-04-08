
#include "lib/str.h"

char *id_key(void *id) {
    char *original = (char *)id;
    return ccn_str_cpy(original);
}
