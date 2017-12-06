#include <stdio.h>
#include <stdbool.h> // bool parameter v inline fci
#include "bit_array.h"
#include "error.h"

#define N 10

int main(void)
{
  ba_create(p, N);

  for (int i = 0; i < N; i++)
  {
    printf("%d: %lu\n", i, ba_get_bit(p, i));
  }

	return 0;
}

