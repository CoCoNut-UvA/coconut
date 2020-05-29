#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include "ast/ast.h"
#include "filegen/util.h"

#include "lib/array.h"
#include "lib/errors.h"
#include "lib/memory.h"
#include "lib/print.h"
#include "lib/set_implementations.h"
#include "lib/smap.h"
#include "lib/str.h"

#define HASH_HEADER "// Hash: %s\n"

/* Struct for directory where files are being generated.
 * The struct stores all files generated this run in the
 * generated set. All files in the actual directory but not
 * generated or not in the blacklist will be deleted.
 */
typedef struct directory_tracking_data {
    ccn_set_t *blacklist;
    ccn_set_t *generated;
} directory_tracking_data_t;

static directory_tracking_data_t *create_tracking_data() {
    directory_tracking_data_t *data =
        mem_alloc(sizeof(directory_tracking_data_t));
    data->blacklist = ccn_set_string_create(10);
    data->generated = ccn_set_string_create(10);
    return data;
}

static char *current_directory = NULL;

static Config *ast_definition = NULL;
static char *output_directory = NULL;
static smap_t *generated_files = NULL;
static smap_t *directories_being_tracked = NULL;

static bool only_list_files = false;

/* Last step of generation is deleting all left over files in the tracked
 * directories. Because of flag change deleted nodes, we do not want to keep
 * these files around. So, we loop over all directories that are tracked and
 * delete the files not in the smap. Therefore users should not combine
 * generated data with their own files, because it might delete them.
 */
bool add_filename_to_tracked_specify_dir(const char *dir,
                                         const char *filename) {
    directory_tracking_data_t *data =
        smap_retrieve(directories_being_tracked, dir);
    if (data == NULL) {
        fprintf(stderr, "Could not find dir: %s\n", dir);
        return false;
    }

    ccn_set_insert(data->generated, ccn_str_cpy(filename));
    return true;
}

/* Add a filename to the tracked set in the current directory.
   Returns false if the current directory is not being tracked.
   Returns true on succes.
 */
bool add_filename_to_tracked(const char *filename) {
    assert(current_directory != NULL);

    return add_filename_to_tracked_specify_dir(current_directory, filename);
}

bool add_filename_to_blacklist(const char *filename) {
    assert(current_directory != NULL);

    directory_tracking_data_t *data =
        smap_retrieve(directories_being_tracked, current_directory);
    if (data == NULL)
        return false;

    ccn_set_insert(data->blacklist, ccn_str_cpy(filename));
    return true;
}

bool add_filename_to_blacklist_specify_dir(const char *dir,
                                           const char *filename) {
    directory_tracking_data_t *data =
        smap_retrieve(directories_being_tracked, dir);
    if (data == NULL)
        return false;

    ccn_set_insert(data->blacklist, ccn_str_cpy(filename));
    return true;
}

