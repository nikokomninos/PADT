#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <limits>
#include <padt/weights.hpp>
#include <stdexcept>

namespace {
InitialAircraftSizing empty_weight_frac_sizer(AircraftType aircraft_type,
                                              double design_weight,
                                              bool swing_wing) {
  return {
      {aircraft_type, EngineType::PureTurbojet, swing_wing},
      {15000.0, 300.0, 15.0, 18000.0, 0.0, design_weight},
      {1, 1, 1, 1, 1},
      1000.0};
}

InitialAircraftSizing fuel_frac_sizer(EngineType engine_type,
                                      MissionLegs mission) {
  return {{AircraftType::JetFighter, engine_type, false},
          {15000.0, 300.0, 15.0, 18000.0, 0.0, 3500.0},
          mission,
          1000.0};
}

InitialAircraftSizing fuel_frac_sizer(AircraftRequirements reqs) {
  return {{AircraftType::JetFighter, EngineType::HighBypassTurbofan, false},
          reqs,
          {1, 1, 1, 1, 1},
          1000.0};
}

InitialAircraftSizing initial_weight_sizer(AircraftType aircraft_type,
                                           bool swing_wing, double design_weight,
                                           double payload_weight) {
  return {{aircraft_type, EngineType::HighBypassTurbofan, swing_wing},
          {15000.0, 300.0, 15.0, 18000.0, 0.0, design_weight},
          {1, 1, 1, 1, 1},
          payload_weight};
}

} // namespace

TEST_CASE("Empty Weight Fraction Computations", "[compute_empty_weight_frac]") {
  SECTION("Sailplane - Unpowered") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::SailplaneUnpowered, 3500.0,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.57).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::SailplaneUnpowered, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.59).margin(0.01));
  }

  SECTION("Sailplane - Powered") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::SailplanePowered, 3500.0, false)
            .compute_empty_weight_frac() == Catch::Approx(0.61).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::SailplanePowered, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.63).margin(0.01));
  }

  SECTION("Homebuilt - Metal / Wood") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::HomebuiltMetalOrWood, 3500.0,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.57).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::HomebuiltMetalOrWood, 3500.0,
                                    true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.59).margin(0.01));
  }

  SECTION("Homebuilt - Composite") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::HomebuiltComposite, 3500.0,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.55).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::HomebuiltComposite, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.57).margin(0.01));
  }

  SECTION("General Aviation - Single Engine") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::GeneralSingleEngine, 3500.0,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.54).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::GeneralSingleEngine, 3500.0,
                                    true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.56).margin(0.01));
  }

  SECTION("General Aviation - Twin Engine") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::GeneralTwinEngine, 3500.0, false)
            .compute_empty_weight_frac() == Catch::Approx(0.67).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::GeneralTwinEngine, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.69).margin(0.01));
  }

  SECTION("Agricultural") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::Agricultural, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.58).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::Agricultural, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.60).margin(0.01));
  }

  SECTION("Twin Turboprop") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::TwinTurboprop, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.64).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::TwinTurboprop, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.66).margin(0.01));
  }

  SECTION("Flying Boat") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::FlyingBoat, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.72).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::FlyingBoat, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.75).margin(0.01));
  }

  SECTION("Jet Trainer") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTrainer, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.70).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTrainer, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.73).margin(0.01));
  }

  SECTION("Jet Fighter") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetFighter, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.81).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetFighter, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.85).margin(0.01));
  }

  SECTION("Military Cargo / Bomber") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::MilitaryCargoOrBomber,
                                    3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.53).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::MilitaryCargoOrBomber,
                                    3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.55).margin(0.01));
  }

  SECTION("Jet Transport") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTransport, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.63).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTransport, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.65).margin(0.01));
  }

  SECTION("UAV - Tac Recce and UCAV") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVTacRecceAndUCAV, 3500.0,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.45).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::UAVTacRecceAndUCAV, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.47).margin(0.01));
  }

  SECTION("UAV - High Altitude") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::UAVHighAltitude, 3500.0, false)
            .compute_empty_weight_frac() == Catch::Approx(0.63).margin(0.01));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::UAVHighAltitude, 3500.0, true)
            .compute_empty_weight_frac() == Catch::Approx(0.66).margin(0.01));
  }

  SECTION("UAV - Small") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVSmall, 3500.0, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.59).margin(0.01));

    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVSmall, 3500.0, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.62).margin(0.01));
  }

  SECTION("Invalid Requirement Arguments") {
    constexpr double invalid_values[] = {
        0.0,
        -1.0,
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
    };

    for (const double invalid_value : invalid_values) {
      REQUIRE_THROWS_WITH(
          empty_weight_frac_sizer(AircraftType::SailplaneUnpowered,
                                  invalid_value, false),
          "m_reqs.design_weight must be greater than 0.0");
    }
  }
}

