#include "ast/ast.h"
#include "filegen/util.h"
#include  "filegen/driver.h"

// TODO(damian): make this in a gate_header and use the loop over phases to call this.
void generate_gate_headers(Config *config, FILE *fp) {
    out("#include <stdbool.h>\n");
    int indent = 0;
    for (int i = 0; i < array_size(config->phases); ++i) {
        Phase *phase = array_get(config->phases, i);
        if (phase->gate_func != NULL) {
            out_statement("bool %s()", phase->gate_func);
        }
    }
}