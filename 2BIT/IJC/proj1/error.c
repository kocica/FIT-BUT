// error.c
// Řešení IJC-DU1, příklad a), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Soubor s definicemi funkci pro tisk chybovych hlaseni
//        a v pripade error_msg ukonceni programu

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void warning_msg(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, arg);
    va_end(arg);
}

int error_msg(const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
}
