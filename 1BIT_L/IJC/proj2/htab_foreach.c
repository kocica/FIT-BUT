// htab_foreach.c
// Řešení IJC-DU2, příklad b), 4.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Pro kazdou polozku zavola funkci func, predanou jako
//        ukazatel na fci.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

void htab_foreach(struct htab *t,
                  void (*func)(const char *key, unsigned int value))
{
  struct htab_listitem *item = NULL;

  for (size_t n = 0; n < t->arr_size; n++)
  {
    item = t->htab[n];
    while (item != NULL)
    {
      func(item->key, item->data);
      item = item->next;
    }
  }
}
