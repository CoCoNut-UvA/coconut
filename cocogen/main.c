#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "commandline.h"
#include "lib/color.h"
#include "lib/errors.h"
#include "ast/ast.h"
#include "ast/check.h"
#include "filegen/driver.h"
#include "filegen/util.h"
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

int main(int argc, char *argv[])
{
    process_commandline_args(argc, argv);
    FILE *f = fopen(yy_filename, "r");
    Config *config = parseDSL(f);
    if (check_config(config)) {
        exit_compile_error();
    }

    // TODO(damian): Create recursive ensure_dir_exists
    if (global_command_options.header_dir == NULL) {
        global_command_options.header_dir = "generated/include/generated/";
        ensure_dir_exists("generated/", 0777); //FIXME(damian): transform to recursive.
        ensure_dir_exists("generated/include/", 0777);
        ensure_dir_exists("generated/include/generated", 0777);
    }
    ensure_dir_exists(global_command_options.header_dir, 0777);

    if (global_command_options.source_dir == NULL) {
        global_command_options.source_dir = "generated/source/";
        ensure_dir_exists("generated/", 0777); //FIXME(damian): transform to recursive.
        ensure_dir_exists("generated/source/", 0777);

    }
    ensure_dir_exists(global_command_options.source_dir, 0777);

    init_tracking_data(2);
    filegen_init(ir, false);

    // TODO: add commandline flags for the right backend.
    typed_backend(ir);
    pretty_print(ir);


    cleanup_tracking_data();
}