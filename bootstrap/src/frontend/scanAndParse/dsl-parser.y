%{

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "dsl-lexer.h"

#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "palm/set.h"

#include "ccngen/ast.h"
#include "ccngen/enum.h"

extern bool yy_lex_keywords;
static node_st *ast;
static int trav_index = 0;
static int node_index = 0;

static struct ctinfo *NewLocation(struct ctinfo *info);


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
    char* string;
    bool boolean;
    node_st* node;
    enum attribute_type attr_type;
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
%type<boolean> is_start is_constructor is_root
%type<node> phase entry pass node traversal cycleheader phaseheader id action actionsbody traversalnodes prefix
    actions childrenbody attributebody attributes attribute children child setoperation setliterals func
    setexpr enum idlist enumvalues nodeset travdata travdatalist travdataitem nodelifetimes
    lifetimes lifetime lifetime_range
%type<attr_type> attribute_primitive_type

%left '&' '-' '|'

%start root

%%

root: entries;

entries: entry ';' entries
       | entry
       | %empty
       ;

entry: phase
    {
        IPHASE_NEXT($1) = AST_IPHASES(ast);
        AST_IPHASES(ast) = $1;
        $$ = ast;
    }
     | pass
     {
        IPASS_NEXT($1) = AST_IPASSES(ast);
        AST_IPASSES(ast) = $1;
        $$ = ast;
     }
     | traversal
     {
        ITRAVERSAL_NEXT($1) = AST_ITRAVERSALS(ast);
        AST_ITRAVERSALS(ast) = $1;
        $$ = ast;
     }
     | node
     {
        INODE_INDEX($1) = node_index++;
        INODE_NEXT($1) = AST_INODES(ast);
        AST_INODES(ast) = $1;
        $$ = ast;
     }
     | nodeset
     {
        INODESET_NEXT($1) = AST_INODESETS(ast);
        AST_INODESETS(ast) = $1;
        $$ = ast;
     }
     | enum
     {
        IENUM_NEXT($1) = AST_ENUMS(ast);
        AST_ENUMS(ast) = $1;
        $$ = ast;
     }
     ;

/* Root of the config, creating the final config */
phase: phaseheader '{' info[information] prefix[identifier] actionsbody[actions] '}'
    {
        $$ = $1;
        IPHASE_IPREFIX($$) = $identifier;
        IPHASE_IINFO($$) = $information;
        IPHASE_IACTIONS($$) = $actions;
    }
    | cycleheader '{' info[information] prefix[identifier] actionsbody[actions] '}'
    {
        $$ = $1;
        IPHASE_IPREFIX($$) = $identifier;
        IPHASE_IINFO($$) = $information;
        IPHASE_IACTIONS($$) = $actions;
    }
    ;

phaseheader: is_start T_PHASE id
    {
        $$ = ASTiphase($3, $1);
    }
    ;

cycleheader: is_start T_CYCLE id
    {
        $$ = ASTiphase($3, $1);
        IPHASE_IS_CYCLE($$) = true;
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
        $$ = ASTipass($name, $information, $identifier);
        IPASS_TARGET_FUNC($$) = $target_func;
    }
    | T_PASS id[name]
    {
        $$ = ASTipass($name, NULL, NULL);
    }
    | T_PASS id[name] '=' id[target_func]
    {
        $$ = ASTipass($name, NULL, NULL);
        IPASS_TARGET_FUNC($$) = $target_func;
    }
    ;

traversal: T_TRAVERSAL id[name] '{' info[information] prefix[identifier] traversalnodes[nodes] travdata[data]'}'
    {
        $$ = ASTitraversal($name);
        ITRAVERSAL_INDEX($$) = trav_index++;
        ITRAVERSAL_IINFO($$) = $information;
        ITRAVERSAL_IPREFIX($$) = $identifier;
        ITRAVERSAL_INODES($$) = $nodes;
        ITRAVERSAL_DATA($$) = $data;
    }
    | T_TRAVERSAL id[name]
    {
        $$ = ASTitraversal($name);
        ITRAVERSAL_INDEX($$) = trav_index++;
    }
    ;

actionsbody: T_ACTIONS '{' actions '}'
     {
        $$ = $3;
     }
     ;

actions: action ';' actions
       {
            IACTIONS_NEXT($1) = $3;
            $$ = $1;
       }
       | action ';'
       {
            $$ = $1;
       }
       ;


action: traversal
      {
        $$ = ASTiactions();
        IACTIONS_REFERENCE($$) = ITRAVERSAL_NAME($1);
        ITRAVERSAL_NEXT($1) = AST_ITRAVERSALS(ast);
        AST_ITRAVERSALS(ast) = $1;
      }
      | pass
      {
        $$ = ASTiactions();
        IACTIONS_REFERENCE($$) = IPASS_NAME($1);
        IPASS_NEXT($1) = AST_IPASSES(ast);
        AST_IPASSES(ast) = $1;
      }
      | phase
      {
        $$ = ASTiactions();
        IACTIONS_REFERENCE($$) = IPHASE_NAME($1);
        IPHASE_NEXT($1) = AST_IPHASES(ast);
        AST_IPHASES(ast) = $1;
      }
      | id
      {
        $$ = ASTiactions();
        IACTIONS_REFERENCE($$) = $1;
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
            $$ = $1;
       }
       | '(' setoperation ')'
       {
            $$ = $2;
       }
       | '{' setliterals '}'
       {
            $$ = $2;
       }
       | id
       {
            $$ = ASTsetreference();
            SETREFERENCE_REFERENCE($$) = $1;
       }
       ;

setoperation: setexpr '|' setexpr
            {
                $$ = ASTsetoperation($1, $3, SO_iunion);
            }
            | setexpr '&' setexpr
            {
                $$ = ASTsetoperation($1, $3, SO_intersect);
            }
            | setexpr '-' setexpr
            {
                $$ = ASTsetoperation($1, $3, SO_difference);
            }
            ;

setliterals: setliterals ',' id
           {
                $$ = ASTsetliteral();
                SETLITERAL_REFERENCE($$) = $3;
                SETLITERAL_NEXT($$) = $1;
           }
           | id
           {
                $$ = ASTsetliteral();
                SETLITERAL_REFERENCE($$) = $1;
           }


node: is_root[root] T_NODE id[name] '{' info[information] childrenbody[children] attributebody[attributes] nodelifetimes[lifetimes] '}'
    {
        $$ = ASTinode($name, $information);
        INODE_IS_ROOT($$) = $root;
        INODE_ICHILDREN($$) = $children;
        INODE_IATTRIBUTES($$) = $attributes;
        INODE_LIFETIMES($$) = $lifetimes;
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
        $$ = NULL;
    }
    ;

attributes: attribute ';' attributes
    {
        ATTRIBUTE_NEXT($1) = $3;
        $$ = $1;
    }
    | attribute ';'
    {
        $$ = $1;
    }
    ;

attribute: attribute_primitive_type[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTattribute();
        ATTRIBUTE_NAME($$) = $name;
        ATTRIBUTE_TYPE($$) = $type;
        ATTRIBUTE_IN_CONSTRUCTOR($$) = $constructor;
    }
    | attribute_primitive_type[type] id[name]
    {
          $$ = ASTattribute();
          ATTRIBUTE_NAME($$) = $name;
          ATTRIBUTE_TYPE($$) = $type;
    }
    | id[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTattribute();
       ATTRIBUTE_NAME($$) = $name;
       ATTRIBUTE_TYPE_REFERENCE($$) = $type;
       ATTRIBUTE_TYPE($$) = AT_link_or_enum;
       ATTRIBUTE_IN_CONSTRUCTOR($$) = $constructor;
    }
    | id[type] id[name]
    {
           $$ = ASTattribute();
           ATTRIBUTE_NAME($$) = $name;
           ATTRIBUTE_TYPE_REFERENCE($$) = $type;
           ATTRIBUTE_TYPE($$) = AT_link_or_enum;
    }
    ;

attribute_primitive_type: T_BOOL
    { $$ = AT_bool; }
    | T_STRING
    { $$ = AT_string; }
    | T_INT
    { $$ = AT_int; }
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
        $$ = NULL;
    }
    ;

children: child ';' children
    {
        CHILD_NEXT($1) = $3;
        $$ = $1;
    }
    | child ';'
    {
        $$ = $1;
    }
    ;

child: id[type] id[name]
    {
        $$ = ASTchild($name);
        CHILD_TYPE_REFERENCE($$) = $type;
    }
    | id[type] id[name] '{' is_constructor[constructor] '}'
    {
        $$ = ASTchild($name);
        CHILD_TYPE_REFERENCE($$) = $type;
        CHILD_IN_CONSTRUCTOR($$) = $constructor;
    }
    ;

nodelifetimes: %empty
    {
        $$ = NULL;
    }
    | T_LIFETIME '{' lifetimes '}'
    {
        $$ = $3;
    }
    ;

lifetimes: lifetime ';' lifetimes
    {
        $$ = $1;
        ILIFETIME_NEXT($1) = $3;
    }
    | lifetime ';'
    {
        $$ = $1;
    }
    ;

lifetime:
    T_DISALLOWED
    {
        $$ = ASTilifetime();
        ILIFETIME_TYPE($$) = LT_disallowed;
    }
    | T_MANDATORY
    {
        $$ = ASTilifetime();
        ILIFETIME_TYPE($$) = LT_mandatory;
    }
    ;



nodeset: T_NODESET id[name] '{' info[information] T_NODES '=' setexpr[expr] '}'
        {
            $$ = ASTinodeset();
            INODESET_EXPR($$) = $expr;
            INODESET_NAME($$) = $name;
            INODESET_IINFO($$) = $information;
        }
        | T_NODESET id[name] '=' setexpr[expr]
        {
            $$ = ASTinodeset();
            INODESET_EXPR($$) = $expr;
            INODESET_NAME($$) = $name;
        }
        ;

enum: T_ENUM id[name] '{' info[information] prefix[identifier] enumvalues[values] '}'
    {

        $$ = ASTienum($values, $name, $identifier, $information);
    }

enumvalues: T_VALUES '=' '{' idlist '}'
    {
        $$ = $4;
    }
    ;


travdata: T_TRAVDATA '=' '{' travdatalist '}'
    {
        $$ = $4;
    }
    | %empty
    {
        $$ = NULL;
    }
    ;

travdatalist: travdatalist ',' travdataitem
        {
            ITRAVDATA_NEXT($3) = $1;
            $$ = $3;
        }
        | travdataitem
        {
            $$ = $1;
        }
        ;

travdataitem: attribute_primitive_type[type] id[name]
    {
        $$ = ASTitravdata($name);
        ITRAVDATA_TYPE($$) = $type;
    }
    | id[type] id[name]
    {
        $$ = ASTitravdata($name);
        ITRAVDATA_TYPE_REFERENCE($$) = $type;
        ITRAVDATA_TYPE($$) = AT_link_or_enum;
    }
    | T_UNSAFE id[type] id[name]
    {
            $$ = ASTitravdata($name);
            ITRAVDATA_TYPE_REFERENCE($$) = $type;
            ITRAVDATA_TYPE($$) = AT_user;
    }
    ;


idlist: id ',' idlist
    {
        ID_NEXT($1) = $3;
        $$ = $1;
    }
    | id
    {
        $$ = $1;
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
        $$ = ASTid($1, STRlower($1), STRupper($1));
        ID_ROW($$) = @1.first_line;
        ID_COL_BEGIN($$) = @1.first_column;
        ID_COL_END($$) = @1.last_column;
    }
  ;
%%

static struct ctinfo *NewLocation(struct ctinfo *info)
{
    struct ctinfo *locinfo = MEMmalloc(sizeof(struct ctinfo));
    memcpy(locinfo, info, sizeof(struct ctinfo));
    locinfo->filename = globals.filename;
    locinfo->line = HTlookup(globals.line_map, &(info->first_line));

    return locinfo;
}


node_st *SPparseDSL(FILE *fp)
{
    ast = ASTast();
    yyin = fp;
    yyparse();
    yylex_destroy();
    AST_NUM_TRAVERSALS(ast) = trav_index;
    AST_NUM_NODES(ast) = node_index;
    fclose(fp);
    return ast;
}
