#include "../include/auxilary.h"
#include "../include/predict.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments is provided
  if (argc < 3 || argc > 5) {
    cerr << "Invalid number of arguments. Usage:\n";
    cerr << "./bpsim <predictor> <M> [N] <trace_file>\n";
    return 1;
  }

  string given_predictor = argv[1];
  string trace_file;

  if (given_predictor == "bimodal") {
    if (argc != 4) {
      cerr << "Usage for bimodal: ./bpsim bimodal <M> <trace_file>\n";
      return 1;
    }

    // Parse M and trace_file
    uint M = atoi(argv[2]);
    trace_file = argv[3];

    // Output parsed values
    predictor my_pred = predictor(M, 0);
    accesspattern accesses;
    parse_file(accesses, trace_file);
    simulate(accesses, my_pred);

    float miss_rate =
        ((float)my_pred.mispreds) / ((float)my_pred.branches) * 100;

    cout << "COMMAND" << endl;
    cout << "./bpsim bimodal ";
    cout << M << " ";
    cout << trace_file << endl;
    cout << "OUTPUT" << endl;
    cout << "number of predictions: " << my_pred.branches << endl;
    cout << "number of mispredictions: " << my_pred.mispreds << endl;
    cout << "misprediction rate: " << fixed << setprecision(2) << miss_rate
         << "%" << endl;
    cout << "FINAL BIMODAL CONTENTS" << endl;
    my_pred.print();

  } else if (given_predictor == "gshare") {
    if (argc != 5) {
      cerr << "Usage for gshare: ./bpsim gshare <M> <N> <trace_file>\n";
      return 1;
    }

    // Parse M, N and trace_file
    uint M = atoi(argv[2]);
    uint N = atoi(argv[3]);
    trace_file = argv[4];

    predictor my_pred = predictor(M, N);
    accesspattern accesses;
    parse_file(accesses, trace_file);
    simulate(accesses, my_pred);

    float miss_rate =
        ((float)my_pred.mispreds) / ((float)my_pred.branches) * 100;

    cout << "COMMAND" << endl;
    cout << "./bpsim gshare ";
    cout << M << " " << N << " ";
    cout << trace_file << endl;
    cout << "OUTPUT" << endl;
    cout << "number of predictions: " << my_pred.branches << endl;
    cout << "number of mispredictions: " << my_pred.mispreds << endl;
    cout << "misprediction rate: " << fixed << setprecision(2) << miss_rate
         << "%" << endl;
    cout << "FINAL GSHARE CONTENTS" << endl;
    my_pred.print();

  } else {
    cerr << "Unknown predictor type. Use either 'bimodal' or 'gshare'.\n";
    return 1;
  }

  return 0;
}
