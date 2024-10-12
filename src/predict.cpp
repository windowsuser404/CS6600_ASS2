#include "../include/predict.h"
#include <iostream>
#include <stdio.h>

#define DEBUG 1

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
predictor::predictor(uint m, uint n) : Gsize(n), PCsize(m) {
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

predictor::~predictor() {}

uint8_t predictor::make_prediction(uint index) {
  uint prediction;
  prediction = prediction_table[index];
  if (prediction > WN)
    return T;
  else
    return NT;
}

void predictor::train_predictor(uint32_t pc, uint8_t outcome) {

#if DEBUG
  cout << "doing pc=" << pc << " outcome=" << outcome << endl;
  cout << "PCsize=" << PCsize << " Gsize=" << Gsize << endl;
#endif

  uint prediction;
  uint pcbits;
  uint REGbits;
  uint index;

  pcbits = (pc >> 2) & this->PCbits; // given last two bits are to be ignored
  REGbits = GHreg & GHbits;
  index = ((REGbits << (PCsize - Gsize)) ^ pcbits) & PCbits;

#if DEBUG
  cout << "pcbits=" << pcbits << endl;
  cout << "REGbits=" << REGbits << endl;
  cout << "index=" << index << endl;
#endif

  prediction = make_prediction(index);

#if DEBUG
  cout << "prediction made is" << prediction << " outcome=" << outcome << endl;
#endif

  if (prediction != outcome) {
    this->mispreds++;
  }

  if (outcome == 't') {
#if DEBUG
    cout << "outcome was taken" << endl;
    cout << "prediction was " << prediction_table[index] << endl;
#endif

    if (prediction_table[index] != ST) {
      prediction_table[index]++;
    }

#if DEBUG
    cout << "prediction is " << prediction_table[index] << endl;
#endif

  } else {
#if DEBUG
    cout << "outcome was not taken" << endl;
    cout << "prediction was " << prediction_table[index] << endl;
#endif
    if (prediction_table[index] != SN) {
      prediction_table[index]--;
    }

#if DEBUG
    cout << "prediction is " << prediction_table[index] << endl;
#endif
  }
  GHreg = GHreg << 1 | outcome;
  return;
}

void predictor::print() {
  for (int i = 0; i < prediction_table.size(); i++) {
    cout << i << " ";
    cout << prediction_table[i] << endl;
  }
}
