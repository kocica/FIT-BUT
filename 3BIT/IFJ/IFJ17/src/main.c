/**
 * @file main.c
 *
 * IAL; IFJ project 2017 -- IFJ17 Compiler
 *
 * <br><b>IFJ17 Compiler<b>
 * <br>
 * <br>Compiles IFJ17 language (subset of FreeBASIC) source code to  IFJcode17 intercode.
 * <br>
 * <br> 1) Read source code from standard input
 * <br> 2) Perform syntactic analyzation
 * <br> 3) Compile source code to target code
 * <br> 4) Print intercode to standard output
 * <br>
 * <br><b>Team<b>
 * <br>  Filip Kocica   (xkocic01)
 * <br>  Matej Knazik   (xknazi00)
 * <br>  Andrea Fickova (xficko00)
 * <br>  Jiri Fiala     (xfiala54)
 *
 * @author(s) Filip Kocica (xkocic01@fit.vutbr.cz), FIT
 * @date      24.9.2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "parser.h"
#ifdef SCANNER_DEBUG
  #include "scanner.h"
#endif

#include "symtable.h"

int main()
{
  //parser();

  #ifdef SCANNER_DEBUG
    scanner();
  #endif

  htab *h;
  htab_init(&h);

  printf("%d-%zu\n", htab_empty(h), htab_size(h));

  htab_add(h, "x");
  htab_add(h, "y");
  htab_add(h, "z");
  htab_remove(h, "y");

  printf("%d-%zu\n", htab_empty(h), htab_size(h));

  htab_free(&h);

  return 0;
}
