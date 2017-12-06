// htab_bucket_count.c
// Řešení IJC-DU2, příklad b), 7.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Vraci pocet prvku tabulky (arr_size).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

size_t htab_bucket_count(struct htab *t)
{
  return t->arr_size;
}
