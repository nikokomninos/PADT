#include <array>
#include <padt/atmosphere.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {
using AltitudeBounds = std::pair<float, float>;

constexpr std::array<AltitudeBounds, static_cast<size_t>(DayType::Count)>
    altitude_bounds_table{
        AltitudeBounds(-15'000.f, 150'000.f) // StandardDay
    };

constexpr std::array<std::string_view, static_cast<size_t>(DayType::Count)>
    atmosphere_types{"Standard Day"};

//TODO possibly replace with a lambda
constexpr bool is_valid_altitude(float altitude, DayType day_type) {
  const auto [lower,
              upper]{altitude_bounds_table[static_cast<size_t>(day_type)]};
  return (altitude >= lower && altitude <= upper);
}
} // namespace

Atmosphere compute_mil_std_3013b_atmosphere(float altitude, DayType day_type) {
  if (day_type == DayType::Count)
    // TODO replace with a compile time assert
    throw std::invalid_argument("invalid day type provided");
  // TODO include passed in value and include bounds in error string
  if (!is_valid_altitude(altitude, day_type))
    throw std::domain_error(
        "given altitude is outside the range of MIL-STD-3013B on a " +
        std::string{atmosphere_types[static_cast<size_t>(day_type)]});

  return {};
}
