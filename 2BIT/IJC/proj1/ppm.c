// ppm.c
// Řešení IJC-DU1, příklad b), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Soubor obsahuje definice externich funkci pro cteni a zapis ppm.
//				V pripade chyby, spatneho formatu nebo nedostatku pameti vola
//				warning_msg a vraci NULL nebo -1.

#include <stdio.h>
#include "ppm.h"
#include "error.h"

#define FAILURE -1

struct ppm * ppm_read(const char * filename)
{
  FILE *fr;
  char type[3];
  unsigned size, xsize, ysize;

  if ( (fr = fopen(filename, "rb")) == NULL )       // rb for nontext files
  {
    warning_msg("Cannot open file %s.\n", filename);
    return NULL;
  }

  if (fscanf(fr, "%2s", type) != 1)
  {
    warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
    return NULL;
  }
  type[2] = '\0';

  if (strcmp(type, "P6") != 0)
  {
    warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
    return NULL;
  }

  if (fscanf(fr, " %u %u\n%u\n", &xsize, &ysize, &size) != 3)
  {
    warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
    return NULL;
  }

  if (size != 255)
  {
    warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
    return NULL;
  }

  struct ppm *p = malloc(sizeof(struct ppm) + (3 * xsize * ysize) * sizeof(char));

  if ( p == NULL )
  {
    warning_msg("Error detected while alocating memory.\n");
    fclose(fr);
    return NULL;
  }

  (*p).xsize = xsize;
  (*p).ysize = ysize;

	if(fread(&p->data, sizeof(char), (3*xsize*ysize), fr) != (3*xsize*ysize))
	{
		warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
		free(p);
    return NULL;
	} else if (fgetc(fr) != EOF)
  {
    warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fr);
		free(p);
    return NULL;
  }

  fclose(fr);
  return p;
}

int ppm_write(struct ppm *p, const char * filename)
{
  FILE *fw;

  if ( (fw = fopen(filename, "wb")) == NULL )       // wb for nontext files
  {
    warning_msg("Cannot open file %s.\n", filename);
		free(p);
    return FAILURE;
  }

  fprintf(fw, "P6\n%u %u\n255\n", (*p).xsize, (*p).ysize);

  if(fwrite(p->data, sizeof(char), (3*(*p).xsize*(*p).ysize), fw) != (3*(*p).xsize*(*p).ysize))
	{
		warning_msg("Error in formating of file %s detected.\n", filename);
    fclose(fw);
		free(p);
    return FAILURE;
	}

  fclose(fw);
  return 0;
}
