#include "../include/predict.h"
#include <stdio.h>
#define TABLE_COUNTER_BITS 2
uint predictor::masker(uint size) {
  uint mask = 0;
  for (int i = 0; i < size; i++) {
    mask = mask | 1 << i;
  }
  return mask;
}
// Initialize the predictor
//
predictor::predictor(uint m, uint n) : Gsize(m), PCsize(n) {
  GHbits = masker(n);
  Tbits = masker(TABLE_COUNTER_BITS);
  PCbits = masker(m);
  GHreg = 0;
  branches = 0;
  mispreds = 0;
  for (int i = 0; i < (1 << m); i++) {
    prediction_table.push_back(WT);
  }
}

uint8_t predictor::make_prediction(uint index) {
  uint prediction;
  prediction = prediction_table[index];
  if (prediction > WN)
    return T;
  else
    return NT;
}

void predictor::train_predictor(uint32_t pc, uint8_t outcome) {
  uint prediction;
  uint pcbits;
  uint REGbits;
  uint index;

  pcbits = (pc >> 2) & PCbits; // given last two bits are to be ignored
  REGbits = GHreg & GHbits;
  index = (REGbits << (PCsize - Gsize)) ^ pcbits;

  prediction = make_prediction(index);

  if (outcome == T) {
    if (prediction_table[index] != ST) {
      prediction_table[index]++;
    }
  } else {
    if (prediction_table[index] != WT) {
      prediction_table[index]--;
    }
  }
  GHreg = GHreg << 1 | outcome;
  return;
}
