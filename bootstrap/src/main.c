#include "commandline.h"
#include "ccngen/ast.h"
#include "ccn/phase_driver.h"
#include <stddef.h>
#include "globals.h"

struct globals globals;

int main(int argc, char *argv[])
{
    CLprocessArgs(argc, argv);
    globals.line_map = HTnew_Int(25);
    struct ccn_node *node = NULL;
    node = CCNstart(node);

    return 0;
}
