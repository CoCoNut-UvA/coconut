#include "ccn/phase_driver.h"
#include "ccngen/ast.h"
#include "commandline.h"
#include "filesystem/gen_files.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "palm/filesystem.h"
#include "palm/str.h"
#include <ccn/ccn.h>
#include <stddef.h>
#include <stdio.h>

struct globals globals;


/* Last pass to run before printing. Outputs a file with #define statement
 * to enable/disable certain parts of CoCoNut.
 */
node_st *genDefines(node_st *ast)
{
    int indent = 0;
    FILE *fp = FSgetIncludeFile("ccn_defs.h");

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
    globals.filename = global_command_line.input_file;
    globals.line_map = HTnew_Int(25);
    globals.gen_hdr_dir = STRcat(global_command_line.gen_dir, "ccngen/");
    globals.gen_user_dir = STRcat(global_command_line.gen_dir, "user/");
    globals.gen_src_dir = global_command_line.gen_dir;
    globals.show_ast = global_command_line.show_ast;
    if (global_command_line.backend == NULL) {
        globals.backend = "dynamic";
    } else {
        globals.backend = global_command_line.backend;
    }

    FSensureDirExists(global_command_line.gen_dir, 0755);
    FSensureDirExists(globals.gen_hdr_dir, 0755);
    FSensureDirExists(globals.gen_user_dir, 0755);

    CCNrun(NULL);

    return 0;
}
