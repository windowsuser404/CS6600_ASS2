#include "../include/auxilary.h"
#include "../include/predict.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments is provided
  if (argc < 3 || argc > 5) {
    std::cerr << "Invalid number of arguments. Usage:\n";
    std::cerr << "./bpsim <predictor> <M> [N] <trace_file>\n";
    return 1;
  }

  std::string given_predictor = argv[1];

  if (given_predictor == "bimodal") {
    if (argc != 4) {
      std::cerr << "Usage for bimodal: ./bpsim bimodal <M> <trace_file>\n";
      return 1;
    }

    // Parse M and trace_file
    uint M = std::atoi(argv[2]);
    std::string trace_file = argv[3];

    // Output parsed values
    predictor my_pred = predictor(M, 0);
    accesspattern accesses;
    parse_file(accesses, trace_file);
    simulate(accesses, my_pred);

  } else if (given_predictor == "gshare") {
    if (argc != 5) {
      std::cerr << "Usage for gshare: ./bpsim gshare <M> <N> <trace_file>\n";
      return 1;
    }

    // Parse M, N and trace_file
    uint M = std::atoi(argv[2]);
    uint N = std::atoi(argv[3]);
    std::string trace_file = argv[4];

    // Output parsed values
    std::cout << "Predictor: " << given_predictor << std::endl;
    std::cout << "M: " << M << std::endl;
    std::cout << "N: " << N << std::endl;
    std::cout << "Trace file: " << trace_file << std::endl;

    predictor my_pred = predictor(M, N);
    accesspattern accesses;
    parse_file(accesses, trace_file);
    simulate(accesses, my_pred);

  } else {
    std::cerr << "Unknown predictor type. Use either 'bimodal' or 'gshare'.\n";
    return 1;
  }

  return 0;
}
