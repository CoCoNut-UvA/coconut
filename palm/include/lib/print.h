#pragma once

#include <stdio.h>
#include <unistd.h>

#include "lib/array.h"
#include "lib/imap.h"

void print_char_n_times(char, size_t n);

/**
 * CONTRACT:
 *   string != NULL
 */
void print_string_n_times(char *string, size_t n);

// We define the type for YYLTYPE here,
// to avoid a circular dependency between this header and the parser
typedef struct ParserLocation {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} ParserLocation;

enum PrintType { PT_error, PT_warning, PT_note };

void print_user_error(char *header, const char *format, ...);

void print_init_compilation_messages(char *header, char *filename,
                                     array *lines, imap_t *parser_locations);

void print_error(void *loc_obj, char *format, ...);
void print_error_at(int lineno, int column, char *line, char *format, ...);
void print_error_range(int lineno, int column_start, int column_end,
                       char *line, char *format, ...);

void print_error_no_loc(char *format, ...);
void print_warning(void *loc_obj, char *format, ...);
void print_warning_at(int lineno, int column, char *line, char *format, ...);
void print_warning_range(int lineno, int column_start, int column_end,
                         char *line, char *format, ...);

void print_warning_no_loc(char *format, ...);
void print_note(void *loc_obj, char *format, ...);
void print_note_at(int lineno, int column, char *line, char *format, ...);
void print_note_range(int lineno, int column_start, int column_end, char *line,
                      char *format, ...);
void print_note_no_loc(char *format, ...);

void _print_internal_error(const char *file, const char *func, int line,
                           const char *format, ...);

#define print_internal_error(...)                                             \
    _print_internal_error(__FILE__, __func__, __LINE__, __VA_ARGS__)
