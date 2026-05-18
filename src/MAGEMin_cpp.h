#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <mpi.h>
#include <algorithm>
#ifdef __cplusplus
extern "C" {
#endif

#include "MAGEMin_modified.h"
#include "PGE_function.h"
#include "dump_function.h"
#include "initialize.h"
#include "io_function.h"
#include "phase_update_function.h"
#include "pp_min_function.h"
#include "simplex_levelling.h"
#include "ss_min_function.h"
#include "toolkit.h"
// #include "gem_function.h"
#include "all_endmembers.h"
#ifdef __cplusplus
}
#endif

struct stableAssemblage {
  int len_oxides; // length of oxides

  std::vector<std::string> oxideNames;

  double meltFraction; // Amount of melt
  //

  // Storing mineral_names
  std::vector<std::string> mineral_names;

  // map for their proportions values
  // std::unordered_map<std::string, std::vector<double>> mineral_proportions;
  std::vector<double> mineral_proportions;
  std::vector<std::vector<double>> mineral_oxides;
  // map for their oxide values
  // std::unordered_map<std::string, std::vector<std::vector<double>>>
  //     mineral_oxides;
  //
  std::vector<double> stability_of_solution;


  // Bulk system mProperties
  double bulk_density;   // rho [kg/m3]
  double bulk_entropy;   // S [J/K]
  double bulk_enthalpy;  // H [J]
  double bulk_cp;        // Cp [J/K]

  // Individual Phase system propoerties
  std::vector<double> fixed_density;   // [kg/m3]
  std::vector<double> fixed_entropy;   // [J/K]
  std::vector<double> fixed_enthalpy;  // [J]
  std::vector<double> fixed_cp;        // [J/K]
  std::vector<double> fixed_volume_molar;
};

class MAGEMin_wrapper {
public:
  bool initialized=false;
  ~MAGEMin_wrapper() {
    if(initialized) {
      FreeDatabases(z_b, DB, gv);
      destroy_simplex_A(&simplex);
      destroy_simplex_B(&simplex);
      initialized=false;
    }
  }
  bulk_info z_b;
  Databases DB;
  global_variable gv;
  simplex_data simplex;
  outdataCPP data_in;
  // Variables to import data
  int len_oxides;
  bool useNativeMageminNames;

  void initializeMAGEMin(char *database, stableAssemblage *sAssemblage);

  // FUnction which will call and execute MAGEMin
  // void executeMAGEMin(int argc, char **argv);
  // void executeMAGEMin(int argc, char **argv, double Temperature,
  //                     double Pressure, int len_oxides, char *database,
  //                     std::vector<double> bulkComposition,
  //                     stableAssemblage *sAssemblage);

  std::vector<std::string> get_oxide_names(const std::string& database);
  std::vector<std::string> get_mineral_names(const std::string& database);
  std::size_t get_n_oxides(const std::string& database);
  void executeMAGEMin(int argc, char **argv,
                      const std::vector<double> &Temperature,
                      const std::vector<double> &Pressure, int len_oxides,
                      char *database,
                      const std::vector<std::vector<double>> &bulkComposition,
                      stableAssemblage *sAssemblage);

  // void getValuesFromASPECT_runs(global_variable *gv, bulk_info *z_b,
  // MAGEMinProps *mProperties);

  // void getValuesFromASPECT_runs(global_variable *gv, bulk_info *z_b,
  //                               double Temperature, double Pressure,
  //                               int len_oxides, char *database,
  //                               std::vector<double> bulkComposition);
  void get_output(global_variable gv, bulk_info z_b, PP_ref *PP_ref_db,
                  SS_ref *SS_ref_db, csd_phase_set *cp, stb_system *sp,
                  stableAssemblage *sAssemblage, int point_index);

  // Bulk composition of the system we are trying to study
  // std::vector<double> bulkComposition
  // {41.49,1.57,3.824,50.56,5.88,0.01,0.25,0.10,0.1,0.0};
  // std::vector<double> bulkComposition{38.494, 1.776, 2.824, 50.566, 5.886,
  // 0.01,
  //                                     0.25,   0.1,   0.096, 0.109,  0.0};

private:
  int i_will_think;
  std::unordered_map<std::string, int> phase_map;
};

// int main(int argc, char** argv);
