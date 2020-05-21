#include "core/error.h"
#include "core/internal_phase_functions.h"

#include <stdlib.h>

// TODO: maybe add a bit more information?
static inline void error_fatal() {
    exit(1);
}

static inline void error_non_fatal() {
    phase_driver_t *pd = _get_phase_driver();
    pd->non_fatal_error = true;
}

static inline void error_action_fatal() {
    phase_driver_t *pd = _get_phase_driver();
    pd->action_error = true;
}

static inline void error_phase_fatal() {
    phase_frame_t *frame = _top_frame();
    frame->phase_error = true;
}

void ccn_notify_error(enum CCN_Error error_type) {
    switch (error_type) {
    case CCN_FATAL:
        error_fatal();
        break;
    case CCN_ACTION_FATAL:
        error_action_fatal();
        break;
    case CCN_NON_FATAL:
        error_non_fatal();
        break;
    case CCN_PHASE_FATAL:
        error_phase_fatal();
        break;
    }
}
