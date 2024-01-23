#pragma once

#include <sys/types.h>
#include <dirent.h>

/**
 * It calls err if the directory could not be created.
 * @param dirname Directory to ensure.
 * @param mode Mode in which directory is to be created.
 */
void FSensureDirExists(char *dirname, mode_t mode);
