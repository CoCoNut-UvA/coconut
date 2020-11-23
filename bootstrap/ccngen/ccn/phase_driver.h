#pragma once

#include <stdbool.h>
#include <stddef.h>

struct ccn_node *CCNstart(struct ccn_node *node);
size_t CCNgetCurrentActionId();
