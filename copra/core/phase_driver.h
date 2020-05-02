#include <stddef.h>

void ccn_phase_driver_init();
void ccn_phase_driver_destroy();
bool ccn_set_breakpoint(char *breakpoint);
bool ccn_set_inspectpoint(char *inspectpoints);
void ccn_phase_driver_start();
void ccn_print_statistic(int n, int m);
void ccn_print_time_statistics(int n);
void ccn_print_mem_statistics(int m);