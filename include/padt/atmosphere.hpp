#pragma once

enum class DayType { StandardDay };

struct AtmosphereEQ {
  double temp_K;
  double temp_F;
  double press_psf;
};

struct AtmosphereTable {
  double alt_ft;
  double alt_geom_ft;
  double temp_F;
  double temp_R;
  double temp_C;
  double temp_K;
  double temp_ratio;
  double press_in_hg;
  double press_psf;
  double press_ratio;
  double density_slugs_ft3;
  double density_ratio;
  double speed_of_sound_ft_s;
  double speed_of_sound_kts;
  double Q_M2;
  double viscosity_lb_sec_ft2;
};

/**
 * @brief Computes the atmosphere for a standard day using the equations
 * from MIL-STD-3013B Appendix A Table A-II
 *
 * @param altitude The altitude to compute the standard day atmosphere at
 *
 * @throws std::domain_error if the altitude is below or above the possible
 * bounds for computation
 */
AtmosphereEQ compute_std_day_atmosphere_by_eq(double altitude);

AtmosphereTable compute_std_atmosphere_by_table(double altitude,
                                                DayType day_type);
