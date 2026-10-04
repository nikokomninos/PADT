#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <padt/atmosphere.hpp>
#include <stdexcept>

TEST_CASE("Standard Day Atmosphere Equations", "[compute_std_day_conditions]") {
  SECTION("Sea Level") {
    const auto atmosphere = compute_std_day_conditions(0.0);
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
      const auto atmosphere = compute_std_day_conditions(reference.altitude);
      REQUIRE(atmosphere.temp_K ==
              Catch::Approx(reference.temp_K).margin(0.01));
      REQUIRE(atmosphere.temp_F ==
              Catch::Approx(reference.temp_F).margin(0.01));
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
      REQUIRE_THROWS_AS(compute_std_day_conditions(invalid_value),
                        std::domain_error);
    }
  }
}

TEST_CASE("Atmosphere Table Interpolation",
          "[interpolate_atmospheric_conditions]") {
  SECTION("Exact rows and table endpoints") {
    struct Reference {
      DayType day;
      double minimum;
      double maximum;
      double sea_level_temp_K;
      double minimum_temp_K;
      double maximum_temp_K;
    };
    constexpr Reference references[] = {
        {DayType::Standard, -15'000, 150'000, 288.15, 317.87, 267.07},
        {DayType::Polar, 0, 100'000, 246.65, 246.65, 210.15},
        {DayType::Tropical, 0, 100'000, 305.25, 305.25, 236.75},
        {DayType::Hot, 0, 15'000, 312.55, 312.55, 280.35},
    };
    for (const auto &reference : references) {
      CAPTURE(reference.day);
      const auto sea_level =
          interpolate_atmospheric_conditions(0, reference.day);
      REQUIRE(sea_level.temp_K == reference.sea_level_temp_K);
      REQUIRE(sea_level.press_psf == 2'116.22);
      const auto minimum =
          interpolate_atmospheric_conditions(reference.minimum, reference.day);
      const auto maximum =
          interpolate_atmospheric_conditions(reference.maximum, reference.day);
      REQUIRE(minimum.alt_ft == reference.minimum);
      REQUIRE(minimum.temp_K == reference.minimum_temp_K);
      REQUIRE(maximum.alt_ft == reference.maximum);
      REQUIRE(maximum.temp_K == reference.maximum_temp_K);
    }
  }

  SECTION("Standard day interpolates every column") {
    const auto conditions =
        interpolate_atmospheric_conditions(250, DayType::Standard);
    const auto check = [](std::optional<double> actual, double expected) {
      REQUIRE(actual.has_value());
      REQUIRE(*actual == Catch::Approx(expected));
    };
    REQUIRE(conditions.alt_ft == 250);
    REQUIRE(conditions.temp_F == Catch::Approx(58.1075));
    REQUIRE(conditions.temp_K == Catch::Approx(287.655));
    REQUIRE(conditions.press_psf == Catch::Approx(2'097.38));
    check(conditions.temp_R, 517.7775);
    check(conditions.temp_C, 14.505);
    check(conditions.temp_ratio, 0.998275);
    check(conditions.press_in_hg, 29.654825);
    check(conditions.press_ratio, 0.9911);
    check(conditions.density_slugs_ft3, 0.00235975);
    check(conditions.density_ratio, 0.992775);
    check(conditions.speed_of_sound_ft_s, 1'115.45);
    check(conditions.speed_of_sound_kts, 660.925);
    check(conditions.Q_M2, 1'468.125);
    check(conditions.viscosity_lb_sec_ft2, 3.732E-07);
    check(conditions.alt_geom_ft, 250);
    REQUIRE_FALSE(conditions.alt_geopotential_ft.has_value());
  }

  SECTION("Nonstandard days use pressure altitude") {
    struct Reference {
      DayType day;
      double temp_K;
      double geopotential_altitude;
    };
    constexpr Reference references[] = {
        {DayType::Polar, 247.075, 487.25},
        {DayType::Tropical, 304.70, 264.5},
        {DayType::Hot, 312.025, 250},
    };
    for (const auto &reference : references) {
      CAPTURE(reference.day);
      const auto conditions =
          interpolate_atmospheric_conditions(250, reference.day);
      REQUIRE(conditions.alt_ft == 250);
      REQUIRE(conditions.temp_K == Catch::Approx(reference.temp_K));
      REQUIRE(conditions.press_psf == Catch::Approx(2'097.38));
      REQUIRE(conditions.alt_geopotential_ft.has_value());
      REQUIRE(*conditions.alt_geopotential_ft ==
              Catch::Approx(reference.geopotential_altitude));
      REQUIRE(conditions.alt_geom_ft.has_value());
      REQUIRE(*conditions.alt_geom_ft ==
              Catch::Approx(reference.geopotential_altitude));
    }
  }

  SECTION("Wider altitude intervals") {
    const auto conditions =
        interpolate_atmospheric_conditions(102'500, DayType::Standard);
    REQUIRE(conditions.temp_K == Catch::Approx(227.895));
    REQUIRE(conditions.alt_geom_ft.has_value());
    REQUIRE(*conditions.alt_geom_ft == Catch::Approx(103'005.5));
  }

  SECTION("Invalid inputs") {
    for (const auto day :
         {DayType::Standard, DayType::Polar, DayType::Tropical, DayType::Hot}) {
      CAPTURE(day);
      const double minimum = day == DayType::Standard ? -15'000 : 0;
      const double maximum = day == DayType::Standard ? 150'000
                             : day == DayType::Hot    ? 15'000
                                                      : 100'000;
      const double invalid_values[] = {
          minimum - 1, maximum + 1, std::numeric_limits<double>::quiet_NaN(),
          std::numeric_limits<double>::infinity(),
          -std::numeric_limits<double>::infinity()};
      for (const double altitude : invalid_values) {
        CAPTURE(altitude);
        REQUIRE_THROWS_AS(interpolate_atmospheric_conditions(altitude, day),
                          std::domain_error);
      }
    }
    REQUIRE_THROWS_AS(
        interpolate_atmospheric_conditions(0, static_cast<DayType>(-1)),
        std::invalid_argument);
  }
}
