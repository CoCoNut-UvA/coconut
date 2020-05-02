
#include "core/internal_phase_functions.h"

#include <stddef.h>

void ccn_cycle_notify() {
    phase_frame_t *top = _top_frame();
    top->cycle_notified = true;
    if (top->curr_mark != NULL) {
        top->curr_mark->notified = true;
    }
}

void ccn_cycle_mark_all() {
    phase_frame_t *top = _top_frame();
    top->cycle_notified = false;
    if (top->marks != NULL) {
        for (int i = 0; i < array_size(top->marks); ++i) {
            cycle_mark_t *mark = array_get(top->marks, i);
            mark->notified = false;
        }
    }
}

void ccn_cycle_mark_reset() {
    phase_frame_t *top = _top_frame();
    top->cycle_notified = true;
    if (top->marks != NULL) {
        for (int i = 0; i < array_size(top->marks); ++i) {
            cycle_mark_t *mark = array_get(top->marks, i);
            mark->notified = true;
        }
    }
}