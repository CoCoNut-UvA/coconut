#include <stdbool.h>
#include "core/phase_driver.h"
#include "generated/ast.h"
#include "generated/create-ast.h"

BinOp *pass_CreateAST_entry(BinOp* bin)
{
    Num *left = create_Num(10);
    Num *right = create_Num(100);
    BinOp *b = create_BinOp(left, right);
    
    return b;
}

int main() {
    ccn_phase_driver_init();
    
    ccn_phase_driver_start();

    ccn_phase_driver_destroy();
    return 0;
}