TEST_CASE("Fuel Fraction Computations", "[compute_fuel_frac]") {
  SECTION("Turbo Jet") {
    REQUIRE(fuel_frac_sizer(EngineType::PureTurbojet, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.29).margin(0.01));
  }

  SECTION("Low-Bypass Turbofan") {
    REQUIRE(fuel_frac_sizer(EngineType::LowBypassTurbofan, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.27).margin(0.01));
  }

  SECTION("High-Bypass Turbofan") {
    REQUIRE(fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.18).margin(0.01));
  }

  SECTION("Invalid Mission Count Arguments") {
    REQUIRE_THROWS_WITH(fuel_frac_sizer(EngineType::HighBypassTurbofan, {}),
                        "m_mission.num_of_to must be at least 1");

    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {0, 1, 1, 1, 1})
            .compute_fuel_frac(),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 0, 1, 1, 1})
            .compute_fuel_frac(),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 1, 0, 1, 1})
            .compute_fuel_frac(),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 1, 1, 0, 1})
            .compute_fuel_frac(),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 1, 1, 1, 0})
            .compute_fuel_frac(),
        std::invalid_argument);
  }

  SECTION("Invalid Requirement Arguments") {
    constexpr double invalid_values[] = {
        0.0,
        -1.0,
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
    };

    for (const double invalid_value : invalid_values) {
      auto reqs =
          AircraftRequirements{invalid_value, 300.0, 15.0,
                               18000.0,      0.0,   3500.0};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs), std::invalid_argument);

      reqs = {15000.0, invalid_value, 15.0,
              18000.0, 0.0,          3500.0};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs), std::invalid_argument);

      reqs = {15000.0, 300.0, invalid_value, 18000.0, 0.0, 3500.0};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs), std::invalid_argument);

      reqs = {15000.0, 300.0, 15.0, invalid_value, 0.0, 3500.0};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs), std::invalid_argument);
    }
  }
}

TEST_CASE("Initial Weight Computations", "[compute_initial_weight]") {
  SECTION("Converges") {
    REQUIRE(initial_weight_sizer(AircraftType::JetTransport, false, 100000.0,
                                 20000.0)
                .compute_initial_weight() ==
            Catch::Approx(67142.0).margin(1.0));
  }

  SECTION("Invalid Payload Weight") {
    constexpr double invalid_values[] = {
        0.0,
        -1.0,
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
    };

    for (const double invalid_value : invalid_values) {
      REQUIRE_THROWS_WITH(initial_weight_sizer(AircraftType::JetTransport,
                                               false, 100000.0, invalid_value),
                          "m_payload_weight must be greater than 0.0");
    }
  }

  SECTION("Invalid Weight Fraction Sum") {
    REQUIRE_THROWS_AS(
        initial_weight_sizer(AircraftType::JetFighter, true, 3500.0, 1000.0)
            .compute_initial_weight(),
        std::domain_error);
  }
}
