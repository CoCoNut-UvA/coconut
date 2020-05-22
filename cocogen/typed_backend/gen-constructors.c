#include <stdbool.h>
#include <stdio.h>

#include "ast/ast.h"
#include "ast/util.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/array.h"
#include "lib/memory.h"
#include "lib/smap.h"

static void generate_node(Node *node, FILE *fp, bool header) {
    out("struct %s *" CREATE_NODE_FORMAT "(", node->id, node->id);

    int arg_count = 0;
    bool first = true;

    for (int i = 0; i < array_size(node->children); i++) {
        Child *c = array_get(node->children, i);
        if (c->construct) {
            if (first) {
                first = false;
                out("\n");
            } else {
                out(",\n");
            }

            out("       struct %s *%s", c->type, c->id);
            arg_count++;
        }
    }

    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        if (attr->construct) {
            if (first) {
                first = false;
                out("\n");
            } else {
                out(",\n");
            }

            out("       %s %s", str_attr_type(attr), attr->id);
            arg_count++;
        }
    }

    if (arg_count == 0)
        out("void");

    if (header) {
        out(");\n\n");
    } else {
        out(") {\n");

        out("   struct %s *res = mem_alloc(sizeof(struct %s));\n", node->id,
            node->id);

        for (int i = 0; i < array_size(node->children); i++) {
            Child *c = array_get(node->children, i);
            if (c->construct) {
                out("   res->%s = %s;\n", c->id, c->id);
            } else {
                out("   res->%s = NULL;\n", c->id);
            }
        }

        for (int i = 0; i < array_size(node->attrs); i++) {
            Attr *attr = array_get(node->attrs, i);
            if (attr->construct) {
                out("   res->%s = %s;\n", attr->id, attr->id);
            } else {
                out("   res->%s = ", attr->id);
                if (attr->default_value) {
                    switch (attr->default_value->type) {
                    case AV_string:
                        out("\"%s\";\n",
                            attr->default_value->value.string_value);
                        break;
                    case AV_int:
                        out("%ld;\n", attr->default_value->value.int_value);
                        break;
                    case AV_uint:
                        out("%lu;\n", attr->default_value->value.uint_value);
                        break;
                    case AV_float:
                        out("%f;\n", attr->default_value->value.float_value);
                        break;
                    case AV_double:
                        out("%f;\n", attr->default_value->value.double_value);
                        break;
                    case AV_bool:
                        out("%s;\n", attr->default_value->value.bool_value
                                         ? "true"
                                         : "false");
                        break;
                    case AV_id:
                        out("NULL; // TODO: fix default value id\n");
                        break;
                    }
                } else {
                    switch (attr->type) {
                    case AT_int:
                    case AT_uint:
                    case AT_int8:
                    case AT_int16:
                    case AT_int32:
                    case AT_int64:
                    case AT_uint8:
                    case AT_uint16:
                    case AT_uint32:
                    case AT_uint64:
                    case AT_enum:
                        out("0;\n");
                        break;
                    case AT_float:
                    case AT_double:
                        out("0.0;\n");
                        break;
                    case AT_bool:
                        out("false;\n");
                        break;
                    case AT_string:
                    case AT_link:
                        out("NULL;\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        out("   return res;\n");

        out("}\n\n");
    }
}

static void generate_nodeset(Nodeset *nodeset, FILE *fp, bool header) {
    for (int i = 0; i < array_size(nodeset->nodes); i++) {
        Node *node = array_get(nodeset->nodes, i);

        out("struct %s *" CREATE_NODESET_FORMAT "(struct %s *_%s)", nodeset->id,
            nodeset->id, node->id, node->id, node->id);

        if (header) {
            out(";\n\n");
        } else {
            out(" {\n");
            out("   struct %s *res = mem_alloc(sizeof(struct %s));\n",
                nodeset->id, nodeset->id);

            out("   res->type = " NS_FORMAT ";\n", nodeset->id, node->id);
            out("   res->value.val_%s = _%s;\n", node->id, node->id);
            out("   return res;\n");
            out("}\n\n");
        }
    }
}

void generate_create_node_header(Config *config, FILE *fp, Node *node) {
    out("#pragma once\n");
    out("#include \"ast-%s.h\"\n", node->id);
    out("\n");

    generate_node(node, fp, true);
}

void generate_create_node_definitions(Config *c, FILE *fp, Node *n) {
    out("#include \"lib/memory.h\"\n");
    out("#include \"ast-%s.h\"\n", n->id);
    out("// ast-%s.h includes the neccesary attribute and children.\n", n->id);

    generate_node(n, fp, false);
}

void generate_create_nodeset_header(Config *c, FILE *fp, Nodeset *n) {
    out("#pragma once\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"ast.h\"\n\n");
    generate_nodeset(n, fp, true);
}

void generate_create_nodeset_definitions(Config *c, FILE *fp, Nodeset *n) {
    out("#include \"create-%s.h\"\n", n->id);
    out("\n");

    smap_t *map = smap_init(32);

    for (int i = 0; i < array_size(n->nodes); ++i) {
        Node *node = (Node *)array_get(n->nodes, i);
        if (smap_retrieve(map, node->id) == NULL) {
            out("#include \"create-%s.h\"\n", node->id);
            smap_insert(map, node->id, node);
        }
    }

    smap_free(map);

    generate_nodeset(n, fp, false);
}

void generate_create_header(Config *config, FILE *fp) {
    out("#pragma once\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        out("#include \"create-%s.h\"\n", node->id);
    }
    for (int i = 0; i < array_size(config->nodesets); ++i) {
        Nodeset *nodeset = array_get(config->nodesets, i);
        out("#include \"create-%s.h\"\n", nodeset->id);
    }
}