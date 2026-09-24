#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <padt/atmosphere.hpp>
#include <stdexcept>

TEST_CASE("Standard Day Atmosphere Equations", "[compute_std_day_atmosphere_by_eq]") {
  SECTION("Sea Level") {
    const auto atmosphere = compute_std_day_atmosphere_by_eq(0.0);
    REQUIRE(atmosphere.temp_K == Catch::Approx(288.15).margin(0.01));
    REQUIRE(atmosphere.temp_F == Catch::Approx(59.00).margin(0.01));
    REQUIRE(atmosphere.press_psf == Catch::Approx(2116.22).margin(0.01));
  }

  SECTION("Atmospheric Bands") {
    struct Reference {
      double altitude;
      double temp_K;
      double temp_F;
      double press_psf;
    };

    constexpr Reference references[] = {
        {50'000.0, 216.65, -69.70, 242.21},
        {80'000.0, 221.03, -61.81, 57.67},
        {120'000.0, 241.46, -25.04, 9.32},
    };

    for (const auto &reference : references) {
      const auto atmosphere = compute_std_day_atmosphere_by_eq(reference.altitude);
      REQUIRE(atmosphere.temp_K == Catch::Approx(reference.temp_K).margin(0.01));
      REQUIRE(atmosphere.temp_F == Catch::Approx(reference.temp_F).margin(0.01));
      REQUIRE(atmosphere.press_psf ==
              Catch::Approx(reference.press_psf).margin(0.01));
    }
  }

  SECTION("Invalid Altitude Arguments") {
    constexpr double invalid_values[] = {
        -15'001.0,
        150'001.0,
        // std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
    };

    for (const double invalid_value : invalid_values) {
      CAPTURE(invalid_value);
      REQUIRE_THROWS_AS(compute_std_day_atmosphere_by_eq(invalid_value),
                        std::domain_error);
    }
  }
}
