#include "ast/ast.h"
#include "format/out_macros.h"
#include "filehandling/gen-driver.h"

void GenTravDataStruct(struct ast *ast, FILE *fp, struct traversal *trav) {
    if (!trav->data) {
        return;
    }
    int indent = 0;
    OUT_STRUCT("TRAV_DATA_%s", trav->name->upr);
    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, trav->data, next) {
        char *type_str  = ASTattributeTypeToString(attr);
        if (attr->type == AT_link) {
            OUT_FIELD("%s *%s", type_str, attr->name->orig);
        } else {
            OUT_FIELD("%s %s", type_str, attr->name->orig);
        }
    }
    OUT_STRUCT_END();
}

void GenTravDataUnion(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_UNION("TRAV_DATA");
    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (!trav->data) {
            continue;
        }
        OUT_FIELD("struct TRAV_DATA_%s *TD_%s", trav->name->upr, trav->name->lwr);
    }
    OUT_STRUCT_END();
}

void GenTravDataMacros(struct ast *ast, FILE *fp, struct traversal *trav) {
    if (!trav->data) {
        return;
    }
    int indent = 0;
    struct attribute *attr = NULL;
    SLIST_FOREACH(attr, trav->data, next) {
        OUT("#define %s_%s (trav->current()->trav_data.TD_%s->%s)\n",
            trav->name->upr, trav->name->upr, trav->name->lwr, trav->name->orig);
    }
}

static
void GenTravDataHeader(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("#ifndef _CCN_TRAV_H_\n");
    OUT("#define _CCN_TRAV_H_\n\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("\n");

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (!trav->data) {
            continue;
        }
        struct attribute *attr = NULL;
        SLIST_FOREACH(attr, trav->data, next) {
            if (attr->type == AT_link_or_enum) {
                OUT("#include \"%s\"\n", attr->include_file);
            }
        }
    }
    OUT("\n");

    trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        GenTravDataStruct(ast, fp, trav);
        GenTravDataMacros(ast, fp, trav);
        if (trav->data) {
            OUT_FIELD("struct ccn_trav *TRAVnew_%s()", trav->name->lwr);
            OUT_FIELD("void TRAVfree_%s(struct ccn_trav *trav)", trav->name->lwr);
        }
    }

    GenTravDataUnion(ast, fp);

    OUT("\n");
    OUT("#endif /* _CCN_TRAV_H_ */\n");
}

void GenTravDataConstructor(struct ast *ast, FILE *fp, struct traversal *trav) {
    int indent = 0;
    OUT_START_FUNC("struct ccn_trav *TRAVnew_%s(struct ccn_trav *trav)", trav->name->lwr);
    OUT_FIELD("trav->trav_data.TD_%s = MEMmalloc(sizeof(struct TRAV_DATA_%s))",
              trav->name->lwr, trav->name->upr);
    OUT_FIELD("trav = %sallocTravData(trav)", trav->prefix->upr);
    OUT_FIELD("return trav");
    OUT_END_FUNC();
}

void GenTravDataDestructor(struct ast *ast, FILE *fp, struct traversal *trav) {
    int indent = 0;
    OUT_START_FUNC("void TRAVfree_%s(struct ccn_trav *trav)", trav->name->lwr);
    OUT_FIELD("%sfreeTravData(trav)", trav->prefix->upr);
    OUT_FIELD("MEMfree(trav->trav_data.TD_%s)", trav->name->lwr);
    OUT_END_FUNC();
}

static
void GenTravDataSrc(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("#include <stdlib.h>\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    OUT("\n");

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (trav->data) {
            GenTravDataConstructor(ast, fp, trav);
            GenTravDataDestructor(ast, fp, trav);
        }
    }
}

struct ast *GTDdoGen(struct ast *ast)
{
    FILE *fp = FHgetIncludeFile("trav_data.h");
    GenTravDataHeader(ast, fp);
    fp = FHgetSourceFile("trav_data.c");
    GenTravDataSrc(ast, fp);
    return ast;
}