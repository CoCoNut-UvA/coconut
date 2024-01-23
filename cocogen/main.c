#include "ccn/phase_driver.h"
#include "ccngen/ast.h"
#include "commandline.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/filesystem.h"
#include "palm/str.h"
#include <ccn/ccn.h>
#include "palm/dbug.h"
#include <stddef.h>
#include <stdio.h>
#include "ccngen/action_handling.h"
#include "ccn/action_types.h"

struct globals globals;

/* Do not detect memory leaks when using AddressSanitizer. */
const char *__asan_default_options() {
  return "detect_leaks=0";
}

/* Last pass to run before printing. Outputs a file with #define statement
 * to enable/disable certain parts of CoCoNut.
 */
node_st *genDefines(node_st *ast)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "ccn_defs.h");

    if (AST_USES_UNSAFE(ast)) {
        OUT("#define CCN_USES_UNSAFE true\n");
    }
    return ast;
}

void BreakpointHandler(node_st *node)
{
    TRAVstart(node, TRAV_PRT);
}



int main(int argc, char *argv[])
{
    CLprocessArgs(argc, argv);
    if (!global_command_line.debug) {
        DBUGoff();
    }

    globals.filename = global_command_line.input_file;
    globals.line_map = HTnew_Int(25);
    globals.gen_hdr_dir = STRcat(global_command_line.gen_dir, "ccngen/");
    globals.gen_eqhdr_dir = STRcat(globals.gen_hdr_dir, "equations/");
    globals.gen_user_dir = STRcat(global_command_line.gen_dir, "user/");
    globals.gen_ag_dot_dir = STRcat(global_command_line.gen_dir, "ag_dot/");
    globals.log_dir = STRcat(global_command_line.gen_dir, "log/");;
    globals.gen_src_dir = global_command_line.gen_dir;
    globals.show_ast = global_command_line.show_ast;
    globals.gen_ctx = GNnew();
    globals.ag_scheduler_max_iter = global_command_line.ag_scheduler_max_iter;

    if (global_command_line.backend == NULL) {
        globals.backend = "dynamic";
    } else {
        globals.backend = global_command_line.backend;
    }

    FSensureDirExists(global_command_line.gen_dir, 0755);
    FSensureDirExists(globals.gen_hdr_dir, 0755);
    FSensureDirExists(globals.gen_eqhdr_dir, 0755);
    FSensureDirExists(globals.gen_user_dir, 0755);
    FSensureDirExists(globals.gen_ag_dot_dir, 0755);
    FSensureDirExists(globals.log_dir, 0755);

    printf("--------------- Generating ---------------\n");
    CCNrun(NULL);
    printf("---------------    Done    ---------------\n");

    return 0;
}
