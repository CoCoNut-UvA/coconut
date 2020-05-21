#include "core/internal_phase_functions.h"
#include "core/break_inspect_points.h"

void ccn_phase_driver_init() {
    return _initialize_phase_driver();
}

void ccn_phase_driver_destroy() {
    return phase_driver_destroy();
}


bool ccn_set_breakpoint(char *breakpoint) {
    _ccn_set_breakpoint(breakpoint);
}

bool ccn_set_inspectpoint(char *inspect) {
    _ccn_set_inspect_point(inspect);
}

void ccn_phase_driver_start() {
    _ccn_phase_driver_start();
}
void ccn_print_time_statistics(int n) {
    _ccn_print_top_n_time(n);
}

void ccn_print_mem_statistics(int m) {

}

void ccn_print_statistic(int n, int m) {
    ccn_print_time_statistics(n);
    ccn_print_time_statistics(m);
}