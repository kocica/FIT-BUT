// htab_move.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Presune tabulku do nove tabulky.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

struct htab * htab_move(size_t newsize, struct htab *t2)
{
  struct htab *t = htab_init(newsize);
  if ( t == NULL )
  {
    fprintf(stderr, "Cannot allocate memory.\n");
    return NULL;
  }

  struct htab_listitem *item = NULL;
  struct htab_listitem *item_next = NULL;

  for (size_t i = 0; i < t2->arr_size; i++)
  {
    item = t2->htab[i];
    while ( item != NULL )
    {
      item_next = item->next;
      item->next = NULL;

      struct htab_listitem *tmp = NULL;

      if ( ( tmp = htab_find(item->key, t)) != NULL )
      {
        tmp->data++;
        item = item_next;
        continue;
      }

      unsigned int index = ( hash_function(item->key) % t->arr_size );

      tmp = t->htab[index];

      t->n++;

      if ( tmp == NULL )
      {
        t->htab[index] = item;
        item = item_next;
        continue;
      }

      while ( tmp->next != NULL )
        tmp = tmp->next;

      tmp->next = item;
      item = item_next;
    }
    t2->htab[i] = NULL;
  }

  t2->n = 0;
  t2->arr_size = 0;
  return t;
}
