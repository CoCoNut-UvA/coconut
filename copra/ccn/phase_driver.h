#pragma once

#include <stdbool.h>
#include <stddef.h>

void CCNrun(struct ccn_node *node);
void CCNcycleNotify();
size_t CCNgetCurrentActionId();
void CCNerrorPhase();
void CCNerrorAction();