// bit_aray.h
// Řešení IJC-DU1, příklad a), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Je potreba zvetsit zasobnik - na unix-like systemech ulimit -s N KiB
//        Funkce alokuje pole bitu pro 303m cisel, pote pomoci
//        eratostheneova sita nastavi prvocisla na 0 zbytek na 1
//				a vzestupne vypise poslednich 10 prvocisel.

#include <stdio.h>
#include <stdbool.h> // bool parameter v inline fci
#include "eratosthenes.h"
#include "error.h"
#include "bit_array.h"

#define N 303000000L

int main (void)
{
  ba_create(p, N);

  Eratosthenes(p);

	unsigned long primes[10];
	short j = 0;

  for(unsigned long i = N; i > 1; i--)
  {
    if(!(ba_get_bit(p,i)))
		{
			primes[j++] = i;
			if(j == 10) break;
		}
  }

	while(j)
		printf("%lu\n", primes[--j]);

	return 0;
}

