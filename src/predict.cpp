#include "../include/predict.h"
#include <stdio.h>

const char *bpName[4] = {"Static", "Gshare", "Tournament", "Custom"};

#define TABLE_COUNTER_BITS 2

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

uint32_t ghist;
uint32_t gmask;
uint32_t lmask;
uint32_t pcmask;

// gshare and custom
uint32_t *gs_pht;

// tournament and custom
uint32_t *local_bht;
uint32_t *local_pht;
uint32_t *global_pht;
uint32_t *choice_pht;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

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
  table_counters temp;
  GHbits = masker(n);
  Tbits = masker(TABLE_COUNTER_BITS);
  PCbits = masker(m);
  GHreg = 0;
  for (int i = 0; i < (1 << m); i++) {
    temp = WT;
    prediction_table.push_back(temp);
  }
}

uint8_t predictor::make_prediction(uint32_t pc) {
  uint prediction;

  // gshare and custom
  uint pcbits;
  uint REGbits;
  uint index;

  // Make a prediction based on the bpType

  pcbits = (pc >> 2) & PCbits; // given last two bits are to be ignored
  REGbits = GHreg & GHbits;
  index = (REGbits << (PCsize - Gsize)) ^ pcbits;
  prediction = prediction_table[index];
  if (prediction > WN)
    return T;
  else
    return NT;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void predictor::train_predictor(uint32_t pc, uint8_t outcome) {
  //
  // TODO: Implement Predictor training
  //

  // gshare and custom
  uint32_t pcbits;
  uint32_t histbits;
  uint32_t index;
  uint8_t prediction = make_prediction(pc);

  // tournament and custom
  uint32_t ghistbits;
  uint32_t choice;
  uint32_t pcidx;
  uint32_t lhist;
  uint32_t lpred;
  uint32_t gpred;

  pcbits = pc & gmask;
  histbits = ghist & gmask;
  index = histbits ^ pcbits;
  if (outcome == T) {
    if (gs_pht[index] < 3)
      gs_pht[index]++;
  } else {
    if (gs_pht[index] > 0)
      gs_pht[index]--;
  }
  ghist = ghist << 1 | outcome;
  return;
}
