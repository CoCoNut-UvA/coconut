#pragma once

#include <stdlib.h>

extern void *MEMmalloc(size_t size);
extern void *MEMfree(void *address);
extern void *MEMcopy(size_t size, void *mem);
