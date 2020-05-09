#pragma once
#include "ast/ast.h"
#include <stdio.h>

// TODO: some of these functions are generic and should be moved to palm
// like get_fp and get_full_path and so on.
// This is mostly fr tracking generated files.

bool add_filename_to_tracked(const char *filename);
bool add_filename_to_tracked_specify_dir(const char *dir, const char *filename);
bool add_filename_to_blacklist(const char *filename);
bool add_filename_to_blacklist_specify_dir(const char *dir, const char *filename);
bool clean_specific_tracked_directory(const char *dirpath);
bool clean_current_tracked_directory();
bool clean_all_tracked_directories();
void set_current_directory_to_be_tracked(const char *dirname);
void init_tracking_data(size_t num_of_dirs);
void cleanup_tracking_data();
bool add_directory_to_tracked_dirs(const char *dir);

void filegen_init(Config *config, bool list_gen_files_flag);
void filegen_dir(char *out_dir);
FILE *get_fp(char *full_path, char *mode);
char *get_full_path_with_dir(const char *dir, char *filename, char *formatter);
bool is_only_list_files();
Config *_get_ast_definition(); // TODO(Damian): This should be moved to a better location.
void filegen_generate(char *filename, void (*func)(Config *, FILE *));
void filegen_all_nodes(char *fileformatter,
                       void (*func)(Config *, FILE *, Node *));
void filegen_all_nodesets(char *fileformatter,
                          void (*func)(Config *, FILE *, Nodeset *));

void filegen_all_traversals(char *fileformatter,
                            void (*func)(Config *, FILE *, Traversal *));
void filegen_all_passes(char *fileformatter,
                        void (*func)(Config *, FILE *, Pass *));

void filegen_all_phases(char *fileFormatter,
                        void (*func)(Config *, FILE *, Phase *));

void filegen_phase_subtree(Config *, void (*func)(Config *, array *));

void filegen_delete_non_generated_filed(ccn_set_t *generated_files, array *directories, ccn_set_t *blacklist);
void filegen_cleanup_old_files(void);
void filegen_cleanup(void);
void print_file_gen(char *full_path);
void add_filename_to_set(char *filename);
bool ensure_dir_exists(char *dirname, int mode);