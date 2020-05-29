#pragma once

#include <stdbool.h>

// TODO: maybe make the struct 'private' and wrap into access modifiers.
typedef struct CommandOptions {
    int verbose;
    bool profiling;
    bool break_inspect_points;
    bool consistcheck;
    bool serialise;
    bool gen_user_files;
    char *backend;
    char *header_dir;
    char *source_dir;
    char *dot_dir;
    char *doc_dir;
} CommandOptions;

extern CommandOptions global_command_options;

void process_commandline_args(int argc, char *argv[]);
void init_global_options();
void usage(char *program);
void version(void);