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

#include "palm/ctinfo.h"

#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "palm/dbug.h"
#include "palm/memory.h"
#include "palm/str.h"

static char *message_buffer = NULL;
static int message_buffer_size = 0;
static int message_line_length = 76;

static char *abort_message_header = "abort: ";
static char *error_message_header = "error: ";
static char *warn_message_header = "warning: ";
static char *state_message_header = "";
static char *note_message_header = "note: ";

static int errors = 0;
static int warnings = 0;

#define MAX_ITEM_NAME_LENGTH 255

/** <!--********************************************************************-->
 *
 * @fn void ProcessMessage( char *buffer, int line_length)
 *
 *   @brief  formats message according to line length
 *
 *           '@' characters are inserted into the buffer to represent line
 *           breaks.
 *
 *   @param buffer  message buffer
 *   @param line_length maximum line length
 *
 ******************************************************************************/

static void ProcessMessage(char *buffer, int line_length)
{
    int index, column, last_space;
    index = 0;
    last_space = 0;
    column = 0;

    while (buffer[index] != '\0') {
        if (buffer[index] == '\t') {
            buffer[index] = ' ';
        }

        if (buffer[index] == ' ') {
            last_space = index;
        }

        if (buffer[index] == '\n') {
            buffer[index] = '@';
            column = 0;
        } else {
            if (column == line_length) {
                if (buffer[last_space] == ' ') {
                    buffer[last_space] = '@';
                    column = index - last_space;
                } else {
                    break;
                }
            }
        }

        index++;
        column++;
    }
}

/** <!--********************************************************************-->
 *
 * @fn void Format2Buffer( const char *format, va_list arg_p)
 *
 *   @brief The message specified by format string and variable number
 *          of arguments is "printed" into the global message buffer.
 *          It is taken care of buffer overflows.
 *
 *   @param format  format string like in printf family of functions
 *
 ******************************************************************************/

static void Format2Buffer(const char *format, va_list arg_p)
{
    int len;
    va_list arg_p_copy;

    va_copy(arg_p_copy, arg_p);
    len = vsnprintf(message_buffer, message_buffer_size, format, arg_p_copy);
    va_end(arg_p_copy);

    if (len < 0) {
        DBUG_ASSERT((message_buffer_size == 0), "message buffer corruption");
        /*
     * Output error due to non-existing message buffer
     */

        len = 120;

        message_buffer = (char *)MEMmalloc(len + 2);
        message_buffer_size = len + 2;

        va_copy(arg_p_copy, arg_p);
        len = vsnprintf(message_buffer, message_buffer_size, format, arg_p_copy);
        va_end(arg_p_copy);
        DBUG_ASSERT((len >= 0), "message buffer corruption");
    }

    if (len >= message_buffer_size) {
        /* buffer too small  */

        MEMfree(message_buffer);
        message_buffer = (char *)MEMmalloc(len + 2);
        message_buffer_size = len + 2;

        va_copy(arg_p_copy, arg_p);
        len = vsnprintf(message_buffer, message_buffer_size, format, arg_p_copy);
        va_end(arg_p_copy);

        DBUG_ASSERT((len < message_buffer_size), "message buffer corruption");
    }
}

/** <!--********************************************************************-->
 *
 * @fn char *CTIgetErrorMessageVA( int line, const char *format, va_list arg_p)
 *
 *   @brief generates error message string
 *
 *          The message specified by format string and variable number
 *          of arguments is "printed" into the global message buffer.
 *          It is taken care of buffer overflows. Afterwards, the message
 *          is formatted to fit a certain line length and is printed to
 *          stderr.
 *
 *   @param format  format string like in printf family of functions
 *
 ******************************************************************************/

char *CTIgetErrorMessageVA(int line, const char *format, va_list arg_p)
{
    char *first_line, *res;

    Format2Buffer(format, arg_p);
    ProcessMessage(message_buffer,
        message_line_length - strlen(error_message_header));

    first_line = (char *)MEMmalloc(32 * sizeof(char));
    sprintf(first_line, "line %d @", line);
    res = STRcat(first_line, message_buffer);
    first_line = MEMfree(first_line);

    return res;
}

//TODO: make this inline with message length.

