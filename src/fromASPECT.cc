#include "MAGEMin_cpp.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

int main(int argc, char *argv[]) {

  // Define variable which would ultimately come fro ASPECT
  //

  double Temperature = 1200 + 273; // in Kelvin
  double Pressure = 32;            // in kbar
  int len_oxides = 11;
  char *database = "ig";
  std::vector<double> bulkComposition{38.494, 1.776, 2.824, 50.566, 5.886, 0.01,
                                      0.25,   0.1,   0.096, 0.109,  0.0};

  MAGEMin_wrapper wrap;
  wrap.executeMAGEMin(argc, argv, Temperature, Pressure, len_oxides, database,
                      bulkComposition);
  return 0;
}
