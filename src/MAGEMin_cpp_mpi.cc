#include "MAGEMin_cpp.h"
#include <stdio.h>

// void MAGEMin_wrapper::getValuesFromASPECT_runs(
//     global_variable *gv, bulk_info *z_b, double Temperature, double Pressure,
//     int len_oxides, char *database, std::vector<double> bulkComposition) {
//
//   gv->verbose = -1;
//   gv->limitCaOpx = 0;
//   gv->CaOpxLim = 1.0;
//   gv->fluidSpec = 0;
//   gv->mbCpx = 0; /** 0: omphacite LT, 1: augite HT
//                   */
//   gv->mbIlm = 0; /** 0: Ilmm, 1: Ilm
//                   */
//   gv->mpSp = 0;  /** 0: Sp LT, 1: Mt1
//                   */
//   gv->mpIlm = 0; /** 0: Ilmm, 1: Ilm
//                   */
//   gv->solver = 2;
//   gv->output_matlab = 0;
//   gv->n_points = 1;
//   gv->test = -1;
//   gv->buffer_n = 0.0;
// }

std::vector<std::string> MAGEMin_wrapper::get_oxide_names(const std::string& database){
  // if (useNativeMageminNames) {
  //   std::vector<std::string> out;
  //   out.reserve(gv.len_ox);
  //   for (int i = 0; i < gv.len_ox; ++i) {
  //     out.emplace_back(gv.ox[i]);
  //   }
  //   return out;
  // }
  
  if(useNativeMageminNames){
      std::cerr << "[diag] get_oxide_names: native=true, db_arg=\"" << database
                << "\", gv.len_ox=" << gv.len_ox << "\n";
      std::vector<std::string> out;
      out.reserve(gv.len_ox);
      for (int i = 0; i < gv.len_ox; ++i) {
        std::cerr << "  ox[" << i << "] = " << gv.ox[i] << "\n";
        out.emplace_back(gv.ox[i]);
      }
      return out;
  }
  // if (database == "ig") {
  //     return {"SiO2", "Al2O3", "CaO", "MgO", "FeOt",
  //             "K2O", "Na2O", "TiO2", "O", "Cr2O3", "H2O"};
  // }
  // else if (database == "mp") {
  //     return {"SiO2", "Al2O3", "CaO", "MgO", "FeOt",
  //             "K2O", "Na2O", "TiO2", "O", "MnO", "H2O"};
  // }
  // else if (database == "mb") {
  //     return {"SiO2", "Al2O3", "CaO", "MgO", "FeOt",
  //             "K2O", "Na2O", "TiO2", "O", "H2O"};
  // }
  // else if (database == "um") {
  //     return {"SiO2", "Al2O3", "MgO", "FeOt", "O", "H2O", "S"};
  // }
  // else if (database == "ume") {
  //     return {"SiO2", "Al2O3", "MgO", "FeOt", "O", "H2O", "S", "CaO", "Na2O","Cr2O3", "CO2"};
  // }
  // else if (database == "mtl") {
  //     return {"SiO2", "Al2O3", "CaO", "MgO", "FeOt", "Na2O"};
  // }
  // else {
  //     throw std::invalid_argument("Unknown MAGEMin database: " + database);
  // }
}


