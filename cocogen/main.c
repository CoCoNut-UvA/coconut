#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast/ast.h"
#include "ast/check.h"
#include "ast/free.h"
#include "commandline.h"
#include "dynamic_backend/API.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/color.h"
#include "lib/errors.h"
#include "lib/str.h"
#include "pretty/printer.h"
#include "typed_backend/API.h"

// Defined in the parser.
extern struct Config *parseDSL(FILE *fp);
extern char *yy_filename;

void exit_compile_error(void) {
    PRINT_COLOR(MAGENTA);
    fprintf(stderr, "Errors where found, code generation terminated.\n");
    PRINT_COLOR(RESET_COLOR);
    exit(INVALID_CONFIG);
}

// TODO: move this.
void generate_enables(Config *c, FILE *fp) {
    if (global_command_options.break_inspect_points) {
        out("#define CCN_ENABLE_POINTS 1\n");
    } else {
        out("#undef CCN_ENABLE_POINTS\n");
    }

    if (global_command_options.consistcheck) {
        out("#define CCN_ENABLE_CHECKS 1\n");
    } else {
        out("#undef CCN_ENABLE_CHECKS\n");
    }

    out("\n");
}

int main(int argc, char *argv[]) {
    process_commandline_args(argc, argv);
    FILE *f = fopen(yy_filename, "r");
    Config *ir = parseDSL(f);
    if (check_config(ir)) {
        exit_compile_error();
    }

    // TODO(damian): Create recursive ensure_dir_exists
    ensure_dir_exists("generated/", 0777);
    ensure_dir_exists("generated/include/", 0777);
    ensure_dir_exists("generated/include/ccngen/", 0777);
    ensure_dir_exists("generated/src/", 0777);
    if (global_command_options.gen_user_files) {
        if (global_command_options.user_dir == NULL) {
            ensure_dir_exists("user/", 0777);
            ensure_dir_exists("user/src/", 0777);
            global_command_options.user_dir = "user/src/";

        } else {
            ensure_dir_exists(global_command_options.user_dir, 0777);
        }
    }

    init_tracking_data(2);
    filegen_init(ir, false);

    // pretty_print(ir);

    // TODO: Maybe handle this better
    if (global_command_options.backend == NULL) {
        PRINT_COLOR(MAGENTA);
        fprintf(stderr, "No backend provided. Supported backends are "
                        "\"typed\" or \"dynamic\".\n");
        PRINT_COLOR(RESET_COLOR);
        exit(INVALID_BACKEND);
    } else if (ccn_str_equal(global_command_options.backend, "typed")) {
        /// TODO: (Damian) fix this
        // typed_backend(ir);
    } else if (ccn_str_equal(global_command_options.backend, "dynamic")) {
        dynamic_backend(ir);
    } else {
        PRINT_COLOR(MAGENTA);
        fprintf(stderr, "Invalid backend provided. Supported backends are "
                        "\"typed\" or \"dynamic\".\n");
        PRINT_COLOR(RESET_COLOR);
        exit(INVALID_BACKEND);
    }

    cleanup_tracking_data();
    free_config(ir);
}
