// wordcount.c
// Řešení IJC-DU2, příklad b), 2.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Prvni inicializujeme novou tabulku o vhodne velikosti
//        pote cteme jednotlive slova ze vstupu a pridavame je
//        do jednotlivych vhodnych seznamu v tabulce.
//        Index do tabulky ziskame pomoci vhodne hashovaci fce
//        ulozene v souboru hash_function.
//        Nakonec celou tabulku uvolnime.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

int main(void)
{
  // delka nacteneho slova
  int word_len;

  // Vysvetleni velikosti v htab.h
  size_t arr_size = HTAB_SIZE;

  struct htab *t = htab_init(arr_size);
  char *key;

  if ( t == NULL )
  {
    fprintf(stderr, "Cannot allocate memory!\n");
    return 1;
  }

  // Alokuj si pamet pro klic
  key = malloc(sizeof(char) * MAX_WORD_LEN);
  // Dokud jsou na vstupu slova
  while ( (word_len = get_word(key, MAX_WORD_LEN, stdin)) != EOF )
  {
    // a jsou delsi nez 0 znaku
    if ( word_len > 0 )
    {
      // tak je pridej do tabulky
      if ( htab_lookup_add(t, key) == NULL )
      {
        fprintf(stderr, "Cannot allocate memory!\n");
        return 1;
      }
      key = malloc(sizeof(char) * MAX_WORD_LEN);
    }
  }
  free(key);

  htab_foreach(t, &htab_print);

  htab_free(t);
  return 0;
}
