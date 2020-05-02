%{
#include <stdio.h>
#include <stdbool.h>
#include <regex.h>

#include "ast/ast.h"
#include "lib/print.h"
#include "lib/errors.h"
#include "lib/array.h"

#include "dsl-parser.h"

#define YY_DECL int yylex()
#define LEX_KEYWORD(k) if (yy_lex_keywords) \
                                return k;   \
                            else {          \
                                yylval.string = strdup(yytext); \
                                return T_ID; \
                            } \

extern void exit_compile_error();

bool yy_lex_keywords = true;
static char yy_linebuf[500];
static regex_t regex;

int yycolumn = 1;

extern array *yy_lines;

static inline void token_action();

static inline void lexer_init();
static inline void check_c_keyword(const char *);

#define YY_USER_INIT lexer_init();
#define YY_USER_ACTION token_action();

%}

%option yylineno
%option nounput
%option noinput

%option stack
%option noyy_top_state

%x COMMENT
%x SAVELINE

%%
<SAVELINE>{

    .* {
        // Save the whole line
        strncpy(yy_linebuf, yytext, sizeof(yy_linebuf));

        array_append(yy_lines, strdup(yy_linebuf));

        // yylineno is set automatically to 0,
        // however the column needs to be reset.
        yycolumn = 1;

        // give back the line for the real lexing state
        yyless(0);

        // Continue to the previous lexing state
        yy_pop_state();
    }
    \n {
        // Save the empty line
        strncpy(yy_linebuf, "", sizeof(yy_linebuf));

        array_append(yy_lines, strdup(yy_linebuf));

        // yylineno is set automatically to 0,
        // however the column needs to be reset.
        yycolumn = 0;

        // Stay in the SAVELINE state, to save the next line
    }
}

\/\/[^\n]*      {  }
"/*"            { yy_push_state(COMMENT); }
<COMMENT>{
    "*/"        { yy_push_state(INITIAL); }
    \n          { yy_push_state(SAVELINE); }
    .           {  }
}
"(" {return '(';}
")" {return ')';}
"[" {return '[';}
"]" {return ']';}
"{" {return '{';}
"}" {return '}';}
"," {return ',';}
"=" {return '=';}
";" {return ';';}
"!" {return '!';}
"|" {return '|';}
"&" {return '&';}
"-" {return '-';}
"." {return '.';}

"attributes"    { LEX_KEYWORD(T_ATTRIBUTES);}
"children"      { LEX_KEYWORD(T_CHILDREN);}
"constructor"   { LEX_KEYWORD(T_CONSTRUCTOR);}
"cycle"         { LEX_KEYWORD(T_CYCLE);}
"enum"          { LEX_KEYWORD(T_ENUM);}
"mandatory"     { LEX_KEYWORD(T_MANDATORY);}
"node"          { LEX_KEYWORD(T_NODE);}
"nodeset"       { LEX_KEYWORD(T_NODESET);}
"nodes"         { LEX_KEYWORD(T_NODES);}
"pass"          { LEX_KEYWORD(T_PASS);}
"passes"        { LEX_KEYWORD(T_PASSES);}
"phase"         { LEX_KEYWORD(T_PHASE);}
"phases"        { LEX_KEYWORD(T_PHASES);}
"actions"       { LEX_KEYWORD(T_ACTIONS);}
"prefix"        { LEX_KEYWORD(T_PREFIX);}
"subphases"     { LEX_KEYWORD(T_SUBPHASES);}
"to"            { LEX_KEYWORD(T_TO);}
"traversal"     { LEX_KEYWORD(T_TRAVERSAL);}
"travdata"      { LEX_KEYWORD(T_TRAVDATA);}
"values"        { LEX_KEYWORD(T_VALUES) ; }
"info"          { LEX_KEYWORD(T_INFO) ; }
"func"          { LEX_KEYWORD(T_FUNC) ; }
"root"          { LEX_KEYWORD(T_ROOT) ; }
"start"         { LEX_KEYWORD(T_START); }
"double"        { LEX_KEYWORD(T_DOUBLE);}
"float"         { LEX_KEYWORD(T_FLOAT);}
"int"           { LEX_KEYWORD(T_INT);}
"uint"          { LEX_KEYWORD(T_UINT);}
"int8"          { LEX_KEYWORD(T_INT8);}
"int16"         { LEX_KEYWORD(T_INT16);}
"int32"         { LEX_KEYWORD(T_INT32);}
"int64"         { LEX_KEYWORD(T_INT64);}
"uint8"         { LEX_KEYWORD(T_UINT8);}
"uint16"        { LEX_KEYWORD(T_UINT16);}
"uint32"        { LEX_KEYWORD(T_UINT32);}
"uint64"        { LEX_KEYWORD(T_UINT64);}
"string"        { LEX_KEYWORD(T_STRING);}
"bool"          { LEX_KEYWORD(T_BOOL);}
"lifetime"      { LEX_KEYWORD(T_LIFETIME);}
"disallowed"    { LEX_KEYWORD(T_DISALLOWED);}
"->"            { LEX_KEYWORD(T_ARROW);}
"false"         { LEX_KEYWORD(T_FALSE);}
"true"          { LEX_KEYWORD(T_TRUE);}
"gate"          { LEX_KEYWORD(T_GATE);}
"unsafe"        { LEX_KEYWORD(T_UNSAFE);}

"NULL"          { return T_NULL;}


[A-Za-z][A-Za-z0-9_]*  {
    // Identifier start with an letter and can have numbers and underscores.
    yylval.string = strdup(yytext);
    check_c_keyword(yylval.string);
    return T_ID; }
-?[0-9]+\.[0-9]+ {
    // (negative) floating point numbers
    yylval.fval = strtold(yytext, NULL); return T_FLOATVAL;}
-[0-9]+ {
    // (negative) integer numbers
    yylval.intval = (int64_t)strtoll(yytext, NULL, 10); return T_INTVAL;}
[0-9]+ {
    // (positive) integer numbers
    yylval.uintval = (uint64_t)strtoull(yytext, NULL, 10); return T_UINTVAL;}

\"(\\.|[^\\"])*\"   {
    // Remove surrounding quotes
    yylval.string = strndup(yytext + 1, yyleng - 2); return T_STRINGVAL;}

[ \t\r]   ; // ignore all whitespace

\n  {
    // Switch to the SAVELINE state to read the whole line first
    yy_push_state(SAVELINE);
}
. {
    print_error_at(yylineno, yycolumn - 1, yy_linebuf,
        "Unexpected token found, quit parsing", yytext);
    exit_compile_error();
}
%%

static inline void lexer_init(void) {
    // We start by reading the whole first line, and then continue lexing it
    yy_push_state(SAVELINE);

    // Compile a regex for checking C-keywords as identifiers.
    const char* regex_str = "^\\(auto\\|break\\|case\\|char\\|const\\|continue"
        "\\|default\\|do\\|double\\|else\\|enum\\|extern\\|float\\|for\\|goto"
        "\\|if\\|inline\\|int\\|long\\|register\\|restrict\\|return\\|short"
        "\\|signed\\|sizeof\\|static\\|struct\\|switch\\|typedef\\|union"
        "\\|unsigned\\|void\\|volatile\\|while\\)$";
    int reti = regcomp(&regex, regex_str, 0);
    if (reti) {
        print_internal_error("Could not compile C keyword regex.");
        exit(REGEX_COMPILE_ERROR);
    }
}

static inline void check_c_keyword(const char *str) {
    if (!yy_lex_keywords) {
        // values of enum are not a problem.
        return;
    }
    // Match for C-keywords.
    int reti = regexec(&regex, str, 0, NULL, 0);
    if (!reti) {
        print_error_range(
                yylineno, yycolumn - strlen(str), yycolumn - 1,
                yy_linebuf, "Found C-keyword as identifier, "
                "which would generate invalid C code.");
        exit_compile_error();
    }
}

static inline void token_action() {
    if (YY_START != SAVELINE) {
        yylloc.first_line = yylloc.last_line = yylineno;
        yylloc.first_column = yycolumn;
        yylloc.last_column = yycolumn + yyleng - 1;
        yycolumn += yyleng;
    }
}

int yywrap() {
    return (1);
}

void yyerror(const char* s) {
    print_error_range(yylineno, yycolumn - yyleng, yycolumn - 1,
        yy_linebuf, (char*) s);
    exit_compile_error();
}
