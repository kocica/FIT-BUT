// io.c
// Řešení IJC-DU2, příklad b), 2.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Ziska slovo ze vstupu.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()
#include "htab.h"

int get_word(char *s, int max, FILE *f)
{
  int c, i;
  static bool perror_once = false;

  for (i = 0; ; i++)
  {
    c = fgetc(f);
    if ( c == EOF )
      return EOF;
    else if ( isspace(c) )
    {
      s[i] = '\0';
      return i;
    }
    else if ( i == (max) )
      break;

    s[i] = c;
  }

  // Preskoci zbytek radku
  while ( ! isspace((c = fgetc(f))) )
    ;
  if ( !perror_once )
  {
    // Po prvnim prekroceni meze radku tiskne chybu na stderr
    fprintf(stderr, "Word is longer than max len [%d].\n", max);
    perror_once = true;
  }

  s[i] = '\0';
  return i;
}
