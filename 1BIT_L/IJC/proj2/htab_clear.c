// htab_clear.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Uvolni jednotlive polozky tabulky.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

void htab_clear(struct htab *t)
{
  struct htab_listitem *i1, *i2;

  // loop over all lists
  for (size_t i = 0; i < t->arr_size; i++)
  {
    // get first item of each list
    i1 = t->htab[i];
    // loop while u are not at the tail of list
    while ( i1 != NULL )
    {
      // get adress of next item
      i2 = i1->next;
      // it is faster to free it directly
      free(i1->key);
      free(i1);
      // also possible throught remove function (slow)
      //htab_remove(t, i1->key);
      i1 = i2;
    }
  }

  // cleared table has 0 items and 0 lists
  t->n = t->arr_size = 0;
}
