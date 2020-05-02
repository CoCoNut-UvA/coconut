#include "core/break_inspect_points.h"
#include "generated/ccn_enables.h"
#include <stdbool.h>
#include "core/internal_phase_functions.h"

#include "generated/ccn_enables.h"

bool _ccn_set_breakpoint(char *breakpoint) {
#ifndef CCN_ENABLE_POINTS
    return false;
#else
    phase_driver_t *phase_driver = _get_phase_driver();
    if (!phase_driver->initialized || phase_driver->breakpoint != NULL)
        return false;
    point_frame_t *frame = _ccn_create_point_frame_from_string(breakpoint);
    if (frame == NULL)
        return false;

    phase_driver->breakpoint = frame;
    return true;
#endif
}


bool _ccn_set_inspect_point(char *inspect) {
#ifndef CCN_ENABLE_POINTS
    return false;
#else
    phase_driver_t *phase_driver = _get_phase_driver();
    if (!phase_driver->initialized)
        return false;

    if (phase_driver->inspection_points == NULL) {
        phase_driver->inspection_points = array_init(2);
    }

    point_frame_t *frame = _ccn_create_point_frame_from_string(inspect);
    if (frame == NULL)
        return false;

    array_append(phase_driver->inspection_points, frame);
    return true;
#endif
}