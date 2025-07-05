#include "MAGEMin_cpp.h"
#include <stdio.h>

void MAGEMin_wrapper::getValuesFromASPECT_runs(
    global_variable *gv, bulk_info *z_b, double Temperature, double Pressure,
    int len_oxides, char *database, std::vector<double> bulkComposition) {

  gv->verbose = -1;
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
  gv->output_matlab = 0;
  gv->n_points = 1;
  gv->test = -1;
  gv->buffer_n = 0.0;
}

void MAGEMin_wrapper::executeMAGEMin(int argc, char **argv, double Temperature,
                                     double Pressure, int len_oxides,
                                     char *database,
                                     std::vector<double> bulkComposition,
                                     stableAssemblage *sAssemblage) {
  /**
   * This function is called from ASPECT Code and inturn calls MAGEMin
   * functions. It returns necessary data inform of data structures to ASPECT.
   */

  // stableAssemblage mProperties;
  gv = global_variable_alloc(gv, &z_b);

  // Change values as per inputs from ASPECT
  z_b.T = Temperature; // this value needs to be in Kelvin
  z_b.P = Pressure;    // this valye needs to be in kbar
  gv.db = database;
  gv.File = "none";
  gv.buffer = "none";
  /**
   * Chemical potential of oxides; This values if used can be drawn from aspect;
   * or otherwise kept constant Same loop also covers bulk composition which
   * needs to be changedl with values derieved from ASPECT
   */
  for (int i = 0; i < len_oxides; i++) {
    gv.arg_gamma[i] = 0.0;
    gv.arg_bulk[i] = bulkComposition[i];
  }

  gv.sys_in = "wt";

  MAGEMin_wrapper::getValuesFromASPECT_runs(
      &gv, &z_b, Temperature, Pressure, len_oxides, database, bulkComposition);

  // Define pointers to our data structures;
  runMAGEMin(argc, argv, z_b, DB, gv, &data_in);

  // Assign computed solution to a structure which can be passed to ASPECT
  sAssemblage->len_oxides = data_in.len_oxides;
  sAssemblage->len_stable_phases = data_in.len_stable_phases;

  for (size_t it = 0; it < sAssemblage->solutionNames.size(); ++it) {

    int indexOfStblMin = -99;

    for (int m = 0; m < data_in.len_stable_phases; m++) {
      if (sAssemblage->solutionNames[it] == data_in.stable_phases_names[m]) {
        indexOfStblMin = m;
        break;
      }
    }

    // if(sAssemblage->solutionNames[indexOfStblMin]=="liq")
    // {
    //   sAssemblage->meltFraction = data_in.all_properties[0];
    // }

    // std::cout << "\n"
    //           << sAssemblage->solutionNames[it] << "  " << indexOfStblMin
    //           << "  " << data_in.stable_phases_names[indexOfStblMin];
    //
    std::vector<double> row1{};
    std::vector<double> row2{};

    if (indexOfStblMin != -99) {
      for (int i = 0; i < data_in.len_oxides; i++) {
        row1.push_back(data_in.major_oxide_propertions[indexOfStblMin][i]);
      }
      row2 = std::vector<double>{data_in.all_properties[indexOfStblMin][0],
                                 data_in.all_properties[indexOfStblMin][1],
                                 data_in.all_properties[indexOfStblMin][2],
                                 data_in.all_properties[indexOfStblMin][3],
                                 data_in.all_properties[indexOfStblMin][4],
                                 data_in.all_properties[indexOfStblMin][5],
                                 data_in.all_properties[indexOfStblMin][6],
                                 data_in.all_properties[indexOfStblMin][7],
                                 data_in.all_properties[indexOfStblMin][8],
                                 data_in.all_properties[indexOfStblMin][9],
                                 data_in.all_properties[indexOfStblMin][10],
                                 data_in.all_properties[indexOfStblMin][11],
                                 data_in.all_properties[indexOfStblMin][12]};
      // sAssemblage->oxideCompositions.push_back(
      // std::vector<double>(&data_in.major_oxide_propertions[indexOfStblMin],
      // &data_in.major_oxide_propertions[indexOfStblMin] +
      // data_in.len_oxides));
    } else {
      row1 = std::vector<double>(data_in.len_oxides, 0.0);
      row2 = std::vector<double>(13, -999.999);
      // sAssemblage->oxideCompositions.push_back(
      // std::vector<double>(data_in.len_oxides, 0.0));
    }
    sAssemblage->oxideCompositions.push_back(row1);
    sAssemblage->stablePhasesProperties.push_back(row2);
  }

  // std::cout << "\nNumber of oxides" << data_in.len_oxides;
  // std::cout << "\n\nOxide compositions [wt fr]:\n";
  // for (int i = 0; i < data_in.len_oxides; i++) {
  //   std::cout << " " << data_in.major_oxide_names[i] << " ";
  // }
  // std::cout << "\n";
  //
  // for (int m = 0; m < data_in.len_stable_phases; m++) {
  //   printf(" %s", data_in.stable_phases_names[m]);
  //   for (int j = 0; j < data_in.len_oxides; j++) {
  //     printf("%10.5f ", data_in.major_oxide_propertions[m][j]);
  //   }
  //   printf("\n");
  // }
  //
  // double G = 0.0;
  // int n = 0;
  // printf("\n\n");
  // printf("Stable mineral assemblage:\n");
  // printf(
  //     "%6s%15s %13s %17s %17s %12s %12s %12s %12s %12s %12s %12s %12s
  //     %12s\n", "phase", "fraction[wt]", "G[J]", "V_molar[cm3/mol]",
  //     "V_partial[cm3]", "Cp[kJ/K]", "Rho[kg/m3]", "Alpha[1/K]",
  //     "Entropy[J/K]", "Enthalpy[J]", "BulkMod[GPa]", "ShearMod[GPa]",
  //     "Vp[km/s]", "Vs[km/s]");
  //
  // n = 0;
  // for (int i = 0; i < data_in.len_stable_phases; i++) {
  //
  //   printf("%6s", data_in.stable_phases_names[i]);
  //   printf("%+15.5f %+13.5f %+17.5f %+17.5f %+12.5f %+12.5f %+12.8f %+12.6f "
  //          "%+14.4f %+12.2f %+12.2f %+12.2f %+12.2f",
  //          data_in.all_properties[i][0], data_in.all_properties[i][1],
  //          data_in.all_properties[i][2], data_in.all_properties[i][3],
  //          data_in.all_properties[i][4], data_in.all_properties[i][5],
  //          data_in.all_properties[i][6], data_in.all_properties[i][7],
  //          data_in.all_properties[i][8], data_in.all_properties[i][9],
  //          data_in.all_properties[i][10], data_in.all_properties[i][11],
  //          data_in.all_properties[i][12]);
  //   printf("\n");
  //   n += 1;
  // }
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
