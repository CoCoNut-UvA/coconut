#include "commandline.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem/utils.h"
#include "ccn/ccn.h"

extern char *yy_filename;

struct command_line global_command_line;

static
void InitGlobalOptions() {
    global_command_line.verbose = 0;
    global_command_line.break_inspect_points = false;
    global_command_line.consistcheck = false;
    global_command_line.profiling = false;
    global_command_line.serialise = false;
    global_command_line.gen_user_files = false;
    global_command_line.show_ast = false;
    global_command_line.backend = NULL;
    global_command_line.gen_dir = NULL;
    global_command_line.debug = false;
}

static
void Usage(char *program) {
    char *program_bin = strrchr(program, '/');
    if (program_bin)
        program = program_bin + 1;

    printf("Usage: %s [OPTION...] COCONUT_FILE\n", program);
    printf("Options:\n");
    printf("  --help                       This help message.\n");
    /*
    printf("  --list-gen-files             Outputs a list of files which "
           "would be (re)generated,\n");
    printf("                               but does not actually modify any "
           "files.(NOT IMPLEMENTED)\n");
    */
    printf("  --verbose/-v                 Enable verbose mode.\n");
    printf("  --breakpoint/-b <breakpoint> Set a breakpoint.\n");
    //printf("  --dot                        Will produce ast.dot in "
//"<gen_dir>/dot.\n");
    printf("  --consistency-checks         Do consistency checks on the AST "
           "during runtime.\n");
    //printf("  --profiling                  Generate the requirements for a "
//           "time and memory profile in your compiler.\n");
    //printf("  --breakpoints                Enable setting breakpoints in your "
 //          "compiler, generates an API for this.\n");
    //printf("  --gen_user_files             Generates user traversal files.\n");
    printf("  --backend <backend name>     Selects generation backend, either "
           "typed or dynamic(default=dynamic).\n");
    printf("  --show-ast                   Pretty print the ast at end of compilation\n");
    printf("  --show-structure             Pretty print the structure of the compiler.\n");
}

static
void Version(void) { printf("cocogen 0.1\nCoCoNut Metacompiler\n"); }

void CLprocessArgs(int argc, char *argv[]) {
    InitGlobalOptions();

    // TODO: remove the hardcoded identifiers.
    static struct option long_options[] = {
        {"verbose", no_argument, &global_command_line.verbose, 1},
        {"help", no_argument, 0, 'h'},
        {"dot", required_argument, 0, 23},
        {"version", no_argument, 0, 20},
        {"profiling", no_argument, 0, 24},
        {"breakpoints", no_argument, 0, 25},
        {"consistency-checks", no_argument, 0, 26},
        {"serialise", no_argument, 0, 27},
        {"inspectpoints", no_argument, 0, 28},
        {"backend", required_argument, 0, 30},
        {"gen-dir", required_argument, 0, 31},
        {"show-ast", no_argument, 0, 32},
        {"breakpoint", required_argument, 0, 33},
        {"show-structure", no_argument, 0, 34},
        {"debug", no_argument, 0, 35},
        {0, 0, 0, 0}};

    int option_index;
    int c;

    while (1) {
        c = getopt_long(argc, argv, "v", long_options, &option_index);

        // End of options
        if (c == -1)
            break;

        switch (c) {
        case 20:
            Version();
            exit(EXIT_SUCCESS);
        case 'v':
            CCNsetVerbosity(PD_V_MEDIUM);
            global_command_line.verbose = 1;
            break;
        case 24:
            global_command_line.profiling = true;
            break;
        case 25:
            global_command_line.break_inspect_points = true;
            break;
        case 26:
            global_command_line.consistcheck = true;
            break;
        case 27:
            global_command_line.serialise = true;
            break;
        case 28:
            global_command_line.break_inspect_points = true;
            break;
        case 29:
            global_command_line.gen_user_files = true;
            break;
        case 30:
            global_command_line.backend = optarg;
            break;
        case 31:
            global_command_line.gen_dir = optarg;
            break;
        case 32:
            global_command_line.show_ast = true;
            break;
        case 33:
            CCNsetBreakpoint(optarg);
            break;
        case 34:
            CCNshowTree();
            exit(0);
        case 35:
            global_command_line.debug = true;
            break;
        case 'h':
            Usage(argv[0]);
            exit(EXIT_SUCCESS);
        case '?':
            Usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (global_command_line.gen_dir == NULL) {
        global_command_line.gen_dir = "ccngen/";
    }
    global_command_line.gen_dir = FSdirifyString(global_command_line.gen_dir);

    if (optind == argc - 1) {
        global_command_line.input_file = argv[optind];
    } else {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
}