bool clean_specific_tracked_directory(const char *dirpath) {
    assert(dirpath != NULL);
    DIR *dir;
    struct dirent *ent;
    directory_tracking_data_t *data =
        smap_retrieve(directories_being_tracked, dirpath);
    if (data == NULL)
        return false;

    if ((dir = opendir(dirpath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type != DT_REG)
                continue;

            char *name = ent->d_name;
            if (!ccn_set_contains(data->blacklist, name) &&
                !ccn_set_contains(data->generated, name)) {
                char *full_path = ccn_str_cat(dirpath, name);
                if (remove(full_path) != 0) {
                    perror("Could not remove file: ");
                    fprintf(stderr, " %s\n", full_path);
                    mem_free(full_path);
                    return false;
                } else {
                    printf("RM: %s\n", full_path);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory for file cleanup"); // TODO: Probably
                                                            // propogate error
                                                            // instead?
        fprintf(stderr, "Dirpath: %s\n", dirpath);
    }
    return true;
}

bool add_directory_to_tracked_dirs(const char *dir) {
    if (smap_retrieve(directories_being_tracked, dir) == NULL) {
        smap_insert(directories_being_tracked, ccn_str_cpy(dir),
                    create_tracking_data());
        return true;
    }

    return false;
}

bool clean_current_directory_to_be_tracked() {
    return clean_specific_tracked_directory(current_directory);
}

// TODO: Handle errors.
bool clean_all_tracked_directories() {
    array *dirs = smap_keys(directories_being_tracked);
    for (int i = 0; i < array_size(dirs); ++i) {
        char *dirname = array_get(dirs, i);
        clean_specific_tracked_directory(dirname);
    }
    return true;
}

void set_current_directory_to_be_tracked(const char *dirname) {
    if (current_directory != NULL)
        mem_free(current_directory);

    current_directory = ccn_str_cpy(dirname);

    if (smap_retrieve(directories_being_tracked, dirname) == NULL) {
        smap_insert(directories_being_tracked, ccn_str_cpy(dirname),
                    create_tracking_data());
    }
}

void init_tracking_data(size_t num_of_dirs) {
    directories_being_tracked = smap_init(num_of_dirs);
}

void cleanup_tracking_data() {
    smap_free_values(directories_being_tracked, mem_free);
    mem_free(current_directory);
}

FILE *get_fp(char *full_path, char *mode) {
    FILE *fp = fopen(full_path, mode);
    if (!fp) {
        perror("Opening file failed");
        exit(CANNOT_OPEN_FILE);
    }
    return fp;
}

bool is_only_list_files() { return only_list_files; }

Config *_get_ast_definition() { return ast_definition; }

char *format_with_formatter(char *formatter, char *value) {
    char *res = mem_alloc(strlen(formatter) + strlen(value) + 1);
    sprintf(res, formatter, value);
    return res;
}

char *get_full_path_with_dir(const char *dir, char *filename, char *formatter) {
    size_t dir_len = strlen(dir);
    char *full_path = mem_alloc(dir_len + strlen(filename) + 1);
    strcpy(full_path, dir);
    strcat(full_path, filename);

    if (formatter) {
        char *old_full_path = full_path;
        full_path =
            mem_alloc(dir_len + strlen(formatter) + strlen(filename) + 1);
        sprintf(full_path, old_full_path, formatter);
        mem_free(old_full_path);
    }

    return full_path;
}

static char *get_full_path(char *filename, char *formatter) {
    return get_full_path_with_dir(output_directory, filename, formatter);
}

void print_file_gen(char *full_path) {
    if (only_list_files)
        printf("%s\n", full_path);
    else
        printf(COLOR_GREEN " GEN       " COLOR_RESET "%s\n", full_path);
}

void add_filename_to_set(char *filename) {
    char *filename_dup = strdup(filename);
    char *fn = strrchr(filename_dup, '/');
    if (fn) {
        // Skip the slash
        fn = fn + 1;
    } else {
        fn = filename;
    }

    size_t length = strlen(fn);
    if ((fn[length - 2] == '.' &&
         (fn[length - 1] == 'c' || fn[length - 1] == 'h'))) {
        fn[length - 2] = '\0';
        smap_insert(generated_files, fn, fn);
        free(filename_dup);
    }
}

static bool hash_match(NodeCommonInfo *info, char *full_path) {
    print_file_gen(full_path);
    return false; // TODO: remove this

    char *current_hash = mem_alloc(43 * sizeof(char));
    bool rv = false;

    FILE *fp = fopen(full_path, "r");
    if (fp != NULL) {
        // TODO: More robustness of the detection of the hash, maybe use regex.
        // Hash: %32s\n -> read 3+4+2+32+1 = 42 characters.
        if (fgets(current_hash, 42, fp) != NULL) {
            if (strncmp(current_hash + 9, info->hash, 32) == 0) {
                rv = true;
                /* printf(COLOR_GREEN " SAME      " COLOR_RESET "%s\n", */
                /*        full_path); */
            }
        }
        fclose(fp);
    }

    if (!rv) {
        print_file_gen(full_path);
    }

    add_filename_to_set(full_path);
    mem_free(current_hash);
    return rv;
}

void filegen_init(Config *config, bool list_gen_files_flag) {
    ast_definition = config;
    only_list_files = list_gen_files_flag;
}

void filegen_cleanup(void) {
    if (output_directory != NULL) {
        mem_free(output_directory);
    }

    if (generated_files != NULL) {
        smap_free(generated_files);
    }
}

bool ensure_dir_exists(char *dirname, __mode_t mode) {
    if (mkdir(dirname, mode) == 0) {
        return true;
    } else {
        if (errno == EEXIST) {
            DIR *d = opendir(dirname);

            // out_dir exists but is a file
            if (!d) {
                perror("Creation failed:");
                exit(CANNOT_CREATE_DIR);
            } else {
                // Directory already exists
                closedir(d);
            }
            return true;
        } else {
            perror("Creation failed");
            exit(CANNOT_CREATE_DIR);
        }
    }
    return false;
}

void filegen_dir(char *out_dir) {
    filegen_cleanup();

    generated_files = smap_init(32);

    size_t out_dir_len = strlen(out_dir);

    if (out_dir[out_dir_len - 1] != '/') {
        output_directory = mem_alloc(out_dir_len + 2);
        sprintf(output_directory, "%s/", out_dir);
    } else {
        output_directory = strdup(out_dir);
    }

    ensure_dir_exists(output_directory, 0755);
}

void filegen_generate(char *filename, void (*func)(Config *, FILE *)) {

    char *full_path = get_full_path(filename, NULL);
    add_filename_to_tracked(filename);
    add_filename_to_set(full_path);

    print_file_gen(full_path);

    if (!only_list_files) {
        FILE *fp = get_fp(full_path, "w");
        func(ast_definition, fp);
        out("\n");
        fclose(fp);
    }

    mem_free(full_path);
}

void filegen_all_nodes(char *fileformatter,
                       void (*func)(Config *, FILE *, Node *)) {
    char *full_path;
    FILE *fp;

    for (int i = 0; i < array_size(ast_definition->nodes); ++i) {
        Node *node = array_get(ast_definition->nodes, i);
        char *filename = format_with_formatter(fileformatter, node->id->lwr);

        full_path = get_full_path_with_dir(current_directory, filename, NULL);
        add_filename_to_tracked(filename);

        if (hash_match(node->common_info, full_path)) {
            mem_free(full_path);
            continue;
        }

        if (!only_list_files) {
            fp = get_fp(full_path, "w");
            out(HASH_HEADER, node->common_info->hash);
            func(ast_definition, fp, node);
            out("\n");
            fclose(fp);
        }
        mem_free(filename);
        mem_free(full_path);
    }
}

void filegen_all_nodesets(char *fileformatter,
                          void (*func)(Config *, FILE *, Nodeset *)) {
    char *full_path;
    FILE *fp;

    for (int i = 0; i < array_size(ast_definition->nodesets); ++i) {
        Nodeset *nodeset = array_get(ast_definition->nodesets, i);
        char *filename = format_with_formatter(fileformatter, nodeset->id->lwr);
        // full_path = get_full_path(fileformatter, nodeset->id);
        full_path = get_full_path_with_dir(current_directory, filename, NULL);
        add_filename_to_tracked(filename);

        if (hash_match(nodeset->common_info, full_path)) {
            mem_free(full_path);
            continue;
        }

        if (!only_list_files) {
            fp = get_fp(full_path, "w");
            out(HASH_HEADER, nodeset->common_info->hash);
            func(ast_definition, fp, nodeset);
            out("\n");
            fclose(fp);
        }
        mem_free(filename);
        mem_free(full_path);
    }
}

void filegen_all_traversals(char *fileformatter,
                            void (*func)(Config *, FILE *, Traversal *)) {
    char *full_path;
    FILE *fp;

    for (int i = 0; i < array_size(ast_definition->traversals); ++i) {
        Traversal *trav = array_get(ast_definition->traversals, i);
        char *filename = format_with_formatter(fileformatter, trav->id->lwr);
        // full_path = get_full_path(fileformatter, trav->id);
        full_path = get_full_path_with_dir(current_directory, filename, NULL);
        add_filename_to_tracked(filename);

        if (hash_match(trav->common_info, full_path)) {
            mem_free(full_path);
            continue;
        }

        if (!only_list_files) {
            fp = get_fp(full_path, "w");
            out(HASH_HEADER, trav->common_info->hash);
            func(ast_definition, fp, trav);
            out("\n");
            fclose(fp);
        }
        mem_free(filename);
        mem_free(full_path);
    }
}

void filegen_all_passes(char *fileformatter,
                        void (*func)(Config *, FILE *, Pass *)) {
    char *full_path;
    FILE *fp;

    for (int i = 0; i < array_size(ast_definition->passes); ++i) {
        Pass *pass = array_get(ast_definition->passes, i);
        char *filename =
            format_with_formatter(fileformatter, pass_func_or_id(pass));
        // full_path = get_full_path(fileformatter, pass->id);
        full_path = get_full_path_with_dir(current_directory, filename, NULL);
        add_filename_to_tracked(filename);

        if (hash_match(pass->common_info, full_path)) {
            mem_free(full_path);
            continue;
        }

        if (!only_list_files) {
            fp = get_fp(full_path, "w");
            out(HASH_HEADER, pass->common_info->hash);
            func(ast_definition, fp, pass);
            out("\n");
            fclose(fp);
        }
        mem_free(filename);
        mem_free(full_path);
    }
}

void filegen_all_phases(char *fileformatter,
                        void (*func)(Config *, FILE *, Phase *)) {

    char *full_path;
    FILE *fp;
    for (int i = 0; i < array_size(ast_definition->phases); ++i) {
        Phase *phase = array_get(ast_definition->phases, i);
        char *filename = format_with_formatter(fileformatter, phase->id->lwr);
        // full_path = get_full_path(fileformatter, phase->id);
        full_path = get_full_path_with_dir(current_directory, filename, NULL);
        add_filename_to_tracked(filename);

        if (hash_match(phase->common_info, full_path)) {
            phase->common_info->hash_matches = true;
            mem_free(full_path);
            continue;
        }

        if (!only_list_files) {
            fp = get_fp(full_path, "w");
            out(HASH_HEADER, phase->common_info->hash);
            func(ast_definition, fp, phase);
            out("\n");
            fclose(fp);
        }
        mem_free(filename);
        mem_free(full_path);
    }
}

void filegen_phase_subtree(Config *config, void (*func)(Config *, array *)) {
    func(config, ast_definition->phases);
}