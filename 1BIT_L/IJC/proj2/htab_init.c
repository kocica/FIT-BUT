// htab_init.c
// Řešení IJC-DU2, příklad b), 3.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Inicializuje novou tabulku a vraci ji nebo NULL pri chybe.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

struct htab * htab_init(int size)
{
  struct htab *t = malloc(sizeof(struct htab) +
                         (sizeof(struct htab_listitem *) * size));
  if ( t == NULL )
  {
    // error handler je v mainu
    return NULL;
  }

  t->arr_size = size;
  t->n = 0;
  for (int i = 0; i < size; i++)
    t->htab[i] = NULL;

  return t;
}
