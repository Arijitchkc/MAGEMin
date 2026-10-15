#include "MAGEMin_cpp.h"

#include <chrono>
#include <stdexcept>
#include <stdio.h>

std::vector<std::string> MAGEMin_wrapper::get_oxide_names(const std::string &database)
{
  if (!initialized || oxideNames.empty())
    throw std::invalid_argument("MAGEMin must be initialized before reading oxide names for database: " + database);
  return oxideNames;
}


std::vector<std::string> MAGEMin_wrapper::get_mineral_names(const std::string &database)
{
  if (!initialized || mineralNames.empty())
    throw std::invalid_argument("MAGEMin must be initialized before reading mineral names for database: " + database);
  return mineralNames;
}
std::size_t MAGEMin_wrapper::get_n_oxides(const std::string &database)
{
  return get_oxide_names(database).size();
}


void MAGEMin_wrapper::setIncludedPhases(
  const std::vector<std::string> &phaseNames)
{
  includedPhases = phaseNames;
}


void MAGEMin_wrapper::setExcludedPhases(
  const std::vector<std::string> &phaseNames)
{
  excludedPhases = phaseNames;
}


void MAGEMin_wrapper::applyPhaseSelection()
{
  if (!includedPhases.empty() && !excludedPhases.empty())
    throw std::invalid_argument("MAGEMin included and excluded phase lists cannot both be set.");

  if (!includedPhases.empty())
    {
      std::vector<bool> found(includedPhases.size(), false);
      for (int phase = 0; phase < gv.len_ss; ++phase)
        {
          const auto selected = std::find(includedPhases.begin(),
                                          includedPhases.end(),
                                          gv.SS_list[phase]);
          if (selected == includedPhases.end())
            for (int flag = 0; flag < gv.n_flags; ++flag)
              DB.SS_ref_db[phase].ss_flags[flag] = 0;
          else
            found[std::distance(includedPhases.begin(), selected)] = true;
        }

      for (int phase = 0; phase < gv.len_pp; ++phase)
        {
          const auto selected = std::find(includedPhases.begin(),
                                          includedPhases.end(),
                                          gv.PP_list[phase]);
          if (selected == includedPhases.end())
            for (int flag = 0; flag < gv.n_flags; ++flag)
              gv.pp_flags[phase][flag] = 0;
          else
            found[std::distance(includedPhases.begin(), selected)] = true;
        }

      for (std::size_t phase = 0; phase < includedPhases.size(); ++phase)
        if (!found[phase])
          throw std::invalid_argument("Unknown MAGEMin phase in inclusion list: " +
                                      includedPhases[phase]);
      return;
    }

  for (const std::string &excluded_phase : excludedPhases)
    {
      bool found = false;
      for (int phase = 0; phase < gv.len_ss; ++phase)
        if (excluded_phase == gv.SS_list[phase])
          {
            for (int flag = 0; flag < gv.n_flags; ++flag)
              DB.SS_ref_db[phase].ss_flags[flag] = 0;
            found = true;
          }

      for (int phase = 0; phase < gv.len_pp; ++phase)
        if (excluded_phase == gv.PP_list[phase])
          {
            for (int flag = 0; flag < gv.n_flags; ++flag)
              gv.pp_flags[phase][flag] = 0;
            found = true;
          }

      // In the igneous database, excluding the fluid solution must leave pure
      // H2O available, matching MAGEMin's official phase-removal behaviour.
      if (excluded_phase == "fl" && gv.EM_database == 2)
        for (int phase = 0; phase < gv.len_pp; ++phase)
          if (std::string(gv.PP_list[phase]) == "H2O")
            {
              for (int flag = 0; flag < gv.n_flags; ++flag)
                gv.pp_flags[phase][flag] = 0;
              gv.pp_flags[phase][0] = 1;
              break;
            }

      if (!found)
        throw std::invalid_argument("Unknown MAGEMin phase in exclusion list: " +
                                    excluded_phase);
    }
}

void MAGEMin_wrapper::initializeMAGEMin(char *db_name,
                                        stableAssemblage *sAssemblage)
{
  initialized=true;
  gv = global_variable_alloc(gv, &z_b);
  strcpy(gv.db, db_name);
  strcpy(gv.File, "none");
  strcpy(gv.sys_in, "wt");
  strcpy(gv.buffer, "none");
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
  gv.solver=2;

  DB = InitializeDatabases(gv, gv.EM_database, DB);

  init_simplex_A(&simplex, gv);
  init_simplex_B_em(&simplex, gv);

  oxideNames.clear();
  oxideNames.reserve(gv.len_ox);
  for (int i = 0; i < gv.len_ox; ++i)
    oxideNames.emplace_back(gv.ox[i]);

  mineralNames.clear();
  mineralNames.reserve(gv.len_ss + gv.len_pp);
  for (int i = 0; i < gv.len_ss; ++i)
    mineralNames.emplace_back(gv.SS_list[i]);
  for (int i = 0; i < gv.len_pp; ++i)
    mineralNames.emplace_back(gv.PP_list[i]);

  phase_map.clear();
  for (std::size_t i = 0; i < mineralNames.size(); ++i)
    phase_map[mineralNames[i]] = i;

  initializeStableAssemblage(db_name, sAssemblage);
}


