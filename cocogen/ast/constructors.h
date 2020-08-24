#pragma once

#include "ast/ast.h"
#include <stdbool.h>

extern struct symbol_table_entry *ASTnewSymbolTableEntry(enum symbol_table_entry_type type, void *data);
extern struct phase *ASTnewPhaseHeader(struct id *identifier, bool is_start, bool is_cycle);
extern void ASTfillPhaseHeader(struct phase *header, struct id *prefix, char *info, struct action_slist *actions, struct id *gate_func);
extern struct traversal *ASTnewTraversal(struct id *identifier, struct id *prefix, char *info, struct ccn_set_expr *target_nodes);
extern struct pass *ASTnewPass(struct id *identifier, struct id *prefix, char *info, struct id *target_func);
extern struct action *ASTnewAction(enum action_type action_type, void *new_action);
extern struct ccn_set_operation *ASTnewSetOperation(enum ccn_set_operator operator, struct ccn_set_expr *left, struct ccn_set_expr *right);
extern struct ccn_set_expr *ASTnewSetExpr(enum ccn_set_type type, void *value);
extern struct node *ASTnewNode(struct id *name, char *info, struct attribute_slist *attributes, struct child_slist *children, bool is_root);
extern struct nodeset *ASTnewNodeSet(struct id *name, char *info, struct ccn_set_expr *expr);
extern struct id *ASTnewId(char *original);
extern struct ccn_enum *ASTnewEnum(struct id *name, struct id *prefix, char *info, struct id_slist *values);
extern struct attribute *ASTnewAttribute(struct id *name, enum attribute_type type, bool in_constructor);
extern struct child *ASTnewChild(struct id *name, struct id *type, bool is_mandatory, bool in_constructor);
extern struct ast *ASTnew();