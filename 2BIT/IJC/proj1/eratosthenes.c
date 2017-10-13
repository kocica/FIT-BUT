// eratosthenes.c
// Řešení IJC-DU1, příklad a), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Funkce pomoci algoritmu znameho jako eratosthenovo sito
//				nastavi bity na indexech bitoveho pole pole na 1, kdyz
//				dane cislo neni prvocislo, zbytek cisel necha na 0.

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "bit_array.h"
#include "eratosthenes.h"

void Eratosthenes(bit_array_t pole)
{
	unsigned long limit = ((ba_size(pole)-1) * UL_BIT);

  for (unsigned long i = 2; i <= sqrt(limit); i++)
  {
    if (!(ba_get_bit(pole,i)))
    	for (unsigned long j = i*i; j < limit; j += i)
   	  {
    	  ba_set_bit(pole,j,1);
    	}
  }
}
