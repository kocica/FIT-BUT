// ppm.h
// Řešení IJC-DU1, příklad b), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Knihovna ppm.h s podminenym prekladem deklaruje zaznam ppm s rozmery
//        a RGB daty ulozenymi ve flexibilnim poli. Funkce ppm_read zajistuje
//        nacteni do dynamicky alokovane struktury a ppm_write zapis do souboru.
//        Obe funkce jsou osetreny proti chybam.

#ifndef PPM_H
#define PPM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ppm {
  unsigned xsize;
  unsigned ysize;
  char data[];
};

struct ppm * ppm_read(const char * filename);

int ppm_write(struct ppm *p, const char * filename);

#endif
