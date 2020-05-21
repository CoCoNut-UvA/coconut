#include "core/consist_checks.h"
#include "lib/print.h"
#include <stdio.h>
#include <stdlib.h>

void ccn_check_disallowed(void *val, size_t offset, const char *node_name, const char *child_name)
{
    char *base = (char*)val;
    void **ref = (void**)(base + offset);
    if (*ref == NULL) {
        if (child_name != NULL) {
            print_user_error("CCN CHK", "Child %s on node type %s is disallowed but is present.", child_name, node_name);
        } else {
            print_user_error("CCN CHK", "Node %s is disallowed but present in tree.", node_name);
        }
        abort();
    }
}

void ccn_check_mandatory(void *val, size_t offset, const char *node_name, const char *child_name)
{
    char *base = (char*)val;
    void **ref = (void**)(base + offset);
    if (*ref == NULL) {
        print_user_error("CCN CHK", "Child %s on node type %s is mandatory but not present.", child_name, node_name);
        abort();
    }

}