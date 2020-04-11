#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "commandline.h"
#include "lib/color.h"
#include "lib/errors.h"
#include "ast/ast.h"
#include "ast/check.h"
#include "pretty/printer.h"

// Defined in the parser.
extern struct Config *parseDSL(FILE *fp);
extern char *yy_filename;

void exit_compile_error(void) {
    PRINT_COLOR(MAGENTA);
    fprintf(stderr, "Errors where found, code generation terminated.\n");
    PRINT_COLOR(RESET_COLOR);
    exit(INVALID_CONFIG);
}

int main(int argc, char *argv[])
{
    process_commandline_args(argc, argv);
    FILE *f = fopen(yy_filename, "r");
    Config *ir = parseDSL(f);
    if (check_config(ir)) {
        exit_compile_error();
    }

    pretty_print(ir);
}