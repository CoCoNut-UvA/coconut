%{

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/create.h"

#include "dsl-lexer.h"

#include "lib/array.h"
#include "lib/imap.h"
#include "lib/print.h"
#include "lib/str.h"

extern bool yy_lex_keywords;

/* Array to append config entries to during reducing */
static array *config_phases;
static array *config_passes;
static array *config_enums;
static array *config_traversals;
static array *config_nodesets;
static array *config_nodes;

static struct Config* parse_result = NULL;

char *yy_filename;
array *yy_lines;
imap_t *yy_parser_locations;

void yyerror(const char* s);
int yydebug = 1;

#define YYLTYPE YYLTYPE
typedef struct ParserLocation YYLTYPE;

struct ParserLocation yy_parser_location;

static void new_location(void *ptr, struct ParserLocation *loc);

// Override YYLLOC_DEFAULT so we can set yy_parser_location
// to the current location
#define YYLLOC_DEFAULT(Cur, Rhs, N)                         \
    if (N) {                                                \
        (Cur).first_line   = YYRHSLOC(Rhs, 1).first_line;   \
        (Cur).first_column = YYRHSLOC(Rhs, 1).first_column; \
        (Cur).last_line    = YYRHSLOC(Rhs, N).last_line;    \
        (Cur).last_column  = YYRHSLOC(Rhs, N).last_column;  \
    } else {                                                \
        (Cur).first_line   = (Cur).last_line   =            \
          YYRHSLOC(Rhs, 0).last_line;                       \
        (Cur).first_column = (Cur).last_column =            \
          YYRHSLOC(Rhs, 0).last_column;                     \
    }                                                       \
    yy_parser_location = (Cur);

%}

%union {
    int64_t intval;
    uint64_t uintval;
    long double fval;
    char *string;
    char *str;
    struct array *array;
    struct Config *config;
    struct Phase *phase;
    struct Pass *pass;
    struct Traversal *traversal;
    struct TravData *travdata;
    struct Enum *attr_enum;
    struct Nodeset *nodeset;
    struct SetExpr *setexpr;
    struct SetOperation *setoperation;
    struct Action *action;
    struct Lifetime *lifetime;
    struct Range_spec *range_spec;
    struct Node *node;
    struct Child *child;
    struct PhaseRange *phaserange;
    struct MandatoryPhase *mandatoryphase;
    enum AttrType attrtype;
    struct Attr *attr;
    struct AttrValue *attrval;
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

%type<string> info func prefix
%type<array> idlist actionsbody actions lifetimelistwithvalues namespacelist
             travdata travdatalist attrlist attrs
             childlist children enumvalues lifetimelist
%type<attrval> attrval
%type<attrtype> attrprimitivetype
%type<attr> attr attrhead
%type<child> child
%type<pass> pass
%type<node> nodebody node
%type<nodeset> nodeset
%type<phase> phase phaseheader
%type<attr_enum> enum
%type<traversal> traversal
%type<travdata> travdataitem
%type<config> root
%type<setexpr> setexpr traversalnodes
%type<setoperation> setoperation
%type<action> action
%type<lifetime> lifetime lifetimewithvalues
%type<range_spec> rangespec_start rangespec_end

%left '&' '-' '|'

%start root

%%

/* Root of the config, creating the final config */
root: entries { parse_result = create_config(config_phases,
                                 config_passes,
                                 config_traversals,
                                 config_enums,
                                 config_nodesets,
                                 config_nodes);
              }
              ;

entries: entry ';' entries
       | entry
       | %empty
       ;

/* For every entry in the config, append to the correct array. */
entry: phase { array_append(config_phases, $1); }
     | pass { array_append(config_passes, $1); }
     | traversal { array_append(config_traversals, $1); }
     | enum { array_append(config_enums, $1); }
     | nodeset { array_append(config_nodesets, $1); }
     | node { array_append(config_nodes, $1);  }
     ;

prefix: T_PREFIX '=' T_ID
      {
          $$ = $3;
          new_location($$, &@$);
      }

phase: phaseheader '{' prefix ',' T_GATE ',' actionsbody '}'
     {
         $$ = create_phase($1, NULL, $3, $7, ccn_str_cat($1->id, "_gate"));
     }
     | phaseheader '{' prefix ',' T_ROOT '=' T_ID ',' actionsbody '}'
     {
         $$ = create_phase($1, $7, $3, $9, NULL);
         new_location($7, &@7);
     }
     | phaseheader '{' prefix ',' actionsbody '}'
     {
         $$ = create_phase($1, NULL, $3, $5, NULL);
     }
     | phaseheader '{' prefix ',' T_ROOT '=' T_ID ',' T_GATE ',' actionsbody '}'
     {
         $$ = create_phase($1, $7, $3, $11, ccn_str_cat($1->id, "_gate"));
         new_location($7, &@7);
     }
     | phaseheader '{' prefix ',' T_ROOT '=' T_ID ',' T_GATE '=' T_STRINGVAL ',' actionsbody '}'
     {
         $$ = create_phase($1, $7, $3, $13, $11);
         new_location($11, &@11);
         new_location($7, &@7);
     }
     | phaseheader '{' prefix ',' T_GATE '=' T_STRINGVAL ',' actionsbody '}'
     {
         $$ = create_phase($1, NULL, $3, $9, $7);
         new_location($7, &@7);
     }
     | phaseheader '{' T_GATE '=' T_STRINGVAL ',' actionsbody '}'
     {
         $$ = create_phase($1, NULL, NULL, $7, $5);
         new_location($5, &@5);
     }
     | phaseheader '{' T_GATE ',' actionsbody '}'
     {
         $$ = create_phase($1, NULL, NULL, $5, ccn_str_cat($1->id, "_gate"));
     }


     ;


actionsbody: T_ACTIONS '{' actions '}'
     {
         $$ = $3;
     }
     ;

actions: actions  action ';'
       {
           array_append($1, $2);
           $$ = $1;
       }
       | action ';'
       {
           array *tmp = create_array();
           array_append(tmp, $1);
           $$ = tmp;
       }
       ;


action: traversal
      {
          $$ = create_action(ACTION_TRAVERSAL, $1, $1->id);
      }
      | pass
      {
          $$ = create_action(ACTION_PASS, $1, $1->id);
      }
      | phase
      {
          $$ = create_action(ACTION_PHASE, $1, $1->id);
      }
      | T_ID
      {
          $$ = create_action(ACTION_REFERENCE, $1, $1);
          new_location($1, &@1);
      }
      ;

phaseheader: T_PHASE T_ID
           {
               $$ = create_phase_header($2, false, false);
               new_location($$, &@$);
               new_location($2, &@2);
           }
           | T_CYCLE T_ID
           {
               $$ = create_phase_header($2, false, true);
               new_location($$, &@$);
               new_location($2, &@2);
           }
           | T_START T_PHASE T_ID
           {
               $$ = create_phase_header($3, true, false);
               new_location($$, &@$);
               new_location($3, &@3);
           }
           | T_START T_CYCLE T_ID
           {
               $$ = create_phase_header($3, true, true);
               new_location($$, &@$);
               new_location($3, &@3);
           }
           ;

pass: T_PASS T_ID '{' prefix ',' T_FUNC '=' T_ID '}'
    {
        $$ = create_pass($2, $8, $4);
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($8, &@8);
    }
    | T_PASS T_ID '{' info ',' prefix ',' T_FUNC '=' T_ID '}'
    {
        $$ = create_pass($2, $10, $6);
        $$->info = $4;
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($10, &@10);
    }
    | T_PASS T_ID '{' info ',' prefix '}'
    {
        $$ = create_pass($2, NULL, $6);
        $$->info = $4;
        new_location($$, &@$);
        new_location($2, &@2);
    }
    | T_PASS T_ID
    {
        $$ = create_pass($2, NULL, NULL);
        new_location($$, &@$);
        new_location($2, &@2);
    }
    | T_PASS T_ID '=' T_ID
    {
        $$ = create_pass($2, $4, NULL);
    }
    ;


traversal: T_TRAVERSAL T_ID
         {
             $$ = create_traversal($2, NULL, NULL, NULL, NULL);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' func '}'
         {
             $$ = create_traversal($2, $6, $4, NULL, NULL);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' func ',' traversalnodes '}'
         {
             $$ = create_traversal($2, $6, $4, $8, NULL);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' traversalnodes '}'
         {
             $$ = create_traversal($2, NULL, $4, $6, NULL);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix '}'
         {
             $$ = create_traversal($2, NULL, $6, NULL, NULL);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' func '}'
         {
             $$ = create_traversal($2, $8, $6, NULL, NULL);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' func ',' traversalnodes '}'
         {
             $$ = create_traversal($2, $8, $6, $10, NULL);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' traversalnodes '}'
         {
             $$ = create_traversal($2, NULL, $6, $8, NULL);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' travdata '}'
         {
             $$ = create_traversal($2, NULL, NULL, NULL, $4);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' travdata '}'
         {
             $$ = create_traversal($2, NULL, $4, NULL, $6);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' func ',' travdata '}'
         {
             $$ = create_traversal($2, $6, $4, NULL, $8);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' func ',' traversalnodes ',' travdata '}'
         {
             $$ = create_traversal($2, $6, $4, $8, $10);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' prefix ',' traversalnodes ',' travdata '}'
         {
             $$ = create_traversal($2, NULL, $4, $6, $8);
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' travdata '}'
         {
             $$ = create_traversal($2, NULL, $6, NULL, $8);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' func ',' travdata '}'
         {
             $$ = create_traversal($2, $8, $6, NULL, $10);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' func ',' traversalnodes ',' travdata '}'
         {
             $$ = create_traversal($2, $8, $6, $10, $12);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '{' info ',' prefix ',' traversalnodes ',' travdata '}'
         {
             $$ = create_traversal($2, NULL, $6, $8, $10);
             $$->info = $4;
             new_location($$, &@$);
             new_location($2, &@2);
         }
         | T_TRAVERSAL T_ID '=' setexpr
         {
            $$ = create_traversal($2, NULL, NULL, $4, NULL);
            new_location($$, &@$);
            new_location($2, &@2);
         }
         ;

func: T_FUNC '=' T_ID
    {
        $$ = $3;
        new_location($3, &@3);
    }
    ;

traversalnodes: T_NODES '=' setexpr
              {
                  $$ = $3;
              }
              ;

enum: T_ENUM T_ID '{' T_PREFIX '=' T_ID ',' enumvalues '}'
    {
        $$ = create_enum($2, $6, $8);
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($6, &@6);
    }
    | T_ENUM T_ID '{' enumvalues ',' T_PREFIX '=' T_ID '}'
    {
        $$ = create_enum($2, $8, $4);
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($8, &@6);
    }
    | T_ENUM T_ID '{' info ',' T_PREFIX '=' T_ID ',' enumvalues '}'
    {
        $$ = create_enum($2, $8, $10);
        $$->info = $4;
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($8, &@8);
    }
    | T_ENUM T_ID '{' info ',' enumvalues ',' T_PREFIX '=' T_ID '}'
    {
        $$ = create_enum($2, $10, $6);
        $$->info = $4;
        new_location($$, &@$);
        new_location($2, &@2);
        new_location($10, &@10);
    }
    ;


enumvalues: T_VALUES '=' '{'
        {
            yy_lex_keywords = false;
        }
        idlist  '}'
        {
            $$ = $5;
            yy_lex_keywords = true;
        }
        ;

nodeset: T_NODESET T_ID '{' T_NODES '=' setexpr '}'
        {
            $$ = create_nodeset($2, $6);
            new_location($$, &@$);
            new_location($2, &@2);
        }
       | T_NODESET T_ID '{' info ',' T_NODES '=' setexpr '}'
       {
           $$ = create_nodeset($2, $8);
           $$->info = $4;
           new_location($$, &@$);
           new_location($2, &@2);
       }
       | T_NODESET T_ID '=' setexpr
       {
           $$ = create_nodeset($2, $4);
           new_location($$, &@$);
           new_location($2, &@2);
       }
       ;

setexpr: setoperation
       {
           $$ = create_set_expr(SET_OPERATION, $1);
           new_location($$, &@$);
       }
       | '(' setoperation ')'
       {
           $$ = create_set_expr(SET_OPERATION, $2);
           new_location($$, &@$);
       }
       | '{' idlist '}'
       {
           $$ = create_set_expr(SET_LITERAL, $2);
           new_location($$, &@$);
       }
       | T_ID
       {
           $$ = create_set_expr(SET_REFERENCE, $1);
           new_location($$, &@$);
       }
       ;

setoperation: setexpr '|' setexpr
            {
                $$ = create_set_operation(SET_UNION, $1, $3);
            }
            | setexpr '&' setexpr
            {
                $$ = create_set_operation(SET_INTERSECT, $1, $3);
            }
            | setexpr '-' setexpr
            {
                $$ = create_set_operation(SET_DIFFERENCE, $1, $3);
            }
            ;

node: T_NODE T_ID '{' nodebody '}'
    {
        $$ = create_node($2, $4);
        new_location($$, &@$);
        new_location($2, &@2);
    }
    | T_ROOT T_NODE T_ID '{' nodebody '}'
    {
        $$ = create_node($3, $5);
        $$->root = true;
        new_location($$, &@$);
        new_location($3, &@3);
    }
    | T_NODE T_ID '{' nodebody T_LIFETIME '{' lifetimelist '}' '}'
    {
        $$ = create_node($2, $4);
        $$->lifetimes = $7;
        new_location($$, &@$);
        new_location($2, &@2);
    }
    ;


/* All possible combinations of children attrs and flags, with allowing empty. */
nodebody: children ',' attrs
        {
            $$ = create_nodebody($1, $3);
            new_location($$, &@$);
        }
        | children
        {
            $$ = create_nodebody($1, NULL);
            new_location($$, &@$);
        }
        | attrs
        {
            $$ = create_nodebody(NULL, $1);
            new_location($$, &@$);
        }
        | info ',' children ',' attrs
        {
            $$ = create_nodebody($3, $5);
            $$->info = $1;
            new_location($$, &@$);
        }
        | info ',' children
        {
            $$ = create_nodebody($3, NULL);
            $$->info = $1;
            new_location($$, &@$);
        }
        | info ',' attrs
        {
            $$ = create_nodebody(NULL, $3);
            $$->info = $1;
            new_location($$, &@$);
        }
        ;


lifetimelistwithvalues: lifetimelistwithvalues ',' lifetimewithvalues
        {
            array_append($$, $3);
        }
        | lifetimewithvalues
        {
            $$ = array_init(2);
            array_append($$, $1);
        }
        ;

lifetimewithvalues: T_DISALLOWED rangespec_start T_ARROW rangespec_end '=' '{' idlist '}'
        {
            $$ = create_lifetime($2, $4, LIFETIME_DISALLOWED, $7);
            new_location($$, &@$);
        }
        | lifetime
        {
            $$ = $1;

        }
        | T_DISALLOWED '=' '{' idlist '}'
        {
            $$ = create_lifetime(NULL, NULL, LIFETIME_DISALLOWED, $4);
            new_location($$, &@$);
        }
        ;

lifetimelist: lifetimelist ',' lifetime
            {
                array_append($$, $3);
            }
            | lifetime
            {
                $$ = array_init(2);
                array_append($$, $1);
                new_location($1, &@1);
            }

lifetime: T_DISALLOWED rangespec_start T_ARROW rangespec_end
        {
            $$ = create_lifetime($2, $4, LIFETIME_DISALLOWED, NULL);
            new_location($$, &@$);
        }
        | T_DISALLOWED
        {
            $$ = create_lifetime(NULL, NULL, LIFETIME_DISALLOWED, NULL);
            new_location($$, &@$);
        }
        | T_MANDATORY rangespec_start T_ARROW rangespec_end
        {
            $$ = create_lifetime($2, $4, LIFETIME_MANDATORY, NULL);
            new_location($$, &@$);
        }
        | T_MANDATORY
        {
            $$ = create_lifetime(NULL, NULL, LIFETIME_MANDATORY, NULL);
            new_location($$, &@$);
        }
        ;

namespacelist: namespacelist '.' T_ID
            {
                array_append($1, $3);
                new_location($3, &@3);
            }
            | T_ID
            {
                $$ = create_array();
                array_append($$, $1);
                new_location($1, &@1);
            }

rangespec_start: '('namespacelist
          {
              $$ = create_range_spec(false, $2);
              new_location($$, &@$);
          }
          | '('
          {
              $$ = NULL;
          }
          | '['namespacelist
          {
              $$ = create_range_spec(true, $2);
              new_location($$, &@$);
          }
          ;

rangespec_end: namespacelist ')'
          {
              $$ = create_range_spec(false, $1);
              new_location($$, &@$);
          }
          | ')'
          {
              $$ = NULL;
          }
          | namespacelist ']'
          {
              $$ = create_range_spec(true, $1);
              new_location($$, &@$);
          }
          ;

children: T_CHILDREN '{' childlist '}'
        {
            $$ = $3;
            new_location($$, &@$);
        }
        ;

childlist: childlist ',' child
         {
             array_append($1, $3);
             $$ = $1;
             // $$ is an array and should not be in the locations list
         }
         | child
         {
             array *tmp = create_array();
             array_append(tmp, $1);
             $$ = tmp;
             // $$ is an array and should not be in the locations list
         }
         ;
/* [construct] [mandatory] ID ID */
child: T_ID T_ID
     {
         $$ = create_child(0, NULL, $2, $1);
         new_location($$, &@$);
         new_location($1, &@1);
         new_location($2, &@2);
     }
     | T_ID T_ID '{' T_CONSTRUCTOR ',' lifetimelist '}'
     {
         $$ = create_child(1, $6, $2, $1);
         new_location($$, &@$);
         new_location($1, &@1);
         new_location($2, &@2);
     }
     | T_ID T_ID '{' lifetimelist ',' T_CONSTRUCTOR '}'
     {
         $$ = create_child(1, $4, $2, $1);
         new_location($$, &@$);
         new_location($1, &@1);
         new_location($2, &@2);
     }
     | T_ID T_ID '{' T_CONSTRUCTOR '}'
     {
         $$ = create_child(1, NULL, $2, $1);
         new_location($$, &@$);
         new_location($1, &@1);
         new_location($2, &@2);
     }
     | T_ID T_ID '{' lifetimelist '}'
     {
         $$ = create_child(0, $4, $2, $1);
         new_location($$, &@$);
         new_location($1, &@1);
         new_location($2, &@2);
     }
     ;
attrs: T_ATTRIBUTES '{' attrlist '}'
     { $$ = $3; }
     ;
attrlist: attrlist ',' attr
        {
            array_append($1, $3);
            $$ = $1;
            // $$ is an array and should not be in the locations list
        }
        | attr
        {
            array *tmp = create_array();
            array_append(tmp, $1);
            $$ = tmp;
            // $$ is an array and should not be in the locations list
        }
        ;
attr: attrhead '{' T_CONSTRUCTOR ',' lifetimelistwithvalues '}'
    {
        $$ = create_attr($1, NULL, 1, $5);
        new_location($$, &@$);
    }
    | attrhead '{' T_CONSTRUCTOR '}'
    {
        $$ = create_attr($1, NULL, 1, NULL);
        new_location($$, &@$);
    }
    | attrhead '=' attrval
    {
        $$ = create_attr($1, $3, 0, NULL);
        new_location($$, &@$);
    }
    ;
/* Optional [construct] keyword, for adding to constructor. */
attrhead: attrprimitivetype T_ID
        {
            $$ = create_attrhead_primitive($1, $2);
            new_location($$, &@$);
            new_location($2, &@2);
        }
        | T_ID T_ID
        {
            $$ = create_attrhead_idtype($1, $2);
            new_location($$, &@$);
            new_location($1, &@1);
            new_location($2, &@2);
        }
        ;

attrprimitivetype: T_INT
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
attrval: T_STRINGVAL
       { $$ = create_attrval_string($1); }
       | T_INTVAL
       { $$ = create_attrval_int($1); }
       | T_UINTVAL
       { $$ = create_attrval_uint($1); }
       | T_FLOATVAL
       { $$ = create_attrval_float($1); }
       | T_ID
       { $$ = create_attrval_id($1); }
       |  T_TRUE
       { $$ = create_attrval_bool(true); }
       | T_FALSE
       { $$ = create_attrval_bool(false); }
       | T_NULL
       { $$ = NULL; }
       ;

/* Comma seperated list of identifiers. */
idlist: idlist ',' T_ID
      {
          array_append($1, $3);
          $$ = $1;
          // $$ is an array and should not be added to location list.
          new_location($3, &@3);
      }
      | T_ID
      {
          array *tmp = create_array();
          array_append(tmp, $1);
          $$ = tmp;
          // $$ is an array and should not be added to location list.
          new_location($1, &@1);
      }
      ;

info: T_INFO '=' T_STRINGVAL
    {
        $$ = $3;
        new_location($$, &@$);
        new_location($3, &@3);
    }

travdata: T_TRAVDATA '=' '{' travdatalist '}'
    { $$ = $4; }
     ;

travdatalist: travdatalist ',' travdataitem
        {
            array_append($1, $3);
            $$ = $1;
            // $$ is an array and should not be in the locations list
        }
        | travdataitem
        {
            array *tmp = create_array();
            array_append(tmp, $1);
            $$ = tmp;
            // $$ is an array and should not be in the locations list
        }
        ;

travdataitem: T_UNSAFE T_ID T_ID '{' T_STRINGVAL '}'
    {
        $$ = create_travdata_struct($2, $3, $5);
        new_location($$, &@$);
        new_location($3, &@2);
        new_location($5, &@5);
    }
    | attrprimitivetype T_ID '=' attrval
    {
        $$ = create_travdata_primitive($1, $2, $4);
        new_location($$, &@$);
        new_location($2, &@2);
    }
    | attrprimitivetype T_ID
    {
        $$ = create_travdata_primitive($1, $2, NULL);
        new_location($$, &@$);
        new_location($2, &@2);
    }
    ;
%%

static void new_location(void *ptr, struct ParserLocation *loc) {
    struct ParserLocation *loc_copy = malloc(sizeof(struct ParserLocation));
    memcpy(loc_copy, loc, sizeof(struct ParserLocation));

    imap_insert(yy_parser_locations, ptr, loc_copy);
}

struct Config* parseDSL(FILE *fp) {
    yyin = fp;
    config_phases = create_array();
    config_passes = create_array();
    config_enums = create_array();
    config_traversals = create_array();
    config_nodesets = create_array();
    config_nodes = create_array();

    yy_lines = array_init(32);
    yy_parser_locations = imap_init(128);

    print_init_compilation_messages(NULL, yy_filename,
        yy_lines, yy_parser_locations);
    yyparse();
    yylex_destroy();

    return parse_result;
}
