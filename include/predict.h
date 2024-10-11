#pragma once
#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>

using namespace std;

enum BR_RES {
  NT = 0, // NOT TAKEN
  T = 1   // TAKEN
};

enum table_counters {
  SN = 0, // predict NT, strong not taken
  WN = 1, // predict NT, weak not taken
  WT = 2, // predict T, weak taken
  ST = 3  // predict T, strong taken
};

#define STATIC 0
#define GSHARE 1
#define TOURNAMENT 2
#define CUSTOM 3
extern const char *bpName[];

extern int verbose;

class predictor {
private:
  uint GHreg;
  uint GHbits; // will be used as the mask to extract needed GHreg
  uint PCbits; // will be used as the mask to extract needed PCbits
  uint Tbits;  // bits for table, its gonna be two
  uint Gsize;  // GH bits to take
  uint PCsize; // PC bits to take
  vector<table_counters> prediction_table;
  uint masker(uint size);

public:
  predictor(uint n, uint m);
  ~predictor();
  uint8_t make_prediction(uint32_t pc);
  void train_predictor(uint32_t pc, uint8_t outcome);
};

#endif