static void PrintLocation(struct ctinfo *info)
{
    if (info->line != NULL) {
        // We push 4 spaces to take the ' |> ' into account.
        fprintf(stderr, " |> %s\n    ", info->line);

        int c = 1;
        while (c < info->first_column) {
            fputc(' ', stderr);
            c++;
        }

        fputc('^', stderr);

        c++;

        while (c <= info->last_column) {
            fputc('~', stderr);
            c++;
        }
        fputc('\n', stderr);
    }
}

static void PrintLine(struct ctinfo *info)
{
    fprintf(stderr, " --> %s:%d\n", info->filename, info->first_line);
}

static void PrintLineCol(struct ctinfo *info)
{
    fprintf(stderr, " --> %s:%d:%d\n", info->filename, info->first_line, info->first_column);
    PrintLocation(info);
}

static void PrintRange(struct ctinfo *info)
{
    if (info->last_line == 0 || info->first_line == info->last_line) {
        fprintf(stderr, " --> %s:%d.%d-%d\n", info->filename, info->first_line, info->first_column, info->last_column);
    } else {
        fprintf(stderr, " --> %s:%d.%d-%d.%d\n", info->filename, info->first_line, info->first_column, info->last_line, info->last_column);
    }
    PrintLocation(info);
}

static void PrintInfo(struct ctinfo *info)
{
    if (!info) {
        return;
    }

    if (info->first_line > 0 && info->first_column <= 0) {
        PrintLine(info);
    } else if (info->last_column <= 0 || ((info->last_line <= 0 || info->first_line == info->last_line) && info->first_column == info->last_column)) {
        PrintLineCol(info);
    } else if (info->last_column > 0) {
        PrintRange(info);
    }
}

/** <!--********************************************************************-->
 *
 * @fn void PrintMessage( const char *header, const char *format, va_list arg_p)
 *
 *   @brief prints message
 *
 *          The message specified by format string and variable number
 *          of arguments is "printed" into the global message buffer.
 *          It is taken care of buffer overflows. Afterwards, the message
 *          is formatted to fit a certain line length and is printed to
 *          stderr.
 *
 *   @param header  string which precedes each line of the message, e.g.
                    ERROR or WARNING.
 *   @param format  format string like in printf family of functions
 *
 ******************************************************************************/

static void PrintMessage(char *header, const char *format, va_list arg_p, bool newline, struct ctinfo *info)
{
    char *line;
    bool first_line = true;

    Format2Buffer(format, arg_p);

    ProcessMessage(message_buffer, message_line_length - strlen(header));

    line = strtok(message_buffer, "@");

    while (line != NULL) {
        fprintf(stderr, "%s%s\n", header, line);
        header = "";
        line = strtok(NULL, "@");
    }

    PrintInfo(info);

    if (newline) {
        fprintf(stderr, "\n");
    }
}

/** <!--********************************************************************-->
 *
 * @fn static void CleanUp()
 *
 *   @brief  does som clean up upon termination
 *
 *
 ******************************************************************************/

static void CleanUp()
{
}

/** <!--********************************************************************-->
 *
 * @fn void AbortCompilation()
 *
 *   @brief  terminates the compilation process with a suitable error message.
 *
 ******************************************************************************/

static void AbortCompilation()
{
    fprintf(stderr, "\n*** Compilation failed ***\n");
    fprintf(stderr, "*** %d Error(s), %d Warning(s)\n\n",
        errors, warnings);

    CleanUp();

    exit(1);
}

void CTIabortCompilation()
{
    AbortCompilation();
}

/** <!--********************************************************************-->
 *
 * @fn void InternalCompilerErrorBreak( int sig)
 *
 *   @brief  interrupt handler for segmentation faults and bus errors
 *
 *           An error message is produced and a bug report is created which
 *           may be sent via email to an appropriate address.
 *           Temporary files are deleted and the compilation process
 *           terminated.
 *
 *           DBUG_ENTER/RETURN are omitted on purpose to reduce risk of
 *           creating more errors during error handling.
 *
 *   @param sig  signal causing interrupt
 *
 ******************************************************************************/

static void InternalCompilerErrorBreak(int sig)
{
    fprintf(stderr,
        "\n\n"
        "OOOPS your program crashed the compiler 8-((\n\n");

    CleanUp();

    exit(1);
}

