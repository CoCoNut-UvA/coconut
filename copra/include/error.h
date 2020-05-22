#pragma once

enum CCN_Error { CCN_FATAL, CCN_NON_FATAL, CCN_ACTION_FATAL, CCN_PHASE_FATAL };

void ccn_notify_error(enum CCN_Error error_type);
