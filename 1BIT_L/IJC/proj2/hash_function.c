// hash_function.c
// Řešení IJC-DU2, příklad b), 2.4.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Ziska ciselnou hodnotu retezce pomoci vhodne
//        hashovaci funkce.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp
#include <stdbool.h> // bool
#include <ctype.h>   // isspace()

unsigned int hash_function(const char *str)
{
  unsigned int h=0;
  const unsigned char *p;
  for(p=(const unsigned char*)str; *p!='\0'; p++)
    h = 65599*h + *p;
  return h;
}
