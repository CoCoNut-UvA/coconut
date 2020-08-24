%{

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "dsl-lexer.h"

#include "globals.h"
#include "ast/ast.h"
#include "ast/constructors.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "palm/set.h"

extern bool yy_lex_keywords;

static struct ctinfo *NewLocation(struct ctinfo *info);
static struct ast *ast;


void yyerror(const char* s);
int yydebug = 1;

#define YYLTYPE YYLTYPE
typedef struct ctinfo YYLTYPE;

struct ctinfo yy_ctinfo;

// Override YYLLOC_DEFAULT so we can set yy_parser_location
// to the current location
# define YYLLOC_DEFAULT(Cur, Rhs, N)                      \
    do                                                        \
    if (N) {                                                \
        (Cur).first_line   = YYRHSLOC(Rhs, 1).first_line;   \
        (Cur).first_column = YYRHSLOC(Rhs, 1).first_column; \
        (Cur).last_line    = YYRHSLOC(Rhs, N).last_line;    \
        (Cur).last_column  = YYRHSLOC(Rhs, N).last_column;  \
    }                                                     \
    else {                                                  \
        (Cur).first_line   = (Cur).last_line   =            \
            YYRHSLOC(Rhs, 0).last_line;                       \
        (Cur).first_column = (Cur).last_column =            \
            YYRHSLOC(Rhs, 0).last_column;                     \
    }                                                     \
    while (0)
%}

%union {
    int64_t intval;
    uint64_t uintval;
    long double fval;
    char *string;
    bool boolean;
    struct phase *phase;
    struct pass *pass;
    struct traversal *traversal;
    struct id *id;
    struct action_slist *actions;
    struct action *action;
    struct set *set;
    struct ccn_set_operation *ccn_set_operation;
    struct ccn_set_expr *ccn_set_expr;
    struct node *node;
    struct attribute *attribute;
    enum attribute_type attribute_type;
    struct attribute_slist *attributes;
    struct child_slist *children;
    struct child *child;
    struct nodeset *nodeset;
    struct nodeset_stailq *nodesets;
    struct ccn_enum *ccn_enum;
    struct id_slist *ids;
}

%define parse.error verbose
%locations

%token<intval> T_INTVAL "integer value"
%token<uintval> T_UINTVAL "unsigned integer value"
%token<fval> T_FLOATVAL "float value"
%token<string> T_CHARVAL "char value"
%token<string> T_STRINGVAL "string value"
%token<string> T_ID "identifier"

%token T_INT "int"
%token T_UINT "uint"

%token T_INT8 "int8"
%token T_INT16 "int16"
%token T_INT32 "int32"
%token T_INT64 "int64"
%token T_UINT8 "uint8"
%token T_UINT16 "uint16"
%token T_UINT32 "uint32"
%token T_UINT64 "uint64"

%token T_BOOL "bool"
%token T_TRUE "true"
%token T_FALSE "false"

%token T_ATTRIBUTES "attributes"
%token T_CHILDREN "children"
%token T_CONSTRUCTOR "construct"
%token T_CYCLE "cycle"
%token T_ENUM "enum"
%token T_MANDATORY "mandatory"
%token T_NODE "node"
%token T_NODES "nodes"
%token T_NODESET "nodeset"
%token T_PASS "pass"
%token T_PASSES "passes"
%token T_PHASE "phase"
%token T_PHASES "phases"
%token T_ACTIONS "actions"
%token T_PREFIX "prefix"
%token T_INFO "info"
%token T_FUNC "func"
%token T_ROOT "root"
%token T_START "start"
%token T_SUBPHASES "subphases"
%token T_TO "to"
%token T_TRAVERSAL "traversal"
%token T_TRAVDATA "travdata"
%token T_FLOAT "float"
%token T_DOUBLE "double"
%token T_STRING "string"
%token T_VALUES "values"
%token T_NULL "NULL"
%token T_LIFETIME "lifetime"
%token T_DISALLOWED "disallowed"
%token T_GATE "gate"
%token T_ARROW "->"
%token T_UNSAFE "unsafe"
%token END 0 "End-of-file (EOF)"

%type<string> info
%type<phase> phase phaseheader cycleheader
%type<pass> pass
%type<traversal> traversal
%type<id> id prefix func
%type<ids> idlist enumvalues
%type<boolean> is_start is_constructor is_root
%type<actions> actions actionsbody
%type<action> action
%type<set> setliterals
%type<ccn_set_operation> setoperation
%type<ccn_set_expr> setexpr traversalnodes
%type<node> node
%type<attribute> attribute
%type<attribute_type> attribute_primitive_type
%type<attributes> attributes attributebody
%type<child> child
%type<children> children childrenbody
%type<nodeset> nodeset
%type<ccn_enum> enum



%left '&' '-' '|'

%start root

%%

root: entries;

entries: entry ';' entries
       | entry
       | %empty
       ;

