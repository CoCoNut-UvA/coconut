#include "format/format.h"

#include <stdio.h>


void FMTprintIndentLevel(FILE *fp, int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "    ");
    }
}