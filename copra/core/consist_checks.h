#include <stddef.h>

void ccn_check_disallowed(void *val, size_t offset, const char *node_name, const char *child_name);
void ccn_check_mandatory(void *val, size_t offset, const char *node_name, const char *child_name);