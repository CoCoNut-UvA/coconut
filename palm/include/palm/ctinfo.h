#pragma once

#include <stdarg.h>

struct ctinfo {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    char *filename;
    char *line;
};

extern void CTIinstallInterruptHandlers(void);
extern void CTIerror(const char *format, ...);
extern void CTIerrorObj(struct ctinfo *info, const char *format, ...);
extern void CTIerrorObjVA(struct ctinfo *info, const char *format, va_list arg_p);
extern void CTIerrorContinued(const char *format, ...);
extern void CTIerrorContinuedObj(struct ctinfo *info, const char *format, ...);
extern int CTIgetErrorMessageLineLength();
extern void CTIabortOnError(void);
extern void CTIabort(const char *format, ...);
extern void CTIabortObj(struct ctinfo *info, const char *format, ...);
extern void CTIabortObjVA(struct ctinfo *info, const char *format, va_list arg_p);
extern void CTIwarn(const char *format, ...);
extern void CTIwarnObj(struct ctinfo *info, const char *format, ...);
extern void CTIwarnObjVA(struct ctinfo *info, const char *format, va_list arg_p);
extern void CTIwarnContinued(const char *format, ...);
extern int CTIgetWarnMessageLineLength(void);
extern int CTIgetWarnings();
extern void CTIstate(const char *format, ...);
extern void CTInote(const char *format, ...);
extern void CTInoteObj(struct ctinfo *info, const char *format, ...);
extern void CTInoteObjVA(struct ctinfo *info, const char *format, va_list arg_p);
extern void CTInoteContinuedObj(struct ctinfo *info, const char *format, ...);
extern void CTInoteContinued(const char *format, ...);
extern void CTIabortOutOfMemory(unsigned int request);
extern void CTIwarnContinuedObj(struct ctinfo *info, const char *format, ...);