/** <!--********************************************************************-->
 *
 * @fn void UserForcedBreak( int sig)
 *
 *   @brief  interrupt handler for user-forced breaks like CTRL-C
 *
 *           Temporary files are deleted and the compilation process
 *           terminated.
 *
 *           DBUG_ENTER/RETURN are omitted on purpose to reduce risk of
 *           creating more errors during error handling.
 *
 *   @param sig  signal causing interrupt
 *
 ******************************************************************************/

static void UserForcedBreak(int sig)
{
    CleanUp();
    exit(0);
}

/** <!--********************************************************************-->
 *
 * @fn void CTIinstallInterruptHandlers( void)
 *
 *   @brief  installs interrupt handlers
 *
 ******************************************************************************/

void CTIinstallInterruptHandlers(void)
{
    signal(SIGSEGV, InternalCompilerErrorBreak); /* Segmentation Fault */
    signal(SIGBUS, InternalCompilerErrorBreak); /* Bus Error */
    signal(SIGINT, UserForcedBreak); /* Interrupt (Control-C) */
}


/** <!--********************************************************************-->
 *
 * @fn int CTIgetErrorMessageLineLength( void)
 *
 *   @brief  yields useful line length for error messages
 *
 *   @return line length
 *
 ******************************************************************************/

int CTIgetErrorMessageLineLength(void)
{
    return message_line_length - strlen(error_message_header);
}

/** <!--********************************************************************-->
 *
 * @fn void CTIabortOnError( void)
 *
 *   @brief  terminates compilation process if errors have occurred.
 *
 ******************************************************************************/

void CTIabortOnError(void)
{
    if (errors > 0) {
        AbortCompilation();
    }
}

/**
 * @return The number of errors currently produced.
 */
int CTIgetErrors()
{
    return errors;
}





/** <!--********************************************************************-->
 *
 * @fn int CTIgetWarnMessageLineLength( )
 *
 *   @brief  yields useful line length for warning messages
 *
 *   @return line length
 *
 ******************************************************************************/

int CTIgetWarnMessageLineLength()
{
    return message_line_length - strlen(warn_message_header);
}

/**
 * @return The number of warnings currently produced.
 */
int CTIgetWarnings()
{
    return warnings;
}


/** <!--********************************************************************-->
 *
 * @fn void CTIabortOutOfMemory( unsigned int request)
 *
 *   @brief   produces a specific "out of memory" error message
 *            without file name and line number and terminates the
 *            compilation process.
 *
 *            This very special function is needed because the normal
 *            procedure of formatting a message may require further
 *            allocation of memory, which in this very case generates
 *            a vicious circle of error messages instead of terminating
 *            compilation properly.
 *
 *   @param request size of requested memory
 *
 ******************************************************************************/

void CTIabortOutOfMemory(unsigned int request)
{
    fprintf(stderr,
        "\n"
        "%sOut of memory:\n"
        "%s %u bytes requested\n",
        abort_message_header,
        abort_message_header, request);

    errors++;

    AbortCompilation();
}

static void HandleCtiCall(enum cti_type type)
{
    if (type == CTI_ERROR) {
        errors++;
    } else if (type == CTI_WARN) {
        warnings++;
    }
}

static char *GetMessageHeader(enum cti_type type)
{
    switch (type) {
    case CTI_WARN:
        return warn_message_header;
    case CTI_ERROR:
        return error_message_header;
    case CTI_NOTE:
        return note_message_header;
    case CTI_STATE:
        return state_message_header;
    default:
        return "";
    }
}

/**
 * Prints a message to stderr with header according to the type.
 * @param type the type of message, see enum cti_type
 * @param newline Whether to append a newline to the output or not.
 * @param format printf style format string
 * @param ... variable args.
 */
void CTI(enum cti_type type, bool newline, const char *format, ...)
{
    char *message_header = GetMessageHeader(type);
    HandleCtiCall(type);
    va_list arg_p;

    va_start(arg_p, format);

    PrintMessage(message_header, format, arg_p, newline, NULL);

    va_end(arg_p);
}

/**
 * See @CTI
 * @param obj an object of type struct ctinfo with extra information.
 */
void CTIobj(enum cti_type type, bool newline, struct ctinfo obj, const char *format, ...)
{
    char *message_header = GetMessageHeader(type);
    HandleCtiCall(type);
    va_list arg_p;

    va_start(arg_p, format);

    PrintMessage(message_header, format, arg_p, newline, &obj);

    va_end(arg_p);
}
