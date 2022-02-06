#include "filesystem/utils.h"
#include "palm/str.h"

char *FSdirifyString(char *path)
{
    return path[STRlen(path) - 1] != '/' ? STRcat(path, "/") : STRcpy(path);
}
