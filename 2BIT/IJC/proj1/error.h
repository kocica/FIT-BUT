// error.h
// Řešení IJC-DU1, příklad a), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Soubor s prototypy funkci pro soubor error.c

#ifndef ERROR_H
#define ERROR_H

/*
 * Tisk varovani podle fmt
 */
void warning_msg(const char *fmt, ...);

/*
 * Vypise CHYBA: ... a ukonci program pomoci exit
 */
int error_msg(const char *fmt, ...);

#endif
