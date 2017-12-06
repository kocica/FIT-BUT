// htab_find.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Nalezne polozku v tabulce a vraci pointer na ni nebo nullptr.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

struct htab_listitem * htab_find(const char *key, struct htab *t)
{
  struct htab_listitem *item;

  int i = ( hash_function(key) % t->arr_size );
  item = t->htab[i];
  while ( item != NULL )
  {
    if ( !strcmp(item->key, key) )
    {
      //t->n++;
      item->data++;
      return item;
    }
    item = item->next;
  }

  return NULL;
}
