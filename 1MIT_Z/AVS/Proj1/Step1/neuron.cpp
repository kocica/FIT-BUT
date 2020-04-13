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
  size_t neuronCount,
  const float* input,
  const float* weights,
  float bias,
  size_t neuronId
)
{
  float result{ 0.0f };
  using std::max;

  for( size_t idx = 0; idx < inputSize; idx++ )
  {
    result += input[ idx ] * weights[ ( idx * neuronCount ) + neuronId ];
  }

  return max( 0.0f, result + bias );
}