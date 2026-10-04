#include <algorithm>
#include <array>
#include <cmath>
#include <padt/atmosphere.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace {
using AltitudeBands = std::pair<double, double>;

inline constexpr std::array<AltitudeBands, 4> alt_bands_standard{
    AltitudeBands(-15'000, 36'089), AltitudeBands(36'809, 65'617),
    AltitudeBands(65'617, 104'987), AltitudeBands(104'987, 150'000)};

inline constexpr std::array<std::string, 4> atmosphere_types{
    "Standard Day", "Polar Day", "Tropical Day", "Hot Day"};

struct StandardTableRow {
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

struct NonStandardTableRow {
  double alt_ft;
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
  double alt_geopotential_ft;
  double alt_geom_ft;
};

#include "atmosphere_tables.inc"

double interpolate(double lower, double upper, double fraction) {
  return std::lerp(lower, upper, fraction);
}

template <typename Row, size_t N>
AtmosphericConditions interpolate_table(double altitude_ft,
                                        const std::array<Row, N> &table) {
  if (!std::isfinite(altitude_ft) || altitude_ft < table.front().alt_ft ||
      altitude_ft > table.back().alt_ft)
    throw std::domain_error("altitude is outside the atmosphere table range");

  const auto upper{std::lower_bound(
      table.begin(), table.end(), altitude_ft,
      [](const Row &row, double altitude) { return row.alt_ft < altitude; })};
  const auto lower{upper->alt_ft == altitude_ft ? upper : upper - 1};
  const double fraction{lower == upper ? 0.0
                                       : (altitude_ft - lower->alt_ft) /
                                             (upper->alt_ft - lower->alt_ft)};
  const auto value{[&](double Row::*member) {
    return interpolate((*lower).*member, (*upper).*member, fraction);
  }};

  AtmosphericConditions conditions{
      .alt_ft = altitude_ft,
      .temp_F = value(&Row::temp_F),
      .temp_R = value(&Row::temp_R),
      .temp_C = value(&Row::temp_C),
      .temp_K = value(&Row::temp_K),
      .temp_ratio = value(&Row::temp_ratio),
      .press_in_hg = value(&Row::press_in_hg),
      .press_psf = value(&Row::press_psf),
      .press_ratio = value(&Row::press_ratio),
      .density_slugs_ft3 = value(&Row::density_slugs_ft3),
      .density_ratio = value(&Row::density_ratio),
      .speed_of_sound_ft_s = value(&Row::speed_of_sound_ft_s),
      .speed_of_sound_kts = value(&Row::speed_of_sound_kts),
      .Q_M2 = value(&Row::Q_M2),
      .viscosity_lb_sec_ft2 = value(&Row::viscosity_lb_sec_ft2),
      .alt_geom_ft = value(&Row::alt_geom_ft)};
  if constexpr (std::is_same_v<Row, NonStandardTableRow>)
    conditions.alt_geopotential_ft = value(&Row::alt_geopotential_ft);
  return conditions;
}

} // namespace

AtmosphericConditions compute_std_day_conditions(double geopotential_altitude) {
  // TODO include passed in value and include bounds in error string

  if (geopotential_altitude < alt_bands_standard.front().first ||
      geopotential_altitude > alt_bands_standard.back().second)
    throw std::domain_error(
        "given altitude is outside the range of MIL-STD-3013B on a " +
        std::string{atmosphere_types[static_cast<size_t>(DayType::Standard)]});

  double temp_K, temp_F, press_psf;

  if (alt_bands_standard.at(0).first < geopotential_altitude &&
      alt_bands_standard.at(0).second) {
    temp_K = 288.15 * (1 - 6.87558 * std::pow(10, -6) * geopotential_altitude);

    press_psf = 2'116.22 *
                std::pow(1 - 6.87558 * std::pow(10, -6) * geopotential_altitude,
                         5.2559f);
  }

  if (alt_bands_standard.at(1).first < geopotential_altitude &&
      alt_bands_standard.at(1).second) {
    temp_K = 216.65;

    press_psf = 2'116.22 * 0.22336 *
                std::exp((-4.80637 * std::pow(10, -5)) *
                         (geopotential_altitude - 36'089.24));
  }

  if (alt_bands_standard.at(2).first < geopotential_altitude &&
      alt_bands_standard.at(2).second) {
    temp_K = 216.65 * (1 + 1.40688 * std::pow(10, -6) *
                               (geopotential_altitude - 65'616.8));

    press_psf = 2'116.22 * 0.0540322 *
                std::pow(1 + 1.40688 * std::pow(10, -6) *
                                 (geopotential_altitude - 65'616.8),
                         -34.1634);
  }

  if (alt_bands_standard.at(3).first < geopotential_altitude &&
      alt_bands_standard.at(3).second) {
    temp_K = 228.65 * (1 + 3.73252 * std::pow(10, -6) *
                               (geopotential_altitude - 104'986.88));

    press_psf = 2'116.22 * 0.00856649 *
                std::pow(1 + 3.73252 * std::pow(10, -6) *
                                 (geopotential_altitude - 104'986.88f),
                         -12.2012);
  }

  temp_K = std::round(temp_K * 100.0) / 100.0;
  temp_F = std::round((temp_K * 1.8 - 459.67) * 100.0) / 100.0;
  press_psf = std::round(press_psf * 100.0) / 100.0;

  return {.alt_ft = geopotential_altitude,
          .temp_F = temp_F,
          .temp_K = temp_K,
          .press_psf = press_psf};
}

AtmosphericConditions interpolate_atmospheric_conditions(double altitude_ft,
                                                         DayType day_type) {
  switch (day_type) {
  case DayType::Standard:
    return interpolate_table(altitude_ft, standard_day_table);
  case DayType::Polar:
    return interpolate_table(altitude_ft, polar_day_table);
  case DayType::Tropical:
    return interpolate_table(altitude_ft, tropical_day_table);
  case DayType::Hot:
    return interpolate_table(altitude_ft, hot_day_table);
  }
  throw std::invalid_argument("unknown atmosphere day type");
}
