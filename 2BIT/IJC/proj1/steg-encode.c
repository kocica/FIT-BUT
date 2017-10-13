// steg-encode.c [dobrovolny]
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Zakoduje zpravu zadanou tretim argumentem na nejnizsi bity (LSB)
//				RGB dat obrazku ve formatu PPM P6.

#include <stdio.h>
#include <string.h>
#include "bit_array.h"
#include "ppm.h"
#include "error.h"
#include "eratosthenes.h"

#define N 3*1000*1000

int main (int argc, char *argv[])
{
	if (argc != 4)
	{
		error_msg("Program args: source (in P6), destination, message");
	}

	struct ppm *p = ppm_read(argv[1]);
		if (p == NULL)
			return 1;

	ba_create(pole,N);

	Eratosthenes(pole);

	int size = (3 * (*p).xsize * (*p).ysize);
	short index = 0;
	short index_bit = 1;
	short index_byte = 0;

	for (unsigned long i = 2; i < size; i++)
	{
		if (!(ba_get_bit(pole,i)))
		{
			p->data[i] &= 0xFE;
			p->data[i] |= ((argv[3][index_byte] & index_bit) >> (index));

			if (++index == 8) index = 0;

			if ((index_bit <<= 1) == 256)
			{
				index_bit = 1;
				if (++index_byte > (unsigned int)strlen(argv[3]))
					break;
  		}
		}
	}

	if (ppm_write(p, argv[2]) < 0)
		return 1;

	return 0;
}
