#pragma once

#include <stdbool.h>
#include <stddef.h>

// TODO: maybe make the struct 'private' and wrap into access modifiers.
struct command_line {
    char *input_file;
    int verbose;
    bool profiling;
    bool break_inspect_points;
    bool consistcheck;
    bool serialise;
    bool gen_user_files;
    bool show_ast;
    bool debug;
    char *backend;
    char *gen_dir;
    size_t ag_scheduler_max_iter;
};

extern struct command_line global_command_line;

void CLprocessArgs(int argc, char *argv[]);
