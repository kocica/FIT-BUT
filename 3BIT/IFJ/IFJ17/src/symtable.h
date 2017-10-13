/**
 * @file symtable.h
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

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "error.h"
#include "types.h"

/**
 * @brief Represents one hash table item
 */
typedef struct
{
  bool declared;
  bool used;
  bool dead;
  VAR_TYPE type;
  char * name;     /**< Hashed key. */
  size_t name_len;
} htab_item;

/**
 * @brief Represents whole hash table
 */
typedef struct
{
  size_t size;      /**< Size of hash table. */
  htab_item htab[]; /**< Hash table represented as flexible array member. */
} htab;

/**
 * @brief Returns size of hash table
 *
 * @param t [in] Pointer to hash table
 *
 * @return Size of hash table
 */
size_t htab_size(htab *t);

/**
 * @brief Tells us whether hash table is empty
 *
 * @param t [in] Pointer to hash table
 *
 * @return true if empty, else false
 */
bool htab_empty(htab *t);

/**
 * @brief Adds new hast to hast table
 *
 * @param t [in] Pointer to hash table
 * @param s [in] Pointer to hash string
 */
void htab_add(htab *t, const char * s);

/**
 * @brief Removes item pointed by parameter i
 *
 * @param t [in] Pointer to hash table
 * @param s [in] Pointer to hash item to remove
 */
void htab_remove(htab *t, const char *s);

/**
 * @brief Initialize new hash table
 *
 * @param t [out] Hash table to init
 */
void htab_init(htab **t);

/**
 * @brief Hashes string to number
 *
 * @param str [in] String to hash
 *
 * @return hash number
 */
unsigned int hash_function(const char * str);

/**
 * @brief Frees hash table
 *
 * @param t [out] Pointer to hash table
 */
void htab_free(htab **t);

#endif /* ! SYMTABLE_H */