entry: phase { STAILQ_INSERT_TAIL(ast->phases, $1, next); }
     | pass { STAILQ_INSERT_TAIL(ast->passes, $1, next); }
     | traversal {STAILQ_INSERT_TAIL(ast->traversals, $1, next); }
     | node {STAILQ_INSERT_TAIL(ast->nodes, $1, next); }
     | nodeset {STAILQ_INSERT_TAIL(ast->nodesets, $1, next); }
     | enum { STAILQ_INSERT_TAIL(ast->enums, $1, next); }
     ;

/* Root of the config, creating the final config */
phase: phaseheader '{' info[information] prefix[identifier] actionsbody[actions] '}'
    {
        ASTfillPhaseHeader($1, $identifier, $information, $actions, NULL);
        $$ = $1;
        $$->loc_info = NewLocation(&@$);
    }
    | cycleheader '{' info[information] prefix[identifier] actionsbody[actions] '}'
    {
        ASTfillPhaseHeader($1, $identifier, $information, $actions, NULL);
        $$ = $1;
        $$->loc_info = NewLocation(&@$);
    }
    ;

phaseheader: is_start T_PHASE id
    {
        $$ = ASTnewPhaseHeader($3, $1, false);
    }
    ;

cycleheader: is_start T_CYCLE id
    {
        $$ = ASTnewPhaseHeader($3, $1, true);
    }
    ;

is_start: %empty
     {
        $$ = false;
     }
     | T_START
     {
        $$ = true;
     }
     ;

pass: T_PASS id[name] '{'  info[information] prefix[identifier] func[target_func]'}'
    {
        $$ = ASTnewPass($name, $identifier, $information, $target_func);
    }
    | T_PASS id[name]
    {
        $$ = ASTnewPass($name, NULL, NULL, NULL);
    }
    | T_PASS id[name] '=' id[target_func]
    {
        $$ = ASTnewPass($name, NULL, NULL, $target_func);
    }
    ;

traversal: T_TRAVERSAL id[name] '{' info[information] prefix[identifier] traversalnodes[nodes] '}'
    {
        $$ = ASTnewTraversal($name, $identifier, $information, $nodes);
    }
    ;

actionsbody: T_ACTIONS '{' actions '}'
     {
         $$ = $3;
     }
     ;

actions: action ';' actions
       {
           SLIST_INSERT_HEAD($3, $1, next);
           $$ = $3;
       }
       | action ';'
       {
           struct action_slist *actions = MEMmalloc(sizeof(struct action_slist));
           SLIST_INIT(actions);
           SLIST_INSERT_HEAD(actions, $1, next);
           $$ = actions;
       }
       ;


action: traversal
      {
          STAILQ_INSERT_TAIL(ast->traversals, $1, next);
          $$ = ASTnewAction(ACTION_TRAVERSAL, $1);
      }
      | pass
      {
          STAILQ_INSERT_TAIL(ast->passes, $1, next);
          $$ = ASTnewAction(ACTION_PASS, $1);
      }
      | phase
      {
          STAILQ_INSERT_TAIL(ast->phases, $1, next);
          $$ = ASTnewAction(ACTION_PHASE, $1);
      }
      | T_ID
      {
          $$ = ASTnewAction(ACTION_REFERENCE, $1);
          $$->loc_info = NewLocation(&@$);
      }
      ;

traversalnodes: T_NODES '=' setexpr
            {
                $$ = $3;
            }
            | %empty
            {
                $$ = NULL;
            }

setexpr: setoperation
       {
           $$ = ASTnewSetExpr(CCN_SET_OPERATION, $1);
       }
       | '(' setoperation ')'
       {
           $$ = ASTnewSetExpr(CCN_SET_OPERATION, $2);
       }
       | '{' setliterals '}'
       {
           $$ = ASTnewSetExpr(CCN_SET_LITERAL, $2);
       }
       | id
       {
           $$ = ASTnewSetExpr(CCN_SET_REFERENCE, $1);
       }
       ;

setoperation: setexpr '|' setexpr
            {
                $$ = ASTnewSetOperation(CCN_SET_UNION, $1, $3);
            }
            | setexpr '&' setexpr
            {
                $$ = ASTnewSetOperation(CCN_SET_INTERSECT, $1, $3);
            }
            | setexpr '-' setexpr
            {
                $$ = ASTnewSetOperation(CCN_SET_DIFFERENCE, $1, $3);
            }
            ;

setliterals: setliterals ',' id
           {
               SETinsert($1, $3);
               $$ = $1;
           }
           | id
           {
               $$ = ASTnewSet_Id(10);
               SETinsert($$, $1);
           }


node: is_root[root] T_NODE id[name] '{' info[information] childrenbody[children] attributebody[attributes] '}'
    {
        $$ = ASTnewNode($name, $information, $attributes, $children, $root);
    }
    ;

is_root:
    T_ROOT
    {
        $$ = true;
    }
    | %empty
    {
        $$ = false;
    }
    ;

