#include "ccngen/ast.h"
#include "globals.h"
#include "palm/str.h"

bool backendIsStatic()
{
    return STReq("static", globals.backend);
}