std::vector<std::string> MAGEMin_wrapper::get_mineral_names(const std::string& database){
  if (useNativeMageminNames) {
    std::vector<std::string> out;
    out.reserve(gv.len_ss + gv.len_pp);
    for (int i = 0; i < gv.len_ss; ++i) {
      out.emplace_back(gv.SS_list[i]);
    }
    for (int i = 0; i < gv.len_pp; ++i) {
      out.emplace_back(gv.PP_list[i]);
    }
    return out;
  }
if (database == "ig") {

    return {"liq" ,"q",   "crst", "trd", "coe", "stv","ky", "sill",
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm", "mu",   "ol",  "fl",  "fsp","pl4T"};
  }
  else if (database== "mp") {
    return {"q",   "crst", "trd", "coe", "stv","ky", "sill",
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm",
                                  "liq", "mu", "sa","st","mt","chl","ctd","ma", "ol",  "fl",  "fsp","pl4T"};

  }
  else if(database == "mb"){
    return {"q",   "crst", "trd", "coe", "stv","ky", "sill",
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm",
                                  "liq", "mu", "sa","st","mt","chl","ctd","ma", "ol",  "fl",  "fsp","pl4T", "omph"};

  }
  else if(database == "um"){
     return {"q",   "crst", "trd", "coe", "stv","ky", "sill","pyr",
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm",
                                  "liq", "mu", "sa","st","mt","chl","ctd","ma", "ol",  "fl",  "fsp","pl4T", "omph","fluid","br","anth","po","anth","t",""};

  }
  else if(database == "ume"){
    return {"q",   "crst", "trd", "coe", "stv","ky", "sill","pyr",
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm",
                                  "liq", "mu", "sa","st","mt","chl","ctd","ma", "ol",  "fl",  "fsp","pl4T", "omph","fluid","br","anth","po","anth","t",""};


  }
  else if(database == "mtl"){
    return {"q",   "crst", "trd", "coe", "stv","ky", "sill","pyr","neph","capv","co"
                                  "and", "ru",   "sph", "spl", "bi",  "cd",
                                  "cpx", "opx",  "ep",  "g",   "amp", "ilm",
                                  "liq", "mu", "sa","st","mt","chl","ctd","ma", "ol",  "fl",  "fsp","pl4T", "omph","fluid","br","anth","po","anth","t",""};
  }
  else {
      throw std::invalid_argument("Unknown MAGEMin database: " + database);
        }
  

}
std::size_t MAGEMin_wrapper::get_n_oxides(const std::string& database) {
        return get_oxide_names(database).size();
    }

void MAGEMin_wrapper::initializeMAGEMin(char *db_name,
                                        stableAssemblage *sAssemblage) {
  initialized=true;
  gv = global_variable_alloc(gv, &z_b);
  // gv.db = db_name;
  strcpy(gv.db, db_name);
  strcpy(gv.File, "none");
  strcpy(gv.sys_in, "wt");
  strcpy(gv.buffer, "none");
  // gv.File = "none";

  // Initialize gv parameters
  std::cout<<"Value of db before setiing up database: "<<gv.db<<"    \n\n";
  gv = SetupDatabase(gv, &z_b);
  gv = global_variable_init(gv, &z_b);
  gv.verbose = 0;
  gv.limitCaOpx = 0;
  gv.CaOpxLim = 1.0;
  gv.fluidSpec = 0;
  gv.mbCpx = 0;
  gv.mbIlm = 0;
  gv.mpSp = 0;
  gv.mpIlm = 0;
  gv.output_matlab = 0;
  gv.n_points = 1;
  gv.test = -1;
  gv.buffer_n = 0;
  //
  gv.solver=2;
  // gv.br_max_tol = 1e-5;
  // gv.max_n_phase = 0.05;
  // gv.merge_value = 2e-1;
  // gv.inner_PGE_ite = 2;2;

  DB = InitializeDatabases(gv, gv.EM_database, DB);

  init_simplex_A(&simplex, gv);
  init_simplex_B_em(&simplex, gv);

  // initialize mineral Names in outpuot
    sAssemblage->oxideNames    = get_oxide_names(db_name);
    sAssemblage->mineral_names = get_mineral_names(db_name);
  // Build the initialization data structures
  this->phase_map.clear();
  for (size_t i = 0; i < sAssemblage->mineral_names.size(); ++i) {
      this->phase_map[sAssemblage->mineral_names[i]] = i;
  }
  // Allocate data structures
  sAssemblage->mineral_proportions.assign(sAssemblage->mineral_names.size(), 0.0);
  sAssemblage->mineral_oxides.assign(sAssemblage->mineral_names.size(), 
                                     std::vector<double>(gv.len_ox, 0.0));

  // Re_size bulk properties arrays
  size_t N = sAssemblage->mineral_names.size();
  sAssemblage->fixed_density.assign(N, 0.0);
  sAssemblage->fixed_entropy.assign(N, 0.0);
  sAssemblage->fixed_enthalpy.assign(N, 0.0);
  sAssemblage->fixed_cp.assign(N, 0.0);
  sAssemblage->fixed_volume_molar.assign(N,0.0);
  // std::cout << "length of names: " << sAssemblage->mineral_names.size()
  //           << " inside initialize \n";
}








void MAGEMin_wrapper::get_output(global_variable gv, bulk_info z_b,
                                 PP_ref *PP_ref_db, SS_ref *SS_ref_db,
                                 csd_phase_set *cp, stb_system *sp,
                                 stableAssemblage *sAssemblage,
                                 int point_index) {
  sAssemblage->len_oxides = gv.len_ox;
  
  // Initialize everything to zero
  std::fill(sAssemblage->mineral_proportions.begin(), sAssemblage->mineral_proportions.end(), 0.0);
  std::fill(sAssemblage->fixed_density.begin(), sAssemblage->fixed_density.end(), 0.0);
  std::fill(sAssemblage->fixed_entropy.begin(), sAssemblage->fixed_entropy.end(), 0.0);
  std::fill(sAssemblage->fixed_enthalpy.begin(), sAssemblage->fixed_enthalpy.end(), 0.0);
  std::fill(sAssemblage->fixed_cp.begin(), sAssemblage->fixed_cp.end(), 0.0);
  std::fill(sAssemblage->fixed_volume_molar.begin(), sAssemblage->fixed_volume_molar.end(), 0.0);
  for(auto& row : sAssemblage->mineral_oxides) {
         std::fill(row.begin(), row.end(), 0.0);
  }


  // Assign bulk properties
  sAssemblage->bulk_density   = gv.system_density;
  sAssemblage->bulk_entropy   = gv.system_entropy;
  sAssemblage->bulk_enthalpy  = gv.system_enthalpy;
  sAssemblage->bulk_cp        = gv.system_cp;
  sAssemblage->meltFraction   = sp[0].frac_M_wt;




  // Mass-weighted merge of a phase instance into a fixed slot.
  // If the slot is empty, assign; otherwise blend so that bulk-system
  // totals (mass, volume, S, H, Cp) are preserved when MAGEMin returns
  // two instances of the same phase (solvus, e.g. 2 opx, 2 fsp).
  auto merge_into_slot = [&](int idx, double f_new,
                             const double *Comp_wt_new,
                             double rho_new, double S_new,
                             double H_new, double cp_new, double V_new) {
    double f_old = sAssemblage->mineral_proportions[idx];
    if (f_old <= 0.0) {
      sAssemblage->mineral_proportions[idx] = f_new;
      for (int j = 0; j < gv.len_ox; j++) {
        sAssemblage->mineral_oxides[idx][j] = Comp_wt_new[j];
      }
      sAssemblage->fixed_density[idx]      = rho_new;
      sAssemblage->fixed_entropy[idx]      = S_new;
      sAssemblage->fixed_enthalpy[idx]     = H_new;
      sAssemblage->fixed_cp[idx]           = cp_new;
      sAssemblage->fixed_volume_molar[idx] = V_new;
    } else {
      std::cout<<"Is this hapennigg\n\n\n\n\n\n\n";
      double f_tot = f_old + f_new;
      double w_old = f_old / f_tot;
      double w_new = f_new / f_tot;

      for (int j = 0; j < gv.len_ox; j++) {
        sAssemblage->mineral_oxides[idx][j] =
            w_old * sAssemblage->mineral_oxides[idx][j] + w_new * Comp_wt_new[j];
      }
      // density inverse-weighted so total volume is conserved
      double rho_old = sAssemblage->fixed_density[idx];
      sAssemblage->fixed_density[idx]      = 1.0 / (w_old / rho_old + w_new / rho_new);
      sAssemblage->fixed_entropy[idx]      = w_old * sAssemblage->fixed_entropy[idx]      + w_new * S_new;
      sAssemblage->fixed_enthalpy[idx]     = w_old * sAssemblage->fixed_enthalpy[idx]     + w_new * H_new;
      sAssemblage->fixed_cp[idx]           = w_old * sAssemblage->fixed_cp[idx]           + w_new * cp_new;
      sAssemblage->fixed_volume_molar[idx] = w_old * sAssemblage->fixed_volume_molar[idx] + w_new * V_new;
      sAssemblage->mineral_proportions[idx] = f_tot;
    }
  };

  // Solution phases: sp[0].ph[0 .. n_SS-1] line up with sp[0].SS[0 .. n_SS-1]
  for (int m = 0; m < sp[0].n_SS; m++) {
    std::string phase = sp[0].ph[m];
    auto it = this->phase_map.find(phase);
    if (it == this->phase_map.end()) continue;
    int idx = it->second;

    merge_into_slot(idx,
                    sp[0].ph_frac_wt[m],
                    sp[0].SS[m].Comp_wt,
                    sp[0].SS[m].rho,
                    sp[0].SS[m].entropy,
                    sp[0].SS[m].enthalpy,
                    sp[0].SS[m].cp,
                    sp[0].SS[m].V);
  }

  // Pure phases: sp[0].ph[n_SS .. n_SS+n_PP-1] line up with sp[0].PP[0 .. n_PP-1]
  for (int m = 0; m < sp[0].n_PP; m++) {
    int n = sp[0].n_SS + m;
    std::string phase = sp[0].ph[n];
    auto it = this->phase_map.find(phase);
    if (it == this->phase_map.end()) continue;
    int idx = it->second;

    merge_into_slot(idx,
                    sp[0].ph_frac_wt[n],
                    sp[0].PP[m].Comp_wt,
                    sp[0].PP[m].rho,
                    sp[0].PP[m].entropy,
                    sp[0].PP[m].enthalpy,
                    sp[0].PP[m].cp,
                    sp[0].PP[m].V);
  }
  
  




  // // Assign mineral oxides to our data structure:: SS 
  // std::vector<std::string> presentPhases;
  // for (int m = 0; m < gv.n_cp_phase; m++) {
  //   std::string phase = sp[0].ph[m];
  //
  //   // Findex of mineral
  //   auto it = this->phase_map.find(phase);
  //   // If mineral is present.... insert it
  //   if (it != this->phase_map.end()) {
  //       int idx = it->second;
  //
  //       // Direct Copy to Fixed Array
  //       for (int j = 0; j < gv.len_ox; j++) {
  //           sAssemblage->mineral_oxides[idx][j] = sp[0].SS[m].Comp_wt[j];
  //       }
  //   }
  //   // // oxide proportions
  //   // std::vector<double> oxProp(gv.len_ox);
  //   // for (int j = 0; j < gv.len_ox; j++) {
  //   //   oxProp[j] = sp[0].SS[m].Comp_wt[j];
  //   // }
  //   // sAssemblage->mineral_oxides[phase].push_back(oxProp);
  // }
  //
  //
  // // Assign mineral oxides to our data structure:: PP
  // int n = 0;
  // for (int m = gv.n_cp_phase; m < gv.n_phase; m++) {
  //   std::string phase = sp[0].ph[m];
  //   auto it = this->phase_map.find(phase);
  //   if (it != this->phase_map.end()) {
  //       int idx = it->second;
  //
  //       for (int j = 0; j < gv.len_ox; j++) {
  //           sAssemblage->mineral_oxides[idx][j] = sp[0].PP[n].Comp_wt[j];
  //       }
  //   }
  //   n += 1;
  //   // std::vector<double> oxProp(gv.len_ox);
  //   // for (int j = 0; j < gv.len_ox; j++) {
  //   //   oxProp[j] = sp[0].PP[n].Comp_wt[j];
  //   // }
  //   // sAssemblage->mineral_oxides[phase].push_back(oxProp);
  //   // n += 1;
  // }
  //
  //
  //
  // double G;
  // n = 0;
  // // Assigning  mineral proportions:: PP phases
  // for (int i = 0; i < gv.len_cp; i++) {
  //   if (cp[i].ss_flags[1] == 1) {
  //
  //     G = 0.0;
  //     for (int j = 0; j < gv.len_ox; j++) {
  //       G += cp[i].ss_comp[j] * gv.gam_tot[j];
  //     }
  //     std::string phase = cp[i].name;
  //     auto it = this->phase_map.find(phase);
  //     if (it != this->phase_map.end()) {
  //         int idx = it->second;
  //         // Direct Assign
  //         sAssemblage->mineral_proportions[idx] = sp[0].ph_frac_wt[n];
  //         sAssemblage->fixed_density[idx]  = cp[i].phase_density;
  //         sAssemblage->fixed_entropy[idx]  = cp[i].phase_entropy;
  //         sAssemblage->fixed_enthalpy[idx] = cp[i].phase_enthalpy;
  //         sAssemblage->fixed_cp[idx]       = cp[i].phase_cp;
  //         sAssemblage->fixed_volume_molar[idx]       = cp[i].volume*10;
  //     }
  //     n += 1;
  //
  //     // presentPhases.push_back(phase);
  //     // sAssemblage->mineral_proportions[phase].push_back(sp[0].ph_frac_wt[n]);
  //     // n += 1;
  //   }
  // }
  // // Assigning  mineral proportions:: SS phases
  // int n_ss = n;
  // for (int i = 0; i < gv.len_pp; i++) {
  //   if (gv.pp_flags[i][1] == 1) {
  //     std::string phase = gv.PP_list[i];
  //     auto it = this->phase_map.find(phase);
  //     if (it != this->phase_map.end()) {
  //         int idx = it->second;
  //         // Direct Assign
  //         sAssemblage->mineral_proportions[idx] = sp[0].ph_frac_wt[n];
  //         sAssemblage->fixed_density[idx]  = PP_ref_db[i].phase_density;
  //         sAssemblage->fixed_entropy[idx]  = PP_ref_db[i].phase_entropy;
  //         sAssemblage->fixed_enthalpy[idx] = PP_ref_db[i].phase_enthalpy;
  //         sAssemblage->fixed_cp[idx]       = PP_ref_db[i].phase_cp;
  //         sAssemblage->fixed_volume_molar[idx]       = PP_ref_db[i].volume*10;
  //     }
  //     n += 1;
  //
  //
  //
  //     // presentPhases.push_back(phase);
  //     // sAssemblage->mineral_proportions[phase].push_back(sp[0].ph_frac_wt[n]);
  //     // n += 1;
  //   }
  // }

  //
  //
  //
  // for (const std::string &name : sAssemblage->mineral_names) {
  //   if (std::count(presentPhases.begin(), presentPhases.end(), name) > 0) {
  //     // → mineral is stable: real values were already pushed
  //     continue;
  //   } else {
  //     // → mineral absent: push zeros
  //     sAssemblage->mineral_proportions[name].push_back(0.0);
  //     sAssemblage->mineral_oxides[name].push_back(
  //         std::vector<double>(gv.len_ox, 0.0));
  //   }
  // }
}

void MAGEMin_wrapper::executeMAGEMin(
    int argc, char **argv, const std::vector<double> &Temperature,
    const std::vector<double> &Pressure, int len_oxides, char *database,
    const std::vector<std::vector<double>> &bulkComposition,
    stableAssemblage *sAssemblage) {
  /**
   * This function is called from ASPECT Code and inturn calls MAGEMin
   * functions. It returns necessary data inform of data structures to ASPECT.
   */

  // Change values as per inputs from ASPECT
  // empty sAssemblage 
  // sAssemblage->len_oxides =0;
  // sAssemblage->mineral_proportions.clear();
  // sAssemblage->mineral_oxides.clear();
  if(!initialized){
    initializeMAGEMin(database, sAssemblage);
    initialized=true;
  }
  else 
  {
    std::fill(sAssemblage->mineral_proportions.begin(), 
              sAssemblage->mineral_proportions.end(), 0.0);
    for (auto& row : sAssemblage->mineral_oxides)
        std::fill(row.begin(), row.end(), 0.0);
    std::fill(sAssemblage->fixed_density.begin(), 
              sAssemblage->fixed_density.end(), 0.0);
    std::fill(sAssemblage->fixed_entropy.begin(), 
              sAssemblage->fixed_entropy.end(), 0.0);
    std::fill(sAssemblage->fixed_enthalpy.begin(), 
              sAssemblage->fixed_enthalpy.end(), 0.0);
    std::fill(sAssemblage->fixed_cp.begin(), 
              sAssemblage->fixed_cp.end(), 0.0);
  }
  // initializeMAGEMin(database, sAssemblage);
  int n_points = Temperature.size();
  io_data input_data[n_points];

  for (int p = 0; p < n_points; p++) {

    z_b.T = Temperature[p]; // this value needs to be in Kelvin
    z_b.P = Pressure[p];    // this valye needs to be in kbar
    /**
     * Chemical potential of oxides; This values if used can be drawn from
     * aspect; or otherwise kept constant Same loop also covers bulk composition
     * which needs to be changedl with values derieved from ASPECT
     */
    for (int i = 0; i < len_oxides; i++) {
      gv.arg_gamma[i] = 0.0;
      gv.arg_bulk[i] = bulkComposition[p][i];
    }

    // MAGEMin_wrapper::getValuesFromASPECT_runs(
    //     &gv, &z_b, Temperature, Pressure, len_oxides, database,
    //     bulkComposition);

    ///// RUnning MAGEMin from her
    int point_index = p;
    gv.n_points = n_points;
    gv.numPoint = point_index;
    // io_data input_data[gv.n_points];

    z_b = retrieve_bulk_PT(gv, input_data, gv.numPoint, z_b);
    gv = reset_gv(gv, z_b, DB.PP_ref_db, DB.SS_ref_db);
    z_b = reset_z_b_bulk(gv, z_b);

    // Reset simplex memory
    reset_simplex_A(&simplex, z_b, gv);
    reset_simplex_B_em(&simplex, gv);

    // reset considered phases
    reset_cp(gv, z_b, DB.cp);
    // reset pure and solution phases
    reset_SS(gv, z_b, DB.SS_ref_db);
    // reset stable phases
    reset_sp(gv, DB.sp);
    /* Perform calculation for a single point
     */
    gv = ComputeG0_point(gv.EM_database, z_b, /** bulk rock informations */
                         gv, /** global variables (e.g. Gamma) 	*/

                         DB.PP_ref_db, /** pure phase database */
                         DB.SS_ref_db);

    gv = ComputeEquilibrium_Point(
        gv.EM_database, input_data[point_index],
        z_b, /** bulk rock informations 			*/
        gv,  /** global variables (e.g. Gamma) 	*/

        &simplex, DB.PP_ref_db, /** pure phase database */
        DB.SS_ref_db,           /** solid solution database 		*/
        DB.cp);

    gv = ComputePostProcessing(
        z_b,          /** bulk rock informations 			*/
        gv,           /** global variables (e.g. Gamma) 	*/
        DB.PP_ref_db, /** pure phase database 			*/
        DB.SS_ref_db, /** solid solution database 		*/
        DB.cp);
    fill_output_struct(gv, &simplex, z_b, DB.PP_ref_db, DB.SS_ref_db, DB.cp,
                       DB.sp);
    get_output(gv, z_b, DB.PP_ref_db, DB.SS_ref_db, DB.cp, DB.sp, sAssemblage,
               point_index);

  }
  // FreeDatabases(z_b, DB, gv);
}

// int main(int argc, char **argv) {
//   MAGEMin_wrapper wrap;
//   wrap.initializeMAGEMin("ig");
//   int rank;
//   // #ifdef USE_MPI
//   //     MPI_Init(&argc, &argv);
//   // 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   // #endif
//   std::vector<double> bulkComposition{38.494, 1.776, 2.824, 50.566, 5.886,
//   0.01,
//                                       0.25,   0.1,   0.096, 0.109,  0.0};
//   stableAssemblage sAssemblage;
//   std::cout << "Hey it works!" << std::endl;
//   wrap.executeMAGEMin(argc, argv, 1400, 2, 11, "ig", bulkComposition,
//                       &sAssemblage);
//
//   // #ifdef USE_MPI
//   // 	MPI_Finalize();
//   // #endif
//   return 0;
// }
