
#include "ccngen/ast.h"
#include "commandline.h"
#include <stdio.h>

extern node_st *SPparseDSL(FILE *);

node_st *scanAndParse(node_st *root)
{
    FILE *fp = fopen(global_command_line.input_file, "r");
    if (fp == NULL) {
        return NULL;
    }
    return SPparseDSL(fp);
}