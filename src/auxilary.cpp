#include "../include/auxilary.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void parse_file(accesspattern &accesses, string &traceFile) {
  accesspattern branchList;
  ifstream file(traceFile);

  if (!file.is_open()) {
    cerr << "Error opening file: " << traceFile << endl;
    exit(1);
  }

  string line;
  while (getline(file, line)) {
    istringstream iss(line);
    string addressHex;
    char takenChar;

    if (iss >> addressHex >> takenChar) {
      unsigned int address;
      stringstream ss;
      ss << hex << addressHex;
      ss >> address;

      accesses.emplace_back(address, takenChar);
    }
  }

  file.close();
}

void simulate(accesspattern &accesses, predictor &sim_pred) {
  for (int acc_count = 0; acc_count < accesses.size(); acc_count++) {
    char operation = accesses[acc_count].second;
    uint pc = accesses[acc_count].first;
    cout << "took " << operation << " in pc " << pc << endl;
  }
}
