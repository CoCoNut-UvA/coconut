/**
 * This module contains the functions used internally by the phase driver during compilation.
 * This creates a bridge between the API functions for errors and cycles and command options.
 * It is internal because the user should never include this file explicitly.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <malloc.h>
#include <time.h>
#include "core/error.h"
#include "core/internal_phase_functions.h"
#include "core/action_handling.h"
#include "lib/array.h"
#include "lib/memory.h"
#include "lib/str.h"
#include "lib/print.h"
#include "lib/color.h"
#include "generated/trav-ast.h"
#include "generated/enum.h"
#include "generated/action_handlers.h"


static phase_driver_t phase_driver;

void _push_chk_frame(char *key, enum ccn_chk_types type) {
    ccn_chk_frame_t *frame = smap_retrieve(phase_driver.consistency_map, key);
    if (frame == NULL) {
        frame = mem_alloc(sizeof(ccn_chk_frame_t));
        frame->type = type;
        frame->ref_counter = 1;
        smap_insert(phase_driver.consistency_map, key, frame);
    } else {
        if (frame->type == type) {
            frame->ref_counter++;
        } else {
            assert(false);
        }
    }
}

void _pop_chk_frame(char *key) {
    ccn_chk_frame_t *frame = smap_retrieve(phase_driver.consistency_map, key);
    if (frame == NULL) {
        return;
    } else {
        if (frame->ref_counter > 0)
            frame->ref_counter--;

        if (frame->ref_counter == 0) {
            smap_remove_element(phase_driver.consistency_map, key);
            mem_free(frame);
        }
    }
}

void _exit_on_action_error(const char *action) {
    if (phase_driver.action_error) {
        print_user_error("CCN Error", "Reached action error.\n| Action: %s\n| Path: %s\n", action,  _ccn_get_path());
        exit(1);
    }
}

void _exit_on_phase_error() {
    if (_top_frame()->phase_error) {
        print_user_error("CCN Error", "Reached phase error .\n| Path: %s\n", _ccn_get_path());
        exit(1);
    }
}


void _reset_cycle() {
    phase_frame_t *frame = array_last(phase_driver.phase_stack);
    assert(frame != NULL);
    frame->cycle_notified = false;
    frame->cycles++;
}

bool _is_cycle_notified() {
    phase_frame_t *frame = array_last(phase_driver.phase_stack);
    assert(frame != NULL);
    return frame->cycle_notified;
}

phase_driver_t *_get_phase_driver() {
    return &phase_driver;
}

phase_frame_t *_top_frame() {
    return array_last(phase_driver.phase_stack);
}

void _push_frame(char *id, NodeType root_type) {
    phase_frame_t *frame = mem_alloc(sizeof(phase_frame_t));
    frame->phase_id = id;
    frame->cycle_notified = false;
    frame->curr_mark = NULL;
    frame->marks = NULL;
    frame->cycles = 0;
    frame->phase_error = false;
    frame->curr_root = root_type;
    array_append(phase_driver.phase_stack, frame);
}

void _pop_frame() {
    phase_frame_t *frame = array_pop(phase_driver.phase_stack);
    mem_free(frame);
}

void _initialize_phase_driver() {
    if (phase_driver.initialized)
        return;
    phase_driver.initialized = true;
    phase_driver.action_error = false;
    phase_driver.non_fatal_error = false;
    phase_driver.phase_stack = create_array();
    phase_driver.passes_time_frames = array_init(20);
    phase_driver.cycles_time_frames = array_init(20);
    phase_driver.action_mem_frames = array_init(20);
    phase_driver.consistency_map = smap_init(20);
    phase_driver.curr_sub_root = NULL;
    phase_driver.level = 0;
    phase_driver.action_id = 1; // We start at 1, as 0 states that its not set.
#ifdef CCN_ENABLE_POINTS
    phase_driver.id_to_enum_map = create_enum_mapping();
    phase_driver.breakpoint = NULL;
#endif
    phase_driver.ast = NULL;
    phase_driver.current_action = NULL;
    phase_driver.total_time = 0;
    phase_driver.total_allocated = 0;
    phase_driver.total_freed = 0;
    phase_driver.print_n = 0;

    ccn_init_action_array();
}

cycle_mark_t *_ccn_new_mark(void *item) {
    cycle_mark_t *mark = mem_alloc(sizeof(cycle_mark_t));
    mark->notified = true;
    mark->node = item;
    return mark;
}

void _ccn_set_curr_mark(cycle_mark_t *mark) {
    assert(mark != NULL);
    phase_frame_t *frame = _top_frame();
    frame->curr_mark = mark;
}

bool _ccn_mark_apply(void *item) {
    phase_frame_t *top = _top_frame();
    for (int i = 0; i < array_size(top->marks); ++i) {
        cycle_mark_t *mark = array_get(top->marks, i);
        if (mark->node == item) {
            mark->notified = false;
            return true;
        }
    }
    return false;
}

// TODO allow user to use this function instead of the safer ones.
bool _ccn_mark_remove(void *item) {
    phase_frame_t *top = _top_frame();
    for (int i = 0; i < array_size(top->marks); ++i) {
        cycle_mark_t *mark = array_get(top->marks, i);
        if (mark->node == item) {
            mark->notified = true;
            return true;
        }
    }
    return false;
}

static void print_phase_header(const char *message, const char *id) {
    PRINT_COLOR_STDOUT(GREEN);
    printf("[CCN] ");
    PRINT_COLOR_STDOUT(RESET_COLOR);
    print_char_n_times('*', phase_driver.level);
    printf("%s %s\n", message, id);
}

void _ccn_start_phase(char *id, NodeType root_type) {
    print_phase_header("start", id);
    phase_driver.level++;
    _push_frame(id, root_type);
}

void _ccn_end_phase(char *id) {
    _exit_on_phase_error();

    phase_driver.level--;
    print_phase_header("end", id);
    _pop_frame();
}

void _ccn_new_phase_time_frame(char *id, double time_sec) {
    time_frame_t *time_frame = mem_alloc(sizeof(time_frame_t));
    time_frame->id = id;
    time_frame->time_sec = time_sec;
    time_frame->path = _ccn_get_path();
    array_append(phase_driver.cycles_time_frames, time_frame);

}

void _ccn_new_pass_time_frame(char *id, double time_sec) {
    time_frame_t *time_frame = mem_alloc(sizeof(time_frame_t));
    time_frame->id = id;
    time_frame->time_sec = time_sec;
    time_frame->path = _ccn_get_path();
    array_append(phase_driver.passes_time_frames, time_frame);
}

int compare_time_frame_inverse(const void *a, const void *b) {
    const time_frame_t *pass_a = *((time_frame_t**)a);
    const time_frame_t *pass_b = *((time_frame_t**)b);

    if (pass_a->time_sec > pass_b->time_sec) {
        return -1;
    } else if (pass_a->time_sec < pass_b->time_sec) {
        return 1;
    }

    return 0;
}

void _ccn_print_time_frame(time_frame_t *time_frame) {
    printf("Name: %s\n", time_frame->id);
    printf("Path: %s\n", time_frame->path);
    printf("Time: %f\n", time_frame->time_sec);
    if (time_frame->time_sec == 0.0) {
        printf("\n");
        return;
    }
    printf("%% Time: %f\n\n" , (time_frame->time_sec / phase_driver.total_time) * 100);
}

void _ccn_print_top_n_time(int n) {
    int local_n = n;

    array *passes_times = phase_driver.passes_time_frames;
    array *cycles_times = phase_driver.cycles_time_frames;
    array_sort(passes_times, compare_time_frame_inverse);
    array_sort(cycles_times, compare_time_frame_inverse);

    if (n > array_size(passes_times) || n < 0)
        local_n = array_size(passes_times);

    putchar('\n');
    print_char_n_times('-', 80);
    putchar('\n');

    printf("Time statistic:\n\n");

    printf("Passes & Traversals:\n");
    for (int i = 0; i < local_n; ++i) {
        time_frame_t *time_frame = array_get(passes_times, i);
        _ccn_print_time_frame(time_frame);
    }

    putchar('\n');
    print_char_n_times('-', 80);
    putchar('\n');

    if (n > array_size(cycles_times) || n < 0)
        local_n = array_size(cycles_times);

    // TODO: Special style for cycles?
    // Maybe for cycles, add an iteration time or so?
    printf("Phases:\n");
    for (int i = 0; i < local_n; ++i) {
        time_frame_t *time_frame = array_get(cycles_times, i);
        _ccn_print_time_frame(time_frame);
    }

    print_char_n_times('-', 80);
    putchar('\n');
    printf("Total time: %f seconds\n", phase_driver.total_time);
    print_char_n_times('-', 80);
    putchar('\n');
}

#ifdef CCN_ENABLE_POINTS
point_frame_t *_ccn_create_point_frame_from_string(char *target) {
    if (!phase_driver.initialized)
        return NULL;

    char *action = NULL;
    long int cycle_count = 1;
    array *actions = ccn_str_split(target, '=');
    if (array_size(actions) > 1) {
        action = array_get(actions, 1);
        printf("ACTION: %s\n", action);
    }

    array *cycle_counts = ccn_str_split(array_get(actions, 0), ':');
    if (array_size(cycle_counts) > 1) {
        char *ptr = NULL;
        // TODO: check errno of strtol?
        cycle_count = strtol(array_get(cycle_counts, 1), &ptr, 10);
        printf("Cycle count: %li\n", cycle_count);
    }

    array *vals = ccn_str_split(array_get(cycle_counts, 0), '.');
    point_frame_t *frame = mem_alloc(sizeof(point_frame_t));
    char *curr_target = array_get(vals, 0);
    frame->current_id = *((enum ACTION_IDS*)smap_retrieve(phase_driver.id_to_enum_map, curr_target));

    frame->cycle_counter = cycle_count;
    frame->action = action;
    frame->ids = vals;
    frame->index = 0;

    array_cleanup(actions, mem_free);
    array_cleanup(cycle_counts, mem_free);

    return frame;
}
#endif

#ifdef CCN_ENABLE_POINTS
bool _ccn_is_final_point(enum ACTION_IDS id, phase_frame_t *frame, point_frame_t *curr_point) {
    if (curr_point->current_id == id && curr_point->cycle_counter == frame->cycles) {
        if (curr_point->index == array_size(curr_point->ids) - 1) {
            return true;
        } else {
            curr_point->index++;
            char *target = array_get(curr_point->ids, curr_point->index);
            curr_point->current_id = *((enum ACTION_IDS*)smap_retrieve(phase_driver.id_to_enum_map, target));
        }
    }

    return false;
}

// TODO: implement function lookup.
void _ccn_check_inspect_point(enum ACTION_IDS id, char *curr_action) {
    if (phase_driver.inspection_points == NULL)
        return;

    phase_frame_t *frame = _top_frame();
    for (int i = 0; i < array_size(phase_driver.inspection_points); ++i) {
        point_frame_t *point = array_get(phase_driver.inspection_points, i);
        if (_ccn_is_final_point(id, frame, point)) {
            char *tmp_filename = ccn_str_cat("inspect_", curr_action);
            char *filename = ccn_str_cat(tmp_filename, ".txt");
            FILE *f = fopen(filename, "w");
            int stdout = dup(STDOUT_FILENO);
            int new_stdout = dup2(fileno(f), STDOUT_FILENO);
            printf("Inspection point reached:\n");
            trav_start_Root(phase_driver.ast, TRAV_Print);
            dup2(stdout, new_stdout);
            close(stdout);
            mem_free(tmp_filename);
            mem_free(filename);
        }
    }
}

void _ccn_check_breakpoint(enum ACTION_IDS id) {
    phase_driver_t *pd = _get_phase_driver();
    if (pd->breakpoint == NULL)
        return;

    if (_ccn_is_final_point(id, _top_frame(), pd->breakpoint)) {
        printf("Breakpoint reached:\n");
        trav_start_Root(phase_driver.ast, TRAV_Print);
        exit(0);
    }
}

void _ccn_check_points(enum ACTION_IDS id, char *current_action) {
    _ccn_check_inspect_point(id, current_action);
    _ccn_check_breakpoint(id);
}
#endif

// TODO: cache this in phase driver as an array.
char *_ccn_get_path() {
    array *path_elements = create_array();
    if (array_size(phase_driver.phase_stack) == 0) {
        return ccn_str_cpy("");
    }

    for (int i = 0; i < array_size(phase_driver.phase_stack) - 1; ++i) {
        phase_frame_t *frame = array_get(phase_driver.phase_stack, i);
        array_append(path_elements, frame->phase_id);
        array_append(path_elements, ".");
    }

    phase_frame_t *frame = array_get(phase_driver.phase_stack, array_size(phase_driver.phase_stack) - 1);
    array_append(path_elements, frame->phase_id);

    char *path = ccn_str_cat_array(path_elements);

    array_clear(path_elements);
    array_cleanup(path_elements, mem_free);

    return path;
}


void _print_path() {
    if (array_size(phase_driver.phase_stack) <= 0)
        return;
    printf("Path: ");
    for (int i = 0; i < array_size(phase_driver.phase_stack) - 1; ++i) {
        phase_frame_t *frame = array_get(phase_driver.phase_stack, i);
        printf("%s.", frame->phase_id);
    }

    phase_frame_t *frame = array_get(phase_driver.phase_stack, array_size(phase_driver.phase_stack) - 1);
    printf("%s\n", frame->phase_id);
}

void *ccn_malloc(size_t size) {
    phase_driver.total_allocated += size;
    return malloc(size);
}

void ccn_free(void* item) {
    phase_driver.total_freed += malloc_usable_size(item); // glib deps, so not crossplatform.
    free(item);
}

void set_allocators() {
    set_allocator(ccn_malloc);
    set_deallocator(ccn_free);
}

void reset_allocators() {
    set_allocator(malloc);
    set_deallocator(free);
}

#ifdef CCN_ENABLE_POINTS
void _ccn_destroy_points(void *item) {
    point_frame_t *point = (point_frame_t*)item;
    array_cleanup(point->ids, mem_free);
    mem_free(point->action);
    mem_free(point);
}
#endif

void _ccn_destroy_time_frame(void *item) {
    time_frame_t *frame = (time_frame_t*)item;
    mem_free(frame->path);
    mem_free(frame);
}

// TODO: make sure statistics and stuff are printed.
void phase_driver_destroy() {
    array_cleanup(phase_driver.passes_time_frames, _ccn_destroy_time_frame);
    array_cleanup(phase_driver.cycles_time_frames, _ccn_destroy_time_frame);
    array_cleanup(phase_driver.phase_stack, NULL);
    array_cleanup(phase_driver.action_mem_frames, NULL);
    smap_free(phase_driver.consistency_map);
#ifdef CCN_ENABLE_POINTS
    array_cleanup(phase_driver.inspection_points, _ccn_destroy_points);
#endif
    ccn_destroy_action_array();
    reset_allocators();

}

void _ccn_destroy_sub_root() {
    if (phase_driver.curr_sub_root != NULL) {
        mem_free(phase_driver.curr_sub_root);
        phase_driver.curr_sub_root = NULL;
    }
}

void ccn_set_print_n(size_t n) {
    phase_driver.print_n = n;
}

void _ccn_phase_driver_start() {
    ccn_action_t *action = get_ccn_action_from_id(CCN_ROOT_ACTION);
    double start = clock();
    ccn_dispatch_action(action, CCN_ROOT_TYPE, NULL, true);
    double end = clock();
    phase_driver.total_time = (end - start) / CLOCKS_PER_SEC;
}
