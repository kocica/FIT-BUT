/**
 * @file stack.h
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

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "token.h"

typedef struct
{
  int test;
} TStackItem;

typedef struct TStack
{
  size_t         top;
  size_t         stack_size;
  TStackItem     *arr;
} TStack;

void stack_init ( TStack* s );
void stack_free ( TStack* s );
bool stack_empty ( const TStack* s );
size_t stack_size ( const TStack* s );
void stack_top ( const TStack* s, char* c );
void stack_pop ( TStack* s );
void stack_push ( TStack* s, char c );

#endif /* ! STACK_H */
