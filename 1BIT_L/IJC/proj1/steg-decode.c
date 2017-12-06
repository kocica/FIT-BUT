// steg-decode.c
// Řešení IJC-DU1, příklad b), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Ve funkci main vytvori zaznam s daty nacteymi z ppm souboru.
//				Pote alokokuje pole o N prvcich a pomoci eratosthenova sita
//				nastavi prvocisla na 0, zbytek na 1. Pote provadi dekodovani
//				zpravy, ktera je retezec znaku ukonceny '\0' a je ulozen
//				na LSB bytu, ktera jsou na indexech prvocisel.
//				Obsah zaznamu pote zapise do souboru. V pripade chyby vola
//				error_msg a ukonci program s navratovou hodnotou 1.

#include <stdio.h>
#include <limits.h> // CHAR_BIT
#include "ppm.h"
#include "bit_array.h"
#include "error.h"
#include "eratosthenes.h"

#define N 3*1000*1000

int main(int argc, char *argv[])
{
  if (argc != 2)
    error_msg("Run: ./steg-decode file.ppm\n");

  struct ppm *p = ppm_read(argv[1]);
	if ( p == NULL )
    error_msg("Message is not ended correctly with NULL char or error"
							" found in formating.\n");


  ba_create(pole,N);

	Eratosthenes(pole);

	int size = (3 * (*p).xsize * (*p).ysize);
  int pocet_bitu = 0;
  char ch = '\0';

  for (int i = 2; i < size; i++)
  {
    if(!(ba_get_bit(pole,i)))
    {
      ch |= ( (p->data[i] & 1) << (pocet_bitu++) );

      if (pocet_bitu == CHAR_BIT)
      {
				// Zarazka
        if (ch == '\0')
				{
					// Za zpravou odradkuje
					printf("\n");
					// Kvuli detekci zarazky
					ch = EOF;
					// Konec
          break;
				}

        putchar(ch);
        ch = '\0';
				pocet_bitu = 0;
      }
    }
  }

	// Zprava neni ukoncena zarazkou -> error
	if (ch != EOF)
		error_msg("Message is not ended correctly with NULL character.\n");


	// Zapise data do souboru, pokud fce vrati < 0 nastala chyba
  // if ( ppm_write(p, "vystup-xkocic01.ppm") < 0 )
  //   error_msg("Nepodarilo se zapsat do souboru.");

  free(p);
  return 0;
}
