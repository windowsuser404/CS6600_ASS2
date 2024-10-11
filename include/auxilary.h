#pragma once
#ifndef AUX_H
#define AUX_H

#include "predict.h"
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

typedef vector<pair<uint, char>> accesspattern;

void parse_file(accesspattern &accesses, string &file);

void simulate(accesspattern &accesses, predictor &sim_pred);

#endif