void MAGEMin_wrapper::initializeStableAssemblage(char *db_name,
                                                 stableAssemblage *sAssemblage)
{

  (void) db_name;
  sAssemblage->oxideNames    = oxideNames;
  sAssemblage->mineral_names = mineralNames;
  sAssemblage->mineral_proportions.assign(sAssemblage->mineral_names.size(), 0.0);
  sAssemblage->mineral_oxides.assign(sAssemblage->mineral_names.size(),
                                     std::vector<double>(gv.len_ox, 0.0));

  const size_t N = sAssemblage->mineral_names.size();
  sAssemblage->fixed_density.assign(N, 0.0);
  sAssemblage->fixed_entropy.assign(N, 0.0);
  sAssemblage->fixed_enthalpy.assign(N, 0.0);
  sAssemblage->fixed_cp.assign(N, 0.0);
  sAssemblage->fixed_volume_molar.assign(N,0.0);
}








void MAGEMin_wrapper::get_output(global_variable gv, bulk_info z_b,
                                 PP_ref *PP_ref_db, SS_ref *SS_ref_db,
                                 csd_phase_set *cp, stb_system *sp,
                                 stableAssemblage *sAssemblage,
                                 int point_index)
{
  sAssemblage->len_oxides = gv.len_ox;

  // Initialize everything to zero
  std::fill(sAssemblage->mineral_proportions.begin(), sAssemblage->mineral_proportions.end(), 0.0);
  std::fill(sAssemblage->fixed_density.begin(), sAssemblage->fixed_density.end(), 0.0);
  std::fill(sAssemblage->fixed_entropy.begin(), sAssemblage->fixed_entropy.end(), 0.0);
  std::fill(sAssemblage->fixed_enthalpy.begin(), sAssemblage->fixed_enthalpy.end(), 0.0);
  std::fill(sAssemblage->fixed_cp.begin(), sAssemblage->fixed_cp.end(), 0.0);
  std::fill(sAssemblage->fixed_volume_molar.begin(), sAssemblage->fixed_volume_molar.end(), 0.0);
  for (auto &row : sAssemblage->mineral_oxides)
    {
      std::fill(row.begin(), row.end(), 0.0);
    }


  // Assign bulk properties
  sAssemblage->bulk_density   = gv.system_density;
  sAssemblage->bulk_entropy   = gv.system_entropy;
  sAssemblage->bulk_enthalpy  = gv.system_enthalpy;
  if (sp[0].M_sys <= 0.0)
    throw std::runtime_error("MAGEMin returned a non-positive bulk molar mass.");
  // MAGEMin stores H in kJ/mol on the normalized bulk basis and M_sys in
  // g/mol. Their ratio multiplied by 1e6 is therefore J/kg.
  sAssemblage->bulk_specific_enthalpy =
    gv.system_enthalpy / sp[0].M_sys * 1e6;
  // MAGEMin already converts the mass-weighted phase heat capacities to
  // J/(kg K) in s_cp. gv.system_cp is the molar system heat capacity.
  sAssemblage->bulk_cp        = sp[0].s_cp;
  // MAGEMin stores both redox diagnostics in base-10 logarithmic form.
  sAssemblage->fO2            = sp[0].fO2;
  sAssemblage->dQFM           = sp[0].dQFM;
  sAssemblage->meltFraction   = sp[0].frac_M_wt;




  // Mass-weighted merge of a phase instance into a fixed slot.
  // If the slot is empty, assign; otherwise blend so that bulk-system
  // totals (mass, volume, S, H, Cp) are preserved when MAGEMin returns
  // two instances of the same phase (solvus, e.g. 2 opx, 2 fsp).
  auto merge_into_slot = [&](int idx, double f_new,
                             const double *Comp_wt_new,
                             double rho_new, double S_new,
                             double H_new, double cp_new, double V_new)
  {
    double f_old = sAssemblage->mineral_proportions[idx];
    if (f_old <= 0.0)
      {
        sAssemblage->mineral_proportions[idx] = f_new;
        for (int j = 0; j < gv.len_ox; j++)
          {
            sAssemblage->mineral_oxides[idx][j] = Comp_wt_new[j];
          }
        sAssemblage->fixed_density[idx]      = rho_new;
        sAssemblage->fixed_entropy[idx]      = S_new;
        sAssemblage->fixed_enthalpy[idx]     = H_new;
        sAssemblage->fixed_cp[idx]           = cp_new;
        sAssemblage->fixed_volume_molar[idx] = V_new;
      }
    else
      {
        double f_tot = f_old + f_new;
        double w_old = f_old / f_tot;
        double w_new = f_new / f_tot;

        for (int j = 0; j < gv.len_ox; j++)
          {
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
  for (int m = 0; m < sp[0].n_SS; m++)
    {
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
  for (int m = 0; m < sp[0].n_PP; m++)
    {
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

}

void MAGEMin_wrapper::executeMAGEMin(
  int argc, char **argv, const std::vector<double> &Temperature,
  const std::vector<double> &Pressure, int len_oxides, char *database,
  const std::vector<std::vector<double>> &bulkComposition,
  stableAssemblage *sAssemblage)
{
  std::vector<stableAssemblage> stableAssemblages;
  executeMAGEMin(argc, argv, Temperature, Pressure, len_oxides, database,
                 bulkComposition, stableAssemblages);

  if (!stableAssemblages.empty())
    *sAssemblage = std::move(stableAssemblages.back());
}


void MAGEMin_wrapper::executeMAGEMin(
  int argc, char **argv, const std::vector<double> &Temperature,
  const std::vector<double> &Pressure, int len_oxides, char *database,
  const std::vector<std::vector<double>> &bulkComposition,
  std::vector<stableAssemblage> &stableAssemblages)
{
  /**
   * This function is called from ASPECT Code and inturn calls MAGEMin
   * functions. It returns necessary data inform of data structures to ASPECT.
   */

  if (Temperature.size() != Pressure.size() ||
      Temperature.size() != bulkComposition.size())
    throw std::invalid_argument("MAGEMin batch input arrays have different sizes.");

  const int n_points = static_cast<int>(Temperature.size());
  stableAssemblages.clear();
  stableAssemblages.resize(n_points);
  if (n_points == 0)
    return;

  if (!initialized)
    initializeMAGEMin(database, &stableAssemblages[0]);
  else
    initializeStableAssemblage(database, &stableAssemblages[0]);

  for (int p = 1; p < n_points; ++p)
    initializeStableAssemblage(database, &stableAssemblages[p]);

  std::vector<io_data> input_data(n_points);

  for (int p = 0; p < n_points; p++)
    {

      z_b.T = Temperature[p]; // this value needs to be in Kelvin
      z_b.P = Pressure[p];    // this valye needs to be in kbar
      /**
       * Chemical potential of oxides; This values if used can be drawn from
       * aspect; or otherwise kept constant Same loop also covers bulk composition
       * which needs to be changedl with values derieved from ASPECT
       */
      for (int i = 0; i < len_oxides; i++)
        {
          gv.arg_gamma[i] = 0.0;
          gv.arg_bulk[i] = bulkComposition[p][i];
        }

      int point_index = p;
      gv.n_points = n_points;
      gv.numPoint = point_index;

      z_b = retrieve_bulk_PT(gv, input_data.data(), gv.numPoint, z_b);
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
                           gv, /** global variables (e.g. Gamma)  */

                           DB.PP_ref_db, /** pure phase database */
                           DB.SS_ref_db);

      applyPhaseSelection();

      gv.leveling_mode = 0;
      gv.fixed_bulk = 0;

      gv = ComputeEquilibrium_Point(
             gv.EM_database, input_data[point_index],
             z_b, /** bulk rock informations       */
             gv,  /** global variables (e.g. Gamma)  */

             &simplex, DB.PP_ref_db, /** pure phase database */
             DB.SS_ref_db,           /** solid solution database     */
             DB.cp);
      gv = ComputePostProcessing(
             z_b,          /** bulk rock informations      */
             gv,           /** global variables (e.g. Gamma)   */
             DB.PP_ref_db, /** pure phase database       */
             DB.SS_ref_db, /** solid solution database     */
             DB.cp);
      fill_output_struct(gv, &simplex, z_b, DB.PP_ref_db, DB.SS_ref_db, DB.cp,
                         DB.sp);
      get_output(gv, z_b, DB.PP_ref_db, DB.SS_ref_db, DB.cp, DB.sp,
                 &stableAssemblages[p], point_index);
      stableAssemblages[p].bulk_composition = bulkComposition[p];

    }
}
