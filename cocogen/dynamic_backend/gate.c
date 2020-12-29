#include <stdbool.h>
#include "palm/str.h"
#include "globals.h"

bool backendIsDynamic()
{
    return STReq("dynamic", globals.backend);
}