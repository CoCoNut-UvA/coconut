#include "commandline.h"
#include "ccngen/ast.h"
#include "ccn/phase_driver.h"
#include <stddef.h>
#include <stdio.h>
#include "globals.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

struct globals globals;

int main(int argc, char *argv[])
{
    CLprocessArgs(argc, argv);
    globals.filename = global_command_line.input_file;
    globals.line_map = HTnew_Int(25);
    globals.gen_hdr_dir = STRcat(global_command_line.gen_dir, "ccngen/");
    globals.gen_src_dir = global_command_line.gen_dir;
    struct ccn_node *node = NULL;
    node = CCNstart(node);
    TRAVstart(node, TRAV_free);

    return 0;
}
