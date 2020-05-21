#pragma once

/**
 * This module contains all the functions that are needed for the phase driver.
 * These functions will be used in the real user compiler by the phase driver.
 * So not in the metacompiler.
 * The user however should never need these functions thats why they are
 * prefixed with an underscore.
 */

#include <stdbool.h>
#include <stdint.h>

#include "lib/array.h"
#include "lib/smap.h"
#include "generated/ccn_enables.h"
#include "generated/enum.h"
#include "generated/action_handlers.h"
#ifdef CCN_ENABLE_POINTS
#include "generated/breakpoint-finder.h"
#endif

typedef struct ccn_subroot ccn_subroot;

enum ccn_chk_types {
    CCN_CHK_DISALLOWED,
    CCN_CHK_MANDATORY,
};

typedef struct ccn_chk_frame {
    enum ccn_chk_types type;
    array *values;
    int ref_counter;
} ccn_chk_frame_t;

typedef struct mem_frame {
    char *id;
    char *path;
    size_t allocated;
    size_t freed;
} mem_frame_t;

typedef struct time_frame {
    char *id;
    char *path;
    double time_sec;
} time_frame_t;

typedef struct cycle_mark {
    bool notified;
    void *node;
} cycle_mark_t;


typedef struct phase_frame {
    char *phase_id;
    bool cycle_notified;
    bool phase_error;
    array *marks;
    NodeType curr_root;
    cycle_mark_t *curr_mark;
    size_t cycles;
} phase_frame_t;

#ifdef CCN_ENABLE_POINTS
typedef struct point_frame {
    array *ids;
    enum ACTION_IDS current_id;
    size_t index;
    char *action;
    size_t cycle_counter;
} point_frame_t;
#endif

typedef struct phase_driver {
    bool initialized;
    bool action_error;
    bool non_fatal_error;
    array *phase_stack;
    array *passes_time_frames;
    array *cycles_time_frames;
    array *action_mem_frames;
    ccn_subroot *curr_sub_root;
    smap_t *consistency_map;
    smap_t *id_to_enum_map;
    int level;
    uint32_t action_id;
    size_t print_n;
#ifdef CCN_ENABLE_POINTS
    point_frame_t *breakpoint;
#endif
    array *inspection_points;
    char *current_action;
    void *ast;
    double total_time;
    size_t total_allocated;
    size_t total_freed;
} phase_driver_t;


phase_frame_t *_top_frame();
cycle_mark_t *_ccn_new_mark(void *);
bool _ccn_mark_apply(void *);
bool _ccn_mark_remove(void *);
void _ccn_start_phase(char *id, NodeType root_type);
void _ccn_end_phase(char *id);
void _push_chk_frame(char *key, enum ccn_chk_types type);
void _pop_chk_frame(char *key);
void _exit_on_action_error();
void _exit_on_phase_error();
bool _is_cycle_notified();
void _reset_cycle();
void _initialize_phase_driver();
void _ccn_set_curr_mark(cycle_mark_t *mark);
void _ccn_new_pass_time_frame(char *id, double time);
void _ccn_new_phase_time_frame(char *id, double time);
void _ccn_print_top_n_time(int n);
void _print_path();
char *_ccn_get_path();
void set_allocators();
void set_deallocators();
void *ccn_malloc(size_t);
void ccn_free(void *);
void phase_driver_destroy();
void _ccn_destroy_sub_root();
void _ccn_phase_driver_start();

#ifdef CCN_ENABLE_POINTS
void _ccn_check_points(enum ACTION_IDS id, char *curr_action);
point_frame_t *_ccn_create_point_frame_from_string(char *target);
#endif

phase_driver_t *_get_phase_driver();
