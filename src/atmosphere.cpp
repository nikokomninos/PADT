#include <array>
#include <padt/atmosphere.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
using AltitudeBands = std::pair<double, double>;

constexpr std::array<AltitudeBands, 4> alt_bands_standard{
    AltitudeBands(-15'000, 36'089), AltitudeBands(36'809, 65'617),
    AltitudeBands(65'617, 104'987), AltitudeBands(104'987, 150'000)};

constexpr std::array<std::string, 1> atmosphere_types{"Standard Day"};

} // namespace

AtmosphereEQ compute_std_day_atmosphere_by_eq(double altitude) {
  // TODO include passed in value and include bounds in error string

  if (altitude < alt_bands_standard.front().first ||
      altitude > alt_bands_standard.back().second)
    throw std::domain_error(
        "given altitude is outside the range of MIL-STD-3013B on a " +
        std::string{
            atmosphere_types[static_cast<size_t>(DayType::StandardDay)]});

  double temp_K, temp_F, press_psf;

  if (alt_bands_standard.at(0).first < altitude &&
      alt_bands_standard.at(0).second) {
    temp_K = 288.15 * (1 - 6.87558 * std::pow(10, -6) * altitude);

    press_psf =
        2116.22 * std::pow(1 - 6.87558 * std::pow(10, -6) * altitude, 5.2559f);
  }

  if (alt_bands_standard.at(1).first < altitude &&
      alt_bands_standard.at(1).second) {
    temp_K = 216.65;

    press_psf = 2116.22 * 0.22336 *
                std::exp((-4.80637 * std::pow(10, -5)) * (altitude - 36089.24));
  }

  if (alt_bands_standard.at(2).first < altitude &&
      alt_bands_standard.at(2).second) {
    temp_K = 216.65 * (1 + 1.40688 * std::pow(10, -6) * (altitude - 65616.8));

    press_psf = 2116.22 * 0.0540322 *
                std::pow(1 + 1.40688 * std::pow(10, -6) * (altitude - 65616.8),
                         -34.1634);
  }

  if (alt_bands_standard.at(3).first < altitude &&
      alt_bands_standard.at(3).second) {
    temp_K = 228.65 * (1 + 3.73252 * std::pow(10, -6) * (altitude - 104986.88));

    press_psf =
        2116.22 * 0.00856649 *
        std::pow(1 + 3.73252 * std::pow(10, -6) * (altitude - 104986.88f),
                 -12.2012);
  }

  temp_K = std::round(temp_K * 100.0) / 100.0;
  temp_F = std::round((temp_K * 1.8 - 459.67) * 100.0) / 100.0;
  press_psf = std::round(press_psf * 100.0) / 100.0;

  return {temp_K, temp_F, press_psf};
}

AtmosphereTable compute_std_atmosphere_by_table(double altitude,
                                                DayType day_type) {
  return {};
}
