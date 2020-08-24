
#include <sys/queue.h>

#include "ast/ast.h"
#include "ast/constructors.h"

#include "palm/dbug.h"
#include "palm/ctinfo.h"

static
bool CheckCollisison(struct ast *ast, char *name, struct symbol_table_entry **entry)
{
    bool found = false;
    *entry = HTlookup(ast->symbol_table, name, &found);
    return found;
}


void InsertEntry(struct ast *ast, enum symbol_table_entry_type type, void *data, char *key)
{
    struct symbol_table_entry *entry = ASTnewSymbolTableEntry(type, data);
    CTIstate("Inserting symbol table entry:%s\n", key);
    bool res = HTinsert(ast->symbol_table, key, entry);
    DBUG_ASSERT(res, "Insert should succeed, but failed.");
}

static
void PrintIdLocationNote(struct symbol_table_entry *entry)
{
    struct id *id = NULL;
    switch(entry->type) {
    case STE_ENUM:
        id = ((struct ccn_enum *)entry->data)->name;
        break;
    case STE_PASS:
        id = ((struct pass*)entry->data)->name;
        break;
    case STE_TRAVERSAL:
        id = ((struct traversal*)entry->data)->name;
        break;
    case STE_PHASE:
        id = ((struct phase*)entry->data)->name;
        break;
    case STE_NODE:
        id = ((struct node*)entry->data)->name;
        break;
    case STE_NODESET:
        id = ((struct node*)entry->data)->name;
        break;
    default:
        DBUG_ASSERT(false, "Unkown symbol table entry type.");
    }
    CTInoteContinuedObj(id->loc_info, "Also defined here.");
}

static
void CheckNamingCollision(struct ast *ast, struct id *id, enum symbol_table_entry_type type, void *data)
{
    struct symbol_table_entry *entry = NULL;
    if (CheckCollisison(ast, id->orig, &entry)) {
        CTIerrorObj(id->loc_info, "Duplicate definition of name.");
        PrintIdLocationNote(entry);
    }  else if (CheckCollisison(ast, id->lwr, &entry) || CheckCollisison(ast, id->upr, &entry)) {
        CTIerrorObj(id->loc_info, "Duplicate definition of name. Remember: CoCoNut is case insensitive.");
        PrintIdLocationNote(entry);
    } else {
        InsertEntry(ast, type, data, id->orig);
    }
}


struct ast *STdoTrav(struct ast *ast)
{
    struct traversal *trav;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        CheckNamingCollision(ast, trav->name, STE_TRAVERSAL, trav);
    }

    struct pass *pass;
    STAILQ_FOREACH(pass, ast->passes, next) {
        CheckNamingCollision(ast, pass->name, STE_PASS, pass);
    }

    struct phase *phase;
    STAILQ_FOREACH(phase, ast->phases, next) {
        CheckNamingCollision(ast, phase->name, STE_PHASE, phase);
    }

    struct nodeset *nodeset;
    STAILQ_FOREACH(nodeset, ast->nodesets, next) {
        CheckNamingCollision(ast, nodeset->name, STE_NODESET, nodeset);
    }

    struct node *node;
    STAILQ_FOREACH(node, ast->nodes, next) {
        CheckNamingCollision(ast, node->name, STE_NODE, node);
    }

    struct ccn_enum *ccn_enum;
    STAILQ_FOREACH(ccn_enum, ast->enums, next) {
        CheckNamingCollision(ast, ccn_enum->name, STE_ENUM, ccn_enum);
    }

    CTIabortOnError();

    return ast;
}