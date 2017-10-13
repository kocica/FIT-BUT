/**
 * @file symtable.c
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

#include "symtable.h"

/** @brief Initial size of hash table */
static const int HTAB_SIZE = 8192;

size_t htab_size(htab *t)
{
  return t->size;
}

bool htab_empty(htab *t)
{
  return (t->size == 0);
}

void htab_add(htab *t, const char * s)
{
  unsigned int index;

  index = hash_function(s) % HTAB_SIZE;

  if (t->htab[index].declared)
  {
    /* Symbol is already defined */
  }

  t->htab[index].name = malloc(strlen(s) + 1);
  if (t->htab[index].name == NULL)
  {
    // ERROR -- INTERNAL
  }
  t->htab[index].name_len = strlen(s);

  strcpy(t->htab[index].name, s);

  t->size++;
}

void htab_remove(htab *t, const char *s)
{
  unsigned int index;

  index = hash_function(s) % HTAB_SIZE;

  if (t->htab[index].name != NULL)
  {
    t->htab[index].declared = false;
    free(t->htab[index].name);
    t->htab[index].name = NULL;
    t->size--;
  }
}

void htab_init(htab **t)
{
  *t = malloc(sizeof **t + (sizeof(htab_item) * HTAB_SIZE));
  if (*t == NULL)
  {
    error_code = E_INTERN;
    error_msg("Failed to allocate memory for hash table.");
  }
  (*t)->size = 0;

  for(int i = 0; i < HTAB_SIZE; i++)
  {
    (*t)->htab[i].name = NULL;
  }
}

void htab_free(htab **t)
{
  for (int i = 0; i < HTAB_SIZE; i++)
  {
    if ((*t)->htab[i].name != NULL)
    {
      free((*t)->htab[i].name);
    }
  }
  free(*t);
}

unsigned int hash_function(const char * str)
{
  unsigned int h=0;
  const unsigned char *p;
  for(p=(const unsigned char*)str; *p!='\0'; p++)
  {
    h = 65599*h + *p;
  }
  return h;
}
