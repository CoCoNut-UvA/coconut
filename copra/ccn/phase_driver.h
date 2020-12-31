#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "ccngen/ast.h"

void CCNrun(node_st *node);
void CCNcycleNotify();
size_t CCNgetCurrentActionId();
void CCNerrorPhase();
void CCNerrorAction();
