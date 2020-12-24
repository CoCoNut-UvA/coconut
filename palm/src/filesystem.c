#include "palm/filesystem.h"

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <err.h>

void FSensureDirExists(char *dirname, __mode_t mode)
{
    if (mkdir(dirname, mode) == 0) {
        return;
    } else {
        if (errno == EEXIST) {
            DIR *d = opendir(dirname);

            // out_dir exists but is a file
            if (!d) {
                err(EXIT_FAILURE, "Trying to create directory \"%s\"", dirname);
            } else {
                // Directory already exists
                closedir(d);
            }
        } else {
            err(EXIT_FAILURE, "Trying to create directory \"%s\"", dirname);
        }
    }
}

