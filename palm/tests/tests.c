#include <stdio.h>
#include <stddef.h>

#include "lib/str.h"
#include "lib/array.h"
#include "lib/assert.h"
#include "lib/log.h"
#include <assert.h>

void log_test() {
    ccn_log_debug("Hello debug test");
    ccn_log_info("Information here");
    ccn_log_warn("Warnings required");
    ccn_log_error("Error happened");
}

void assert_test() {
    ccn_assert(2 > 1, "Two is bigger than one");
    //ccn_contract_in(NULL != NULL);
}

int main(int argc, char **argv) {
    char *split_test = "split.on.dots";
    array *splits = ccn_str_split(split_test, '.');
    for (int i = 0; i < array_size(splits); ++i) {
        printf("Split: %s\n", (char*)array_get(splits, i));
    }

    char *catted_n = ccn_str_cat_n(3, "Hello", " World", " Catted");
    printf("%s\n", catted_n);

    array *array_string = create_array();
    for (int i = 0; i < 3; i++) {
        char *res = ccn_str_cpy("Hello ");
        array_append(array_string, res);
    }

    char *array_catted = ccn_str_cat_array(array_string);
    printf("%s\n", array_catted);

    log_test();
    assert_test();

    return 0;
}
