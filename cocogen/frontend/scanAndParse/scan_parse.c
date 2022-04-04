
#include "ccngen/ast.h"
#include "commandline.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

extern node_st *SPparseDSL(FILE *);

node_st *SPscanAndParse(node_st *root)
{
    root = (void*)root; // unused error
    FILE *fp = fopen(global_command_line.input_file, "r");
    if (fp == NULL) {
        err(EXIT_FAILURE, "Can not open DSL file: %s", global_command_line.input_file);
    }
    return SPparseDSL(fp);
}
