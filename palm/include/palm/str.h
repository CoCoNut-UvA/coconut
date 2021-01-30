#pragma once

#include <stdbool.h>

extern char *STRcpy(const char *source);
extern char *STRncpy(const char *source, int maxlen);
extern char *STRcat(const char *first, const char* second);
extern char *STRcatn(int n, ...);
extern char *STRtok(const char *str, const char *tok);
extern bool STReq(const char *first, const char* second);
extern bool STReqci(const char *first, const char* second);
extern bool STRprefix( const char *prefix, const char *str);
extern bool STRsuffix( const char *suffix, const char *str);
extern bool STReqn(const char *first, const char* second, int n);
extern bool STRsub(const char *sub, const char *str);
extern int  STRlen(const char *str);
extern char *STRonNull( char *alt,  char *str);
extern char *STRsubStr( const char *string, int start, int len);
extern char *STRnull( );
extern char *STRitoa(int number);
extern char *STRsubstToken( const char *str, const char *token, const char *subst);
extern void STRtoLower(char *str);
extern void STRtoUpper(char *str);
extern char *STRlower(const char *str);
extern char *STRupper(const char *str);
#define F_PTR "%p"

