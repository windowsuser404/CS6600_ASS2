#pragma once
#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#define NT 0 // NOT TAKEN
#define T 1  // TAKEN

#define SN 0
#define WN 1
#define WT 2
#define ST 3

class predictor {
private:
  uint GHreg;
  uint GHbits; // will be used as the mask to extract needed GHreg
  uint PCbits; // will be used as the mask to extract needed PCbits
  uint Tbits;  // bits for table, its gonna be two
  uint Gsize;  // GH bits to take
  uint PCsize; // PC bits to take
  vector<uint> prediction_table;
  uint masker(uint size);

public:
  uint branches;
  uint mispreds;
  predictor(uint n, uint m);
  ~predictor();
  uint8_t make_prediction(uint32_t pc);
  void train_predictor(uint32_t pc, uint8_t outcome);
};

#endif
