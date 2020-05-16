#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/memory.h"
#include "lib/smap.h"
#include <stdio.h>

static void generate_nodeset(Nodeset *nodeset, FILE *fp, bool header) {

    out("void " FREE_TREE_FORMAT "(struct %s *nodeset)", nodeset->id,
        nodeset->id);

    if (header) {
        out(";");
    } else {
        int indent = 1;
        out(" {\n");
        out_begin_if("nodeset == NULL");
        out_statement("return");
        out_end_if();

        out("    switch(nodeset->type) {\n");
        for (int i = 0; i < array_size(nodeset->nodes); ++i) {
            Node *node = (Node *)array_get(nodeset->nodes, i);
            out("    case " NS_FORMAT ":\n", nodeset->id, node->id);
            out("        " FREE_TREE_FORMAT "(nodeset->value.val_%s);\n",
                node->id, node->id);
            out("        break;\n");
        }
        out("    }\n");
        out("    mem_free(nodeset);\n");
        out("}\n");
    }

    out("void " FREE_NODE_FORMAT "(struct %s* nodeset)", nodeset->id,
        nodeset->id);
    if (header) {
        out(";");
    } else {
        int indent = 1;
        out(" {\n");
        out_begin_if("nodeset == NULL");
        out_statement("return");
        out_end_if();
        out("    switch(nodeset->type) {\n");
        for (int i = 0; i < array_size(nodeset->nodes); ++i) {
            Node *node = (Node *)array_get(nodeset->nodes, i);
            out("    case " NS_FORMAT ":\n", nodeset->id, node->id);
            out("        " FREE_NODE_FORMAT "(nodeset->value.val_%s);\n",
                node->id, node->id);
            out("        break;\n");
        }
        out("    }\n");
        out("    mem_free(nodeset);\n");
        out("}\n");
    }
}

static void generate_node(Node *node, FILE *fp, bool header) {
    out("void " FREE_TREE_FORMAT "(struct %s* node)", node->id, node->id);
    int indent = 1;
    if (header) {
        out(";");
    } else {
        out(" {\n");
        out_begin_if("node == NULL");
        out_statement("return");
        out_end_if();

        for (int i = 0; i < array_size(node->children); ++i) {
            Child *child = (Child *)array_get(node->children, i);
            out("    " FREE_TREE_FORMAT "(node->%s);\n", child->type,
                child->id);
        }

        // Only need to free strings, as all other attributes are literals or
        // pointers to node's which are not owned by this node.
        for (int i = 0; i < array_size(node->attrs); ++i) {
            Attr *attr = (Attr *)array_get(node->attrs, i);
            if (attr->type == AT_string) {
                out("    mem_free(node->%s);\n", attr->id);
            }
        }

        /* out("    "); */
        out("    mem_free(node);\n");
        out("}\n");
    }

    out("void " FREE_NODE_FORMAT "(struct %s* node)", node->id, node->id);
    if (header) {
        out(";");
    } else {
        out(" {\n");
        out(" // skip children.\n");

        // Only need to free strings, as all other attributes are literals or
        // pointers to node's which are not owned by this node.
        for (int i = 0; i < array_size(node->attrs); ++i) {
            Attr *attr = (Attr *)array_get(node->attrs, i);
            if (attr->type == AT_string) {
                out("    mem_free(node->%s);\n", attr->id);
            }
        }
        out("    mem_free(node);\n");
        out("}\n");
    }
}

void generate_free_node_header(Config *c, FILE *fp, Node *n) {
    out("#pragma once\n");
    out("#include <stdbool.h>\n");
    out("#include <string.h>\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"generated/ast.h\"\n");
    generate_node(n, fp, true);
}

void generate_free_node_definitions(Config *c, FILE *fp, Node *n) {
    out("#include \"generated/free-%s.h\"\n", n->id);
    out("\n");

    smap_t *map = smap_init(32);

    for (int i = 0; i < array_size(n->children); ++i) {
        Child *child = (Child *)array_get(n->children, i);
        if (smap_retrieve(map, child->type) == NULL) {
            out("#include \"generated/free-%s.h\"\n", child->type);
            smap_insert(map, child->type, child);
        }
    }

    smap_free(map);

    generate_node(n, fp, false);
}

void generate_free_nodeset_header(Config *c, FILE *fp, Nodeset *n) {
    out("#pragma once\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"generated/ast.h\"\n\n");
    generate_nodeset(n, fp, true);
}

void generate_free_nodeset_definitions(Config *c, FILE *fp, Nodeset *n) {
    out("#include \"generated/free-%s.h\"\n", n->id);
    out("\n");

    smap_t *map = smap_init(32);

    for (int i = 0; i < array_size(n->nodes); ++i) {
        Node *node = (Node *)array_get(n->nodes, i);
        if (smap_retrieve(map, node->id) == NULL) {
            out("#include \"generated/free-%s.h\"\n", node->id);
            smap_insert(map, node->id, node);
        }
    }

    smap_free(map);

    generate_nodeset(n, fp, false);
}

void generate_free_header(Config *config, FILE *fp) {
    out("#pragma once\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        out("#include \"generated/free-%s.h\"\n", node->id);
    }
    for (int i = 0; i < array_size(config->nodesets); ++i) {
        Nodeset *nodeset = array_get(config->nodesets, i);
        out("#include \"generated/free-%s.h\"\n", nodeset->id);
    }
}