// tail.h
// Řešení IJC-DU2, příklad a), 4.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Funkce orezava vystup na poslednich N (implicitne 10)
//        radku a ty tisknou na stdout. Data nacita ze souboru zadaneho
//        jako parametr nebo stdin v pripade absence souboru.
//        Pocet tisknutych radku se da upravit prepinacem -n N. N > 0.
//        Nactena data uklada do dynamicky alokovaneho "2D pole".

#ifndef TAIL_H
#define TAIL_H

#define _GNU_SOURCE // getline
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** @brief Maximalni delka radku, zbytek radku se preskakuje */
#define ROW_MAX_LEN    1024
/** @brief Ve funkci vznikla chyba */
#define FAILURE        -1
/** @brief Funkce probehla v poradku */
#define OK             0
/** @brief Implicitni pocet poslednich radku k tisknuti */
#define NO_ROWS_IMP    10

/** @brief Error kody pro fci myperror
 *
 * OVERFLOW - doslo k prekroceni max velikosti radku
 * OPEN     - nepovedlo se otevrit soubor
 * MALLOC   - nepovedlo se alokovat pamet
 * WINPUT   - neplatny vstup
*/
enum {
  ERR_ROW_OVERFLOW = -1,
  ERR_FILE_OPEN    = -2,
  ERR_MALLOC       = -3,
  ERR_WINPUT       = -4
};

/**
 * @brief Kontroluje platnost vstupnich dat a nacita parametry
 *
 * Funkce v zavislosti na poctu parametru (argc) kontroluje
 * platnost vstupnich dat. Pri zadanem souboru umisti ukazatel
 * na prislusny argv do filename.
 *
 * @param argc [in] Pocet parametru predanych programu
 * @param argv [in] Parametry predane programu
 * @param filename [out] Ukazatel na jmeno souboru (argv[{1|3}]) nebo null
 * @return Pocet radku k tisknuti nebo -1 pri chybe
*/
int input_handler(int argc, char *argv[], char **filename);

/**
 * @brief Podle kodu tiskne chybovou hlasku na stderr.
 *
 * @param code[in] Kod chyboveho stavu.
 */
void myperror(int code);

/**
 * @brief Tiskne jednotlive radky na stdout.
 *
 * @param rows[in] Radky.
 * @param n[in] Pocet radku k tisknuti.
 */
void print_rows(char **rows, int n);

/**
 * @brief Alokuje pamet pro n radku s delkou row_len.
 *
 * @param n[in] Pocet radku.
 * @param row_len[in] Max delka jednoho radku.
 * @return Alokovanou 'tabulku'.
 */
char **alloc_rows(int n, int row_len);

/**
 * @brief Posune radky o jedna dolu, prvni vypadne ven z tabulky.
 *
 * @param rows[in] Tabulka.
 * @param n[in] Pocet radku tabulky.
 */
void shift_rows(char **rows, int n);

/**
 * @brief Provede cely program tail.
 *
 * @param rows[in] Tabulka.
 * @param rows_print[in] Kolik radku chceme tisknout.
 * @param f[in] Soubor ze ktereho cteme.
 */
void tail(char **rows, int rows_print, FILE *fr);

/**
 * @brief Uvolni jednotlive rady tabulky a tabulku samotnou.
 *
 * @param rows[in] Tabulka.
 * @param n[in] Pocet radku tabulky.
 */
void free_rows(char **rows, int n);

#endif /* ! TAIL_H */
