#include "../include/predict.h"
#include <bitset>
#include <iostream>
#include <stdio.h>

#define DEBUG 0

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

void predictor::train_predictor(uint32_t pc, char letter_outcome) {

  uint outcome;
  uint prediction;
  uint pcbits;
  uint REGbits;
  uint index;

#if DEBUG
  cout << "GHREG was " << bitset<2>(GHreg) << endl;
  cout << "doing pc=" << hex << pc << dec << " outcome=" << letter_outcome
       << endl;
#endif

  if (letter_outcome == 't') {
    outcome = 1;
  } else {
    outcome = 0;
  }

  pcbits = (pc >> 2) & this->PCbits; // given last two bits are to be ignored
  REGbits = GHreg & GHbits;
  index = ((REGbits << (PCsize - Gsize)) ^ pcbits) & PCbits;

#if DEBUG
  cout << "pcbits=" << bitset<4>(pcbits) << endl;
  cout << "REGbits=" << bitset<2>(REGbits) << endl;
  cout << "index=" << bitset<4>(index) << endl;
#endif

  prediction = make_prediction(index);

#if DEBUG
  cout << "prediction was " << prediction_table[index] << endl;
  cout << "prediction made is" << prediction << " outcome=" << outcome << endl;
#endif

  if (prediction != outcome) {
    this->mispreds++;
  }

  if (letter_outcome == 't') {

#if DEBUG
    cout << "outcome was taken" << endl;
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
#endif
    if (prediction_table[index] != SN) {
      prediction_table[index]--;
    }

#if DEBUG
    cout << "prediction is " << prediction_table[index] << endl;
#endif
  }
  GHreg = (GHreg >> 1) | (outcome << (Gsize - 1));
#if DEBUG
  cout << "GHREG is " << bitset<2>(GHreg) << endl;
  cout << "\n\n\n" << endl;
#endif
  return;
}

void predictor::print() {
  for (int i = 0; i < prediction_table.size(); i++) {
    cout << i << " ";
    cout << prediction_table[i] << endl;
  }
}
