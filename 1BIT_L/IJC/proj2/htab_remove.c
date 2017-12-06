// htab_remove.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: V tabulce t nalezne polozku s klicem key a odstrani ji.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

bool htab_remove(struct htab *t, char *key)
{
  struct htab_listitem *i1, *i2;

  // find item we wanna remove
  i1 = htab_find(key, t);

  // item doesnt exist
  if ( i1 == NULL )
    return false;

  // item exist
  t->n--;

  // get index of list in the table
  int index = ( hash_function(key) % t->arr_size );
  // get first item
  i2 = t->htab[index];

  // if we want remove first item of list
  if ( i1 == i2 )
  {
    // list has only one item
    if ( i1->next == NULL )
    {
      free(i1->key);
      free(i1);
      t->htab[index] = NULL;
      return true;
    }
    // list has more items
    else
    {
      i2 = i1->next;
      free(i1->key);
      free(i1);
      t->htab[index] = i2;
      return true;
    }
  }

  // loop while u didnt match removed item
  while ( i2->next != i1 )
    i2 = i2->next;

  // last item of list ?
  if ( i1->next == NULL )
    i2->next = NULL;
  else
    i2->next = i1->next;

  // now we can free memory and return true-success
  free(i1->key);
  free(i1);

  return true;
}
