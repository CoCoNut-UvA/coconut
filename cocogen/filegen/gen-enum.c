#include <stdio.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"

#include "filegen/gen-enum.h"

void gen_enum(Config *config, FILE *fp, Enum *arg_enum) {
    out("// Enum %s\n", arg_enum->id);
    out("typedef enum {\n");
    for (int i = 0; i < array_size(arg_enum->values); i++) {
        char *id = (char *)array_get(arg_enum->values, i);
        out("    %s_%s,\n", arg_enum->prefix, id);
    }
    out("    %s_NULL\n", arg_enum->prefix);
    out("} %s;\n\n", arg_enum->id);
}

void gen_nodetype_enum(Config *config, FILE *fp) {
    out("// Enum NodeType\n");
    out("typedef enum {\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out("    N_%s,\n", node->id);
    }
    out("} NodeType;\n\n");
}

void gen_enum_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_ENUM_H_\n");
    out("#define _CCN_ENUM_H_\n\n");
    gen_nodetype_enum(config, fp);
    for (int i = 0; i < array_size(config->enums); ++i) {
        Enum *arg_enum = (Enum *)array_get(config->enums, i);
        gen_enum(config, fp, arg_enum);
    }
    out("#endif /* _CCN_ENUM_H_ */\n");
}

void gen_enum_src(Config *config, FILE *fp) {}