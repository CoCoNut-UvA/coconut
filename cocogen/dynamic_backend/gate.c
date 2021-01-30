#include <stdbool.h>
#include "palm/str.h"
#include "globals.h"

bool backendIsDynamic()
{
    return STReqci("dynamic", globals.backend);
}