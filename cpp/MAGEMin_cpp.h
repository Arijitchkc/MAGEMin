#include <cmath>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <mpi.h>
#include <algorithm>
#ifdef __cplusplus
extern "C" {
#endif

#include "MAGEMin.h"
#include "PGE_function.h"
#include "dump_function.h"
#include "initialize.h"
#include "io_function.h"
#include "phase_update_function.h"
#include "pp_min_function.h"
#include "simplex_levelling.h"
#include "ss_min_function.h"
#include "toolkit.h"
#include "all_endmembers.h"
#ifdef __cplusplus
}
#endif

struct stableAssemblage {
  int len_oxides = 0; // length of oxides

  std::vector<std::string> oxideNames;
  // Normalized solver input that produced this assemblage.
  std::vector<double> bulk_composition;

  double meltFraction = 0.0; // Amount of melt
  std::vector<std::string> mineral_names;

  std::vector<double> mineral_proportions;
  std::vector<std::vector<double>> mineral_oxides;
  std::vector<double> stability_of_solution;


  // Bulk system mProperties
  double bulk_density = 0.0;            // rho [kg/m3]
  double bulk_entropy = 0.0;            // Molar entropy on MAGEMin's bulk basis [kJ/(mol K)]
  double bulk_enthalpy = 0.0;           // Molar enthalpy on MAGEMin's bulk basis [kJ/mol]
  double bulk_specific_enthalpy = 0.0;  // Specific enthalpy [J/kg]
  double bulk_cp = 0.0;                 // Specific heat [J/(kg K)]

  // MAGEMin returns both redox diagnostics in base-10 logarithmic form.
  double fO2 = std::numeric_limits<double>::quiet_NaN();
  double dQFM = std::numeric_limits<double>::quiet_NaN();

  // Individual Phase system propoerties
  std::vector<double> fixed_density;   // [kg/m3]
  std::vector<double> fixed_entropy;   // Molar entropy on MAGEMin's phase basis [kJ/(mol K)]
  std::vector<double> fixed_enthalpy;  // Molar enthalpy on MAGEMin's phase basis [kJ/mol]
  std::vector<double> fixed_cp;        // Specific heat [J/(kg K)]
  std::vector<double> fixed_volume_molar; // [cm3/mol]

};

class MAGEMin_wrapper {
public:
  bool initialized=false;
  ~MAGEMin_wrapper() {
    if(initialized) {
      FreeDatabases(gv, DB, z_b);
      destroy_simplex_A(&simplex);
      destroy_simplex_B(&simplex);
      initialized=false;
    }
  }
  bulk_info z_b;
  Databases DB;
  global_variable gv;
  simplex_data simplex;
  int len_oxides;
  bool useNativeMageminNames = true;

  void initializeMAGEMin(char *database, stableAssemblage *sAssemblage);

  std::vector<std::string> get_oxide_names(const std::string& database);
  std::vector<std::string> get_mineral_names(const std::string& database);
  std::size_t get_n_oxides(const std::string& database);
  void setIncludedPhases(const std::vector<std::string> &phaseNames);
  void setExcludedPhases(const std::vector<std::string> &phaseNames);
  void executeMAGEMin(int argc, char **argv,
                      const std::vector<double> &Temperature,
                      const std::vector<double> &Pressure, int len_oxides,
                      char *database,
                      const std::vector<std::vector<double>> &bulkComposition,
                      stableAssemblage *sAssemblage);

  void executeMAGEMin(int argc, char **argv,
                      const std::vector<double> &Temperature,
                      const std::vector<double> &Pressure, int len_oxides,
                      char *database,
                      const std::vector<std::vector<double>> &bulkComposition,
                      std::vector<stableAssemblage> &stableAssemblages);

  void get_output(global_variable gv, bulk_info z_b, PP_ref *PP_ref_db,
                  SS_ref *SS_ref_db, csd_phase_set *cp, stb_system *sp,
                  stableAssemblage *sAssemblage, int point_index);

  void applyPhaseSelection();

private:
  void initializeStableAssemblage(char *database,
                                  stableAssemblage *sAssemblage);
  std::unordered_map<std::string, int> phase_map;
  std::vector<std::string> oxideNames;
  std::vector<std::string> mineralNames;
  std::vector<std::string> includedPhases;
  std::vector<std::string> excludedPhases;
};