attributebody: T_ATTRIBUTES '{' attributes '}'
    {
        $$ = $3;
    }
    | %empty
    {
        struct attribute_slist *attributes = MEMmalloc(sizeof(struct attribute_slist));
        SLIST_INIT(attributes);
        $$ = attributes;
    }
    ;

attributes: attribute ';' attributes
    {
        SLIST_INSERT_HEAD($3, $1, next);
    }
    | attribute ';'
    {
        struct attribute_slist *attributes = MEMmalloc(sizeof(struct attribute_slist));
        SLIST_INIT(attributes);
        SLIST_INSERT_HEAD(attributes, $1, next);
        $$ = attributes;
    }
    ;

attribute: attribute_primitive_type[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTnewAttribute($name, $type, $constructor);
    }
    | id[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTnewAttribute($name, AT_link_or_enum, $constructor);
        $$->type_reference = $type;
    }

attribute_primitive_type:
    T_INT
    { $$ = AT_int; }
    | T_INT8
    { $$ = AT_int8; }
    | T_INT16
    { $$ = AT_int16; }
    | T_INT32
    { $$ = AT_int32; }
    | T_INT64
    { $$ = AT_int64; }
    | T_UINT
    { $$ = AT_uint; }
    | T_UINT8
    { $$ = AT_uint8; }
    | T_UINT16
    { $$ = AT_uint16; }
    | T_UINT32
    { $$ = AT_uint32; }
    | T_UINT64
    { $$ = AT_uint64; }
    | T_FLOAT
    { $$ = AT_float; }
    | T_DOUBLE
    { $$ = AT_double; }
    | T_BOOL
    { $$ = AT_bool; }
    | T_STRING
    { $$ = AT_string; }
    ;

is_constructor:
    T_CONSTRUCTOR
    {
        $$ = true;
    }
    | %empty
    {
        $$ = false;
    }
    ;

childrenbody: T_CHILDREN '{' children  '}'
    {
        $$ = $3;
    }
    | %empty
    {
        // Create an empty list.
        struct child_slist *children = MEMmalloc(sizeof(struct child_slist));
        SLIST_INIT(children);
        $$ = children;
    }
    ;

children: child ';' children
    {
        SLIST_INSERT_HEAD($3, $1, next);
        $$ = $3;
    }
    | child ';'
    {
        struct child_slist *children = MEMmalloc(sizeof(struct child_slist));
        SLIST_INIT(children);
        SLIST_INSERT_HEAD(children, $1, next);
        $$ = children;
    }
    ;

child: id[type] id[name]
    {
        $$ = ASTnewChild($name, $type, false, false);
    }
    | id[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTnewChild($name, $type, false, $constructor);
    }
    ;

nodeset: T_NODESET id[name] '{' info[information] T_NODES '=' setexpr[expr] '}'
        {
            $$ = ASTnewNodeSet($name, $information, $expr);
            $$->loc_info = NewLocation(&@$);
        }
        | T_NODESET id[name] '=' setexpr[expr]
        {
            $$ = ASTnewNodeSet($name, NULL, $expr);
            $$->loc_info = NewLocation(&@$);
        }
        ;

enum: T_ENUM id[name] '{' info[information] prefix[identifier] enumvalues[values] '}'
    {
        $$ = ASTnewEnum($name, $identifier, $information, $values);
    }

enumvalues: T_VALUES '=' '{' idlist '}'
    {
        $$ = $4;
    }
    ;

idlist: id ',' idlist
    {
        SLIST_INSERT_HEAD($3, $1, next);
        $$ = $3;
    }
    | id
    {
        struct id_slist *ids = MEMmalloc(sizeof(struct id_slist));
        SLIST_INIT(ids);
        SLIST_INSERT_HEAD(ids, $1, next);
        $$ = ids;
    }
    ;


func: %empty
    {
        $$ = NULL;
    }
    | T_FUNC '=' id
    {
        $$ = $3;
    }
    ;

info: %empty
    {
        $$ = NULL;
    }
    | T_INFO '=' T_STRINGVAL
    {
        $$ = $3;
    }
    ;

prefix: %empty
    {
        $$ = NULL;
    }
    | T_PREFIX '=' id
    {
        $$ = $3;
    }
    ;

id: T_ID
    {
        $$ = ASTnewId($1);
        $$->loc_info = NewLocation(&@$);
    }
  ;
%%

static struct ctinfo *NewLocation(struct ctinfo *info)
{
    struct ctinfo *locinfo = MEMmalloc(sizeof(struct ctinfo));
    memcpy(locinfo, info, sizeof(struct ctinfo));
    locinfo->filename = globals.filename;
    bool found = false;
    locinfo->line = HTlookup(globals.line_map, &(info->first_line), &found);

    return locinfo;
}


struct ast *SPparseDSL(FILE *fp)
{
    ast = ASTnew();
    yyin = fp;
    yyparse();
    yylex_destroy();
    return ast;
}