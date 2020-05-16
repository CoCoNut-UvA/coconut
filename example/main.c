#include "generated/create-ast.h"
#include "generated/ast-IntConst.h"

int main() {
    IntConst *c = create_IntConst(10);
    printf("YUEA!");
    printf("Val: %d\n", c->value);
}
