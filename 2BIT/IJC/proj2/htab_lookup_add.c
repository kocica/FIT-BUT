// htab_lookup_add.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Hleda v tabulce t polozku s klicem key, pokud najde
//        vraci pointer na ni, jinak vytvori novou polozku
//        a vraci pointer na tuto novou polozku.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

// funkce ocekava dynamicky alokovany klic
struct htab_listitem * htab_lookup_add(struct htab *t, char *key)
{
  struct htab_listitem *item = NULL;

  if ( ( item = htab_find(key, t)) != NULL )
  {
    free(key);
    return item;
  }

  item = malloc(sizeof(struct htab_listitem));
  // error handler v mainu
  if ( item == NULL )
    return NULL;

  // item init
  item->key = key;
  item->data = 1;
  item->next = NULL;

  unsigned int index = ( hash_function(key) % t->arr_size );

  struct htab_listitem *tmp = t->htab[index];

  t->n++;

  if ( tmp == NULL )
  {
    t->htab[index] = item;
    return item;
  }

  while ( tmp->next != NULL )
    tmp = tmp->next;

  tmp->next = item;
  return item;
}
