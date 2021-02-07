#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "ccngen/ast.h"

enum pd_verbosity {
    PD_V_QUIET = 0,
    PD_V_SMALL = 1,
    PD_V_MEDIUM = 2,
    PD_V_HIGH = 3,
};

void CCNrun(node_st *node);
void CCNcycleNotify();
size_t CCNgetCurrentActionId();
void CCNerrorPhase();
void CCNerrorAction();
void CCNsetBreakpoint(char *breakpoint);
void CCNsetBreakpointWithID(size_t id);
void CCNsetCycles(size_t cycle_count);
void CCNsetVerbosity(enum pd_verbosity type);
void CCNsetTreeCheck(bool enable);
void CCNshowTree();
