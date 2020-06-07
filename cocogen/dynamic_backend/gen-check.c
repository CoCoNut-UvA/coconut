#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"
#include "lib/print.h"

static int indent = 0;

void gen_check_header(Config *config, FILE *fp) {}

void gen_nodeset_func(Config *config, FILE *fp, Nodeset *nodeset) {
    out_start_func("bool type_is_%s(Node *arg_node)", nodeset->id->lwr);
    out("    return (");
    for (int i = 0; i < array_size(nodeset->nodes); i++) {
        Node *node = array_get(nodeset->nodes, i);
        out("NODE_TYPE(arg_node) == NT_%s", node->id->lwr);
        if (i != array_size(nodeset->nodes) - 1) {
            out(" || ");
        }
    }
    out(");\n");
    out_end_func();
}

void gen_check_func(Config *config, FILE *fp, Node *node) {
    out_comment("%s", node->id->orig);
    out_start_func("Node *ccn_check_%s(Node *arg_node)", node->id->lwr);
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        out_begin_if("%s_%s(arg_node)", node->id->upr, child->id->upr);
        if (child->node) {
            out_begin_if("NODE_TYPE(%s_%s(arg_node)) != NT_%s", node->id->upr,
                         child->id->upr, child->type->lwr);
        } else if (child->nodeset) {
            out_begin_if("!type_is_%s(%s_%s(arg_node))", child->type->lwr,
                         node->id->upr, child->id->upr);
        }
        out_field(
            "print_user_error(\"consistency checking\", \"Child \\\"%s\\\" of "
            "node \\\"%s\\\" has nonallowed type.\")",
            child->id->orig, node->id->orig);
        out_end_if();
        out_end_if();
    }
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
    }
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    out_field("return arg_node");
    out_end_func();
}

void gen_check_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"ccn/actions_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"lib/print.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodesets); i++) {
        Nodeset *nodeset = (Nodeset *)array_get(config->nodesets, i);
        gen_nodeset_func(config, fp, nodeset);
    }
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_check_func(config, fp, node);
    }
}