// bit_aray.h
// Řešení IJC-DU1, příklad a), 24.2.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Hlavickovy soubor s podminenym prekladem,
//				definujici bitove pole. Pri definovanem
//				symbolu USE_INLINE, se operace provadeji
//				pomoci inline funkci(tam kde to jde)
//        v opacnem pripade se pouziji funkci makra.
//        V pripade chyby se vola funkce error_msg
//				s prislusnou chybovou hlaskou a ukonci
//				program pomoci funkce exit.

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <limits.h>
#include "error.h"

typedef unsigned long *bit_array_t;

#define UL_BIT (sizeof(unsigned long) * (CHAR_BIT))

#define BA_GET_BIT_(p,i) (((p[((i)+UL_BIT)/UL_BIT]) >> (((i)+UL_BIT)%UL_BIT)) & 1)

#define BA_SET_BIT_(p,i,b) (b) \
 ? ((p[((i)+UL_BIT)/UL_BIT]) |= (1LU << (((i)+UL_BIT)%UL_BIT))) \
 : ((p[((i)+UL_BIT)/UL_BIT]) &= ~(1LU << (((i)+UL_BIT)%UL_BIT)))

#define ba_create(p,size) \
    unsigned long p[((size)/UL_BIT)+2] = { (size), 0, };


#ifndef USE_INLINE

#define ba_size(p) (((p[0])/UL_BIT)+2)

#define ba_set_bit(p,i,b) \
 (((ba_size(p)-1) * UL_BIT) <= (i) || ((i) < 0)) \
 ? error_msg("Index %lu out of scope 0..%lu",(i),(((ba_size(p)-1)*UL_BIT)-1)) \
 : BA_SET_BIT_(p,i,b)

#define ba_get_bit(p,i) \
 (((ba_size(p)-1) * UL_BIT) <= (i) || ((i) < 0)) \
 ? error_msg("Index %lu out of scope 0..%lu",(i),(((ba_size(p)-1)*UL_BIT)-1)) \
 : BA_GET_BIT_(p,i)

#else

//############################# USE_INLINE #############################

inline unsigned long ba_size(bit_array_t p)
{
  return ((p[0]/UL_BIT)+2);
}

inline void ba_set_bit(bit_array_t p, int i, bool b)
{
  unsigned long array_size = ba_size(p)-1;

  ((array_size*UL_BIT) <= i || i < 0)
  ? error_msg("Index %lu out of scope 0..%lu",i, (array_size*UL_BIT) - 1)
  : BA_SET_BIT_(p,i,b);
}

inline int ba_get_bit(bit_array_t p, int i)
{
  unsigned long array_size = ba_size(p)-1;

  if ((array_size*UL_BIT) <= i || i < 0)
    error_msg("Index %lu out of scope 0..%lu",i, (array_size*UL_BIT) - 1);
  return BA_GET_BIT_(p,i);
}

#endif /* USE_INLINE */
#endif /* ! BIT_ARRAY_H */
