#include "ccn/phase_driver.h"

int main() {
    ccn_phase_driver_init();
    ccn_phase_driver_start();
    ccn_phase_driver_destroy();
}
