// tail.c
// Řešení IJC-DU2, příklad a), 27.3.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Funkce orezava vystup na poslednich N (implicitne 10)
//        radku a ty tiskne na stdout. Data bere ze souboru zadaneho
//        jako parametr nebo stdin v pripade absence souboru.
//        Pocet tisknutych radku se da upravit prepinacem -n N. N > 0.
//        Nactena data uklada do dynamicky alokovaneho "2D pole".

#define _GNU_SOURCE // getline
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tail.h"

int input_handler(int argc, char *argv[], char **filename)
{
  if (argc > 4)
    return FAILURE;

  else if (argc == 1)
    return NO_ROWS_IMP;

  switch (argc)
  {
		case 4:  if ( !strcmp ("-n", argv[1]) &&
								  atoi (argv[2]) > 0 )
             {
               filename[0] = argv[3];
               return atoi (argv[2]);
             }

						 else if ( !strcmp ("-n", argv[2]) &&
								  atoi (argv[3]) > 0 )
             {
               filename[0] = argv[1];
               return atoi (argv[3]);
             }
             break;

		case 3:	 if ( !strcmp ("-n", argv[1]) &&
									atoi (argv[2]) > 0 )
					   return atoi (argv[2]);
             break;

    case 2:  filename[0] = argv[1];
             return NO_ROWS_IMP;
	}

	return FAILURE;
}

void myperror(int code)
{
  switch (code)
  {
    case ERR_WINPUT:
		     fprintf(stderr, "ERROR #1: Insufficient input.\n");
         return ;

    case ERR_FILE_OPEN:
		     fprintf(stderr, "ERROR #2: Cannot open file.\n");
         return ;

    case ERR_MALLOC:
		     fprintf(stderr, "ERROR #3: Cannot allocate memory.\n");
         return ;

    case ERR_ROW_OVERFLOW:
         fprintf(stderr, "ERROR #4: First overflow row max len. [%u].\n", ROW_MAX_LEN);
         return ;
  }
}

char **alloc_rows(int n, int row_len)
{
	char **rows;

	if ( (rows = calloc(n, sizeof(char *))) == NULL)
		return NULL;

	for (int i = 0; i < n; i++)
	{
		if ( (rows[i] = calloc(row_len, sizeof(char))) == NULL )
			return NULL;
	}

  return rows;
}

void tail(char **rows, int rows_print, FILE *fr)
{
  char *row = NULL;
  int rows_file = 0;
	int rows_counter = 0;
	short first_err = 1;
  size_t len = 0;
  ssize_t row_len = 0;

  // Dokud jsou v souboru radky
  while ( (row_len = getline(&row, &len, fr)) >= 0 )
  {
    // Radek presahuje max -> zkrat ho na max
    if ( row_len > ROW_MAX_LEN )
		{
			if ( first_err )
			{
				myperror(ERR_ROW_OVERFLOW);
				first_err = 0;
			}
			row[ROW_MAX_LEN - 1] = '\0';
		}
    else if (row_len != 1)
      row[row_len - 1] = '\0';

    // Jsem na konci pole -> posun prvky
    if ( rows_file == rows_print )
    {
      shift_rows(rows, rows_print);
      rows_file--;
    }

    // Na posledni prvek pridej novy radek
    strcpy(rows[rows_file],row);
		if ( rows_file < rows_print )
      rows_file++;
		rows_counter++;
  }

	// Pokud chci tisknout vice radku nez mam nacteno, tisknu vsechny nactene
	if ( rows_counter < rows_print )
  	print_rows(rows, rows_counter);
	else
	  print_rows(rows, rows_print);

  // Uvolni pamet
  free(row);
}

void shift_rows(char **rows, int n)
{
  for (int i = 0; i < n-1; i++)
  {
    strcpy(rows[i], rows[i+1]);
  }
}

void free_rows(char **rows, int n)
{
  for (int i = 0; i < n; i++)
  {
    free(rows[i]);
  }

  free(rows);
}


void print_rows(char **rows, int n)
{
  for (int i = 0; i < n; i++)
  {
    printf("%s", rows[i]);
    if ( rows[i][(int)strlen(rows[i]) - 1] != '\n' )
      puts("");
  }
}


int main (int argc, char *argv[])
{
  char **rows;
  char *filename = NULL;
  int rows_print = input_handler(argc, argv, &filename);
  FILE *fr = stdin;

  // Spatne zadany vstup
	if ( rows_print < 0 )
	{
    myperror(ERR_WINPUT);
		return EXIT_FAILURE;
	}

  // Bylo zadano jmeno souboru (implicitne stdin)
  if ( filename != NULL )
  {
    if ( (fr = fopen(filename, "r")) == NULL )
    {
      myperror(ERR_FILE_OPEN);
		  return EXIT_FAILURE;
    }
  }

  // Nepodarilo se alokovat pamet
  if ( (rows = alloc_rows(rows_print, ROW_MAX_LEN)) == NULL )
  {
    myperror(ERR_MALLOC);
		fclose(fr);
		return EXIT_FAILURE;
  }

  tail(rows, rows_print, fr);

  // ============ UVOLNENI PAMETI =================
  fclose(fr);
  free_rows(rows, rows_print);
  // =========== ! UVOLNENI PAMETI ================

	return EXIT_SUCCESS;
}
