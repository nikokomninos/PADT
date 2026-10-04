#pragma once

#include <optional>

enum class DayType { Standard, Polar, Tropical, Hot };

struct AtmosphericConditions {
  double alt_ft; // Geopotential on standard day, pressure on all others
  double temp_F;
  std::optional<double> temp_R = std::nullopt;
  std::optional<double> temp_C = std::nullopt;
  double temp_K;
  std::optional<double> temp_ratio = std::nullopt;
  std::optional<double> press_in_hg = std::nullopt;
  double press_psf;
  std::optional<double> press_ratio = std::nullopt;
  std::optional<double> density_slugs_ft3 = std::nullopt;
  std::optional<double> density_ratio = std::nullopt;
  std::optional<double> speed_of_sound_ft_s = std::nullopt;
  std::optional<double> speed_of_sound_kts = std::nullopt;
  std::optional<double> Q_M2 = std::nullopt;
  std::optional<double> viscosity_lb_sec_ft2 = std::nullopt;
  std::optional<double> alt_geopotential_ft = std::nullopt;
  std::optional<double> alt_geom_ft = std::nullopt;
};

/**
 * @brief Computes the atmosphere for a standard day using the equations
 * from MIL-STD-3013B Appendix A Table A-II
 *
 * @param altitude The altitude to compute the standard day atmosphere at
 *
 * @throws std::domain_error if the altitude is below or above the possible
 * bounds for computation
 *
 * @return AtmosphericConditions, with only the altitude, temp in F, temp in K,
 * and pressure in psf.
 */
AtmosphericConditions compute_std_day_conditions(double geopotential_altitude);

/**
 * @brief Linearly interpolates the selected MIL-STD-3013B atmosphere table.
 *
 * @param altitude_ft Geopotential altitude for Standard Day, pressure altitude
 * for Polar, Tropical, and Hot Days, in feet.
 *
 * @throws std::domain_error if altitude is nonfinite or outside the table
 * range.
 *
 * @throws std::invalid_argument if day_type is unknown.
 *
 * @return All table fields; alt_geopotential_ft is unset for Standard Day,
 * where alt_ft already represents geopotential altitude.
 */
AtmosphericConditions interpolate_atmospheric_conditions(double altitude_ft,
                                                         DayType day_type);
