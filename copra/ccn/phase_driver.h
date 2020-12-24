#pragma once

#include <stdbool.h>
#include <stddef.h>

void CCNrun(struct ccn_node *node);
size_t CCNgetCurrentActionId();
