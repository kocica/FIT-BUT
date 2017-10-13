/**
 * @file stack.c
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

#include "stack.h"

static const int STACK_RESIZE_CHUNK = 10;
static const int STACK_EMPTY = -1;

void stack_init ( TStack* s )
{
  s->arr = malloc ((sizeof *(s->arr)) * STACK_RESIZE_CHUNK);
  s->top = STACK_EMPTY;
  s->stack_size = STACK_RESIZE_CHUNK;
}

void stack_free ( TStack* s )
{
  s->top = STACK_EMPTY;
  s->stack_size = 0;
  free(s->arr);
  s->arr = NULL;
}

bool stack_empty ( const TStack* s )
{
  return ((int)s->top == STACK_EMPTY);
}

size_t stack_size ( const TStack* s )
{
  return s->stack_size;
}

void stack_pop ( TStack* s )
{
  s->top--;
}
