#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <padt/atmosphere.hpp>
#include <stdexcept>

TEST_CASE("MIL-STD-3013B Atmosphere Computations",
          "[compute_mil_std_3013b_atmosphere]") {
  SECTION("Invalid Altitude Arguments") {
    constexpr float invalid_values[] = {
        -15'001.f,
        150'001.f,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };

    for (const float invalid_value : invalid_values) {
      REQUIRE_THROWS_AS(
          compute_mil_std_3013b_atmosphere(invalid_value, DayType::StandardDay),
          std::domain_error);
    }
  }
}
