#include "printer.h"
#include <stdio.h>

#include "lib/array.h"

static int indent_level = 0;

void print_indent()
{
    for (int i = 0; i < indent_level; i++) {
        printf("    ");
    }
}

void print_pass_reference(const void *it)
{
    Pass *p = (Pass*)it;
    print_indent();
    printf("<pass_reference id=%s>\n", p->id);
}

void print_action_type(enum ActionType t)
{
    switch(t) {
    case ACTION_PASS:
        printf("pass");
        break;
    case ACTION_PHASE:
        printf("phase");
        break;
    case ACTION_TRAVERSAL:
        printf("traversal");
        break;
    case ACTION_REFERENCE:
        printf("referece");
        break;
    default:
        printf("unkown");
    }
}

void print_action(const void *it)
{
    Action *action = (Action*)it;
    print_indent();
    printf("<action id=%s type=", action->id);
    print_action_type(action->type);
    printf("/>\n");
}

void print_phases(const Phase *p)
{
    printf("<phase id=%s>\n", p->id);
    indent_level++;
    array_map(p->actions, print_action);
    indent_level--;
    printf("</phase>\n");
}

void pretty_print(Config *ir)
{
    array_map(ir->phases, (void (*)(const void*))print_phases);
}