#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"
#include "gen_helpers/format.h"
#include "globals.h"

node_st *DGTDUast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_NO_INDENT("// Ignore empty struct warning\n");
    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic push\n");
    OUT_NO_INDENT("#pragma clang diagnostic ignored \"-Wgnu-empty-struct\"\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic push\n");
    OUT_NO_INDENT("#pragma GCC diagnostic ignored \"-Wpedantic\"\n");
    OUT_NO_INDENT("#endif\n\n");

    OUT_UNION("TRAV_DATA");
    TRAVitraversals(node);
    OUT_STRUCT_END();

    OUT_NO_INDENT("#if defined(__clang__)\n");
    OUT_NO_INDENT("#pragma clang diagnostic pop\n");
    OUT_NO_INDENT("#elif defined(__GNUC__)\n");
    OUT_NO_INDENT("#pragma GCC diagnostic pop\n");
    OUT_NO_INDENT("#endif\n\n");

    return node;
}


node_st *DGTDUitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT_FIELD("struct data_%s *%s", ID_LWR(ITRAVERSAL_IPREFIX(node)), ID_LWR(id));
    }
    TRAVnext(node);
    return node;
}
