#include "MAGEMin_cpp.h"

void MAGEMin_wrapper::getValuesFromASPECT_runs(
    global_variable *gv, bulk_info *z_b, MAGEMinProps *mProperties,
    double Temperature, double Pressure, int len_oxides, char *database,
    std::vector<double> bulkComposition) {

  // // Assign already known variables
  // if (database == "ig") {
  //   len_oxides = 11;
  //   major_oxide_names = {"SiO2", "Al2O3", "CaO", "MgO",   "FeOt", "K2O",
  //                        "Na2O", "TiO2",  "O",   "Cr2O3", "H2O"};
  // } else if (database == "um") {
  //   len_oxides = 7;
  //   major_oxide_names = {"SiO2", "Al2O3", "MgO", "FeO", "O", "H2O", "S"};
  // }

  gv->verbose = 1;
  gv->db = database;
  gv->limitCaOpx = 0;
  gv->CaOpxLim = 1.0;
  gv->fluidSpec = 0;
  gv->mbCpx = 0; /** 0: omphacite LT, 1: augite HT
                  */
  gv->mbIlm = 0; /** 0: Ilmm, 1: Ilm
                  */
  gv->mpSp = 0;  /** 0: Sp LT, 1: Mt1
                  */
  gv->mpIlm = 0; /** 0: Ilmm, 1: Ilm
                  */
  gv->solver = 2;
  gv->output_matlab = 1;
  gv->n_points = 1;
  gv->test = -1;
  gv->buffer_n = 0.0;
  gv->File = "none";
  gv->buffer = "none";

  //  Main variables which I am changing
  z_b->T = Temperature; // Temperature need to be in Kelin
  z_b->P = Pressure;    // Pressure need to be in kbar

  /**
   * Chemical potential of oxides; This values if used can be drawn from aspect;
   * or otherwise kept constant Same loop also covers bulk composition which
   * needs to be changedl with values derieved from ASPECT
   */

  for (int i = 0; i < len_oxides; i++) {
    gv->arg_gamma[i] = 0.0;
    gv->arg_bulk[i] = bulkComposition[i];
  }

  gv->sys_in = "mol";

  // return gv;
}

void MAGEMin_wrapper::executeMAGEMin(int argc, char **argv, double Temperature,
                                     double Pressure, int len_oxides,
                                     char *database,
                                     std::vector<double> bulkComposition) {
  /**
   * First Objective: Run MAGEMin from this C++ script
   *      1. Do away with ReadCommandLineOptions functions and pass those
   * functions yourself Second Objective: Compare results to see if anything
   * changed Third Objective: Import output function to be able to directly use
   * it in our code
   */

  /**
   * Initialize structure of variables
   * allocate global variables(Ideally would come from ASPECT)
   */
  MAGEMinProps mProperties;
  gv = global_variable_alloc(gv, &z_b);

  MAGEMin_wrapper::getValuesFromASPECT_runs(&gv, &z_b, &mProperties,
                                            Temperature, Pressure, len_oxides,
                                            database, bulkComposition);

  // Define pointers to our data structures;
  runMAGEMin(argc, argv, z_b, DB, gv, &data_in);

  std::cout << "\nNumber of oxides" << data_in.len_oxides;
  std::cout << "\n\nOxide compositions [wt fr]:\n";
  for (int i = 0; i < data_in.len_oxides; i++) {
    std::cout << " " << data_in.major_oxide_names[i] << " ";
  }
  std::cout << "\n";

  for (int m = 0; m < data_in.len_stable_phases; m++) {
    printf(" %s", data_in.stable_phases_names[m]);
    for (int j = 0; j < data_in.len_oxides; j++) {
      printf("%10.5f ", data_in.major_oxide_propertions[m][j]);
    }
    printf("\n");
  }

  double G = 0.0;
  int n = 0;
  printf("\n\n");
  printf("Stable mineral assemblage:\n");
  printf(
      "%6s%15s %13s %17s %17s %12s %12s %12s %12s %12s %12s %12s %12s %12s\n",
      "phase", "fraction[wt]", "G[J]", "V_molar[cm3/mol]", "V_partial[cm3]",
      "Cp[kJ/K]", "Rho[kg/m3]", "Alpha[1/K]", "Entropy[J/K]", "Enthalpy[J]",
      "BulkMod[GPa]", "ShearMod[GPa]", "Vp[km/s]", "Vs[km/s]");

  n = 0;
  for (int i = 0; i < data_in.len_stable_phases; i++) {

    printf("%6s", data_in.stable_phases_names[i]);
    printf("%+15.5f %+13.5f %+17.5f %+17.5f %+12.5f %+12.5f %+12.8f %+12.6f "
           "%+14.4f %+12.2f %+12.2f %+12.2f %+12.2f",
           data_in.all_properties[i][0], data_in.all_properties[i][1],
           data_in.all_properties[i][2], data_in.all_properties[i][3],
           data_in.all_properties[i][4], data_in.all_properties[i][5],
           data_in.all_properties[i][6], data_in.all_properties[i][7],
           data_in.all_properties[i][8], data_in.all_properties[i][9],
           data_in.all_properties[i][10], data_in.all_properties[i][11],
           data_in.all_properties[i][12]);
    printf("\n");
    n += 1;
  }
}

// int main(int argc, char **argv) {
//   MAGEMin_wrapper wrap;
//   int rank;
//   // #ifdef USE_MPI
//   //     MPI_Init(&argc, &argv);
//   // 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   // #endif
//
//   std::cout << "Hey it works!" << std::endl;
//   wrap.executeMAGEMin(argc, argv);
//
//   // #ifdef USE_MPI
//   // 	MPI_Finalize();
//   // #endif
//   return 0;
//
