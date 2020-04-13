/*
 * Architektury výpocetních systému (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xkocic01
 */

#include <algorithm>
#include <cstdlib>
#include "neuron.h"

float evalNeuron(
  size_t inputSize,
  const float* input,
  const float* weights,
  float bias
)
{
  float result{ 0.0f };
  using std::max;

  #pragma omp simd reduction(+:result)
  for( size_t idx = 0; idx < inputSize; idx++ )
  {
    result += input[ idx ] * weights[ idx ];
  }

  return max( 0.0f, result + bias );
}