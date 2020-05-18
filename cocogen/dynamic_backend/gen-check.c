#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_check_header(Config *config, FILE *fp) {}

void gen_nodeset_func(Config *config, FILE *fp, Nodeset *nodeset) {
    char *nodesetlwr = strlwr(nodeset->id);
    out_start_func("bool type_is_%s(Node *arg_node)", nodesetlwr);
    for (int i = 0; i < array_size(nodeset->nodes); i++) {
        Node *node = array_get(nodeset->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_begin_if("NODE_TYPE(arg_node) == NT_%s", nodelwr);
        out_field("return true");
        out_end_if();
        mem_free(nodelwr);
    }
    out_field("return false");
    out_end_func();
    mem_free(nodesetlwr);
}

void gen_check_func(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_comment("%s", node->id);
    out_start_func("Node *check_%s(Node *arg_node)", nodelwr);
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        char *childtypelwr = strlwr(child->type);
        if (child->node) {
            out_begin_if(
                "%s_%s(arg_node) && (NODE_TYPE(%s_%s(arg_node)) != NT_%s)",
                nodeupr, childupr, nodeupr, childupr, childtypelwr);
        } else if (child->nodeset) {
            out_begin_if("%s_%s(arg_node) && !type_is_%s(%s_%s(arg_node))",
                         nodeupr, childupr, childtypelwr, nodeupr, childupr);
        }
        out_end_if();
        mem_free(childupr);
        mem_free(childtypelwr);
    }
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
    }
    if (node->children) {
        out_field("arg_node = trav_children(arg_node)");
    }
    out_field("return arg_node");
    out_end_func();
    mem_free(nodelwr);
    mem_free(nodeupr);
}

void gen_check_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"include/core/actions_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
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