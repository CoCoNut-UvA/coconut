#pragma once

#include "format/out_macros.h"
#include "format/gen-functions.h"

struct format_settings {
    char *basic_node_type;
    char *node_constructor_prefix;
    char *nodetype_enum_prefix;
    char *nodeset_enum_prefix;
    bool access_macro_uppercase;
};

void FMTprintIndentLevel(FILE *fp, int indent);
