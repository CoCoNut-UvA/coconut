#pragma once
/**
 *
 * @file
 *
 * This file provides the interface for producing any kind of output during
 * compilation.
 *
 * We have 4 levels of verbosity controlled by the command line option -v
 * and the global variable verbose_level.
 *
 * Verbose level 0:
 *
 * Only error messages are printed.
 *
 * Verbose level 1:
 *
 * Error messages and warnings are printed.
 *
 * Verbose level 2:
 *
 * Error messages, warnings and basic compile time information, e.g. compiler
 * phases,  are printed.
 *
 * Verbose level 3:
 *
 * Error messages, warnings and full compile time information are printed.
 *
 *
 * Default values are 1 for the product version and 3 for the developer version.
 *
 */

#include <stdarg.h>
#include <stdbool.h>

/**
 * Object to describe location information of a compile time object.
 * Pass this to CTIobj function.
 *
 * Values that are 0/NULL are not displayed, but are handled.
 */
struct ctinfo {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    char *filename;
    char *line;
};

/**
 * Type of message for the CTI and CTIobj functions.
 * Determines the header of the message. The error and warning messages have a
 * counter counting the errors and warnings, which will be displayed on CTIabort
 * functions.
 */
enum cti_type {
    CTI_STATE,
    CTI_NOTE,
    CTI_WARN,
    CTI_ERROR,

};

/**
 * Installs interrupt handlers
 */
extern void CTIinstallInterruptHandlers(void);

/**
 * Yields useful line length for error messages
 *
 * @return Line length
 */
extern int CTIgetErrorMessageLineLength();

/**
 * Yields useful line length for warning messages
 *
 * @return Line length
 */
extern int CTIgetWarnMessageLineLength(void);

/**
 *
 * @return The number of warnings currently produced
 */
extern int CTIgetWarnings();

/**
 *
 * @return The number of errors currently produced.
 */
extern int CTIgetErrors();

/**
 * Terminates the compilation process with a suitable error message.
 *
 */
extern void CTIabortCompilation();

/**
 * Terminates the compilation process if errors have occurred.
 *
 */
extern void CTIabortOnError();

/**
 * produces a specific "out of memory" error message without file name and
 * line number and terminates the compilation process.
 *
 * This very special function is needed because the normal procedure of
 * formatting a message may require further llocation of memory, which in this
 * very case generates a vicious circle of error messages instead of terminating
 * compilation properly.
 *
 * @param request size of requested memory
 *
 */
extern void CTIabortOutOfMemory(unsigned int request);

/**
 * Prints a message to stderr with header according to the type.
 * @param type the type of message, see enum cti_type
 * @param newline Whether to append a newline to the output or not.
 * @param format printf style format string
 * @param ... variable args.
 */
extern void CTI(enum cti_type type, bool newline, const char *format, ...);

/**
 * See @CTI
 * @param obj an object of type struct ctinfo with extra information.
 */
extern void CTIobj(enum cti_type type, bool newline, struct ctinfo obj,
                   const char *format, ...);
