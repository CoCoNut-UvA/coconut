#include <stddef.h>
#include "generated/ast.h"
#include "generated/trav-ast.h"

struct Info {
    int a;
};

Info *Print_createinfo(void) { return NULL; }

void Print_freeinfo(Info *i) { }

void Print_BinOp(BinOp *b, Info *i)
{
    trav_BinOp_left(b, i);
    trav_BinOp_right(b, i);
}

void Print_Num(Num *n, Info *i)
{
    printf("Num: %d\n", n->value);
}
