// htab.h
// Řešení IJC-DU2, příklad b), 2.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Soubor htab.h obsahuje spolecne rozhrani vsech
//        zdrojovych souboru pro druhy ukol.
//        Ma podmineny preklad.
//        Obsahuje definici limitu velikosti jednoho
//        slova, struktur popisujici jednotlive polozky
//        seznamu a take celou tabulku seznamu.

#ifndef HTAB_H
#define HTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()

/******** OPTIMALNI VELIKOST TABULKY ********

  -Velikost
    Mela by byt prvocislo, ktere se blizi
    k mocnine 2.

  -Jednotky
    Cas   [s] -- bez vypisu tabulky
    Pamet [B]

  -Testovano na
    verze se statickou knihovnou


  === Testy pro /usr/share/dict/words ===

  1] 4129 (blizi se k 2^12)
    - Spotreba pameti: 15.012.092
    - Doba trvani    : 0.165

  2] 16384 (blizi se k 2^14)
    - Spotreba pameti: 15.110.132
    - Doba trvani    : 0.092

  3] 32779 (blizi se k 2^15)
    - Spotreba pameti: 15.241.292
    - Doba trvani    : 0.084

  4] 131063 (blizi se k 2^17)
    - Spotreba pameti: 16.027.564
    - Doba trvani    : 0.082

  === Testy pro seq 1000000 2000000|shuf ===

  1] 4129 (blizi se k 2^12)
    - Spotreba pameti: 151.037.422
    - Doba trvani    : 14.874

  2] 16384 (blizi se k 2^14)
    - Spotreba pameti: 151.135.462
    - Doba trvani    : 14.458

  3] 32779 (blizi se k 2^15)
    - Spotreba pameti: 151.266.622
    - Doba trvani    : 2.443

  4] 131063 (blizi se k 2^17)
    - Spotreba pameti: 152.052.894
    - Doba trvani    : 1.890

  -Vysledek
    Vybral jsem velikost tabulky 32779
    Protoze poskytuje kompromis mezi
    casem a spotrebovanou pameti.

********************************************/
#define HTAB_SIZE         32779

/**
 * @brief Maximalni delka slova.
 */
#define MAX_WORD_LEN      127 // 127 znaku + '\0'

/**
 * @brief Reprezentace jedne polozky v seznamu
 *
 * Struktura obsahuje polozku data s poctem vyskytu polozek
 * se stejnym klicem. Obsahuje ukazatel na dalsi prvek
 * v seznamu.
 */
struct htab_listitem {
  unsigned int data;
  char *key;
  struct htab_listitem *next;
};

/**
 * @brief Reprezentace hashovaci tabulky.
 *
 * Obsahuje velikost tabulky, neboli pocet linearnich seznamu
 * a take pocet vsech polozek ve vsech seznamech dohromady.
 * Pomoci flexible array member je definovana tabulka pointeru
 * na jednotlive seznamy.
 */
struct htab {
  size_t arr_size;
  size_t n;
  struct htab_listitem *htab[];
};


/**
 * @brief Ziska slovo ze vstupu.
 *
 * @param s[in] Klic do ktereho ukladame slovo.
 * @param max[in] Maximalni delka slova.
 * @param f[in] Soubor ze ktereho cteme.
 * @return Delku nacteneho slova.
 */
int get_word(char *s, int max, FILE *f);

/**
 * @brief Presune tabulku do nove tabulky ktera je vetsi/mensi.
 *
 * @param newsize[in] Velikost nove tabulky.
 * @param t2[in] Stara tabulka.
 * @return Novou alokovanou tabulku nebo nullptr.
 */
struct htab * htab_move(size_t newsize, struct htab *t2);

/**
 * @brief Ziska ciselnou hodnotu retezce.
 *
 * @param str[in] Retezec, ktery chceme zaheshovat.
 * @return Ciselnou hodnotu retezce.
 */
unsigned int hash_function(const char *str);

/**
 * @brief Inicializuje novou tabulku.
 *
 * @param size[in] Velikost nove tabulky.
 * @return Novu tabulku.
 */
struct htab * htab_init(int size);

/**
 * @brief Uvolni jednotlive polozky tabulky.
 *
 * @param t[in] Tabulka jez chceme vycistit.
 */
void htab_clear(struct htab *t);

/**
 * @brief Zavola htab_clear a pote uvolni celou tabulku.
 *
 * @param t[in] Tabulku jez chceme uvolnit.
 */
void htab_free(struct htab *t);

/**
 * @brief Hleda v tabulce t polozku s klicem key, pokud najde
 *        vraci pointer na ni, jinak vytvori novou polozku
 *        a vraci pointer na tuto novou polozku.
 *
 * @param t[in] Tabulka v niz hledame a pridavame do ni polozky.
 * @param key[in] Klic podle ktereho kledame polozku.
 * @return Nalezenou nebo vztvorenou polozku.
 */
struct htab_listitem * htab_lookup_add(struct htab *t, char *key);

/**
 * @brief Nalezne polozku v tabulce a vraci pointer na ni nebo NULL.
 *
 * @param key[in] Klic podle ktereho kledame polozku.
 * @param t[in] Tabulka v niz hledame polozky.
 * @return Polozku nalezenou nebo nullptr.
 */
struct htab_listitem * htab_find(const char *key, struct htab *t);

/**
 * @brief V tabulce t nalezne polozku s klicem key a odstrani ji.
 *
 * @param t[in] Tabulka z nic odtranujeme.
 * @param key[in] Polozka kterou chceme odstranit.
 * @return Uspech / neuspech.
 */
bool htab_remove(struct htab *t, char *key);

/**
 * @brief Tiskne jednu polozku tabulky.
 *
 * @param key[in] Klic ktery tiskneme.
 * @param value[in] Pocet vyskytu teto polozky.
 */
void htab_print(const char *key, unsigned int value);

/**
 * @brief Pro kazdou polozku zavola funkci func.
 *
 * @param t[in] Tabulka.
 * @param func[in] Funkce kterou volame pro kazdou polozku tabulky.
 */
void htab_foreach(struct htab *t,
                  void (*func)(const char *key, unsigned int value));

/**
 * @brief Vraci velikost tabulky (arr_size).
 *
 * @param t[in] Tabulka.
 * @return Velikost tabulky.
 */
size_t htab_size(struct htab *t);

/**
 * @brief Vraci pocet polozek v tabulce.
 *
 * @param t[in] Tabulka.
 * @return Pocet polozek v tabulce.
 */
size_t htab_bucket_count(struct htab *t);

#endif  /* ! HTAB_H */
