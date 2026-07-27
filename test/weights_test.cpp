#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <stdexcept>
#include <weights.hpp>

namespace {
InitialAircraftSizing empty_weight_frac_sizer(AircraftType aircraft_type,
                                              float design_weight,
                                              bool swing_wing) {
  return {
      {aircraft_type, swing_wing},
      {0.0f, EngineType::PureTurbojet, 0.0f, 0.0f, 0.0f, 0.0f, design_weight},
      {1, 1, 1, 1, 1},
      0.0f};
}

InitialAircraftSizing fuel_frac_sizer(EngineType engine_type,
                                      MissionLegs mission) {
  return {{AircraftType::JetFighter, false},
          {15000.0f, engine_type, 300.0f, 15.0f, 18000.0f, 0.0f, 3500.0f},
          mission,
          0.0f};
}

InitialAircraftSizing fuel_frac_sizer(AircraftRequirements reqs) {
  return {{AircraftType::JetFighter, false}, reqs, {1, 1, 1, 1, 1}, 0.0f};
}

InitialAircraftSizing initial_weight_sizer(AircraftType aircraft_type,
                                           bool swing_wing, float design_weight,
                                           float payload_weight) {
  return {{aircraft_type, swing_wing},
          {15000.0f, EngineType::HighBypassTurbofan, 300.0f, 15.0f, 18000.0f,
           0.0f, design_weight},
          {1, 1, 1, 1, 1},
          payload_weight};
}

} // namespace

TEST_CASE("Empty Weight Fraction Calculations", "[compute_empty_weight_frac]") {
  SECTION("Sailplane - Unpowered") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::SailplaneUnpowered, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.57f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::SailplaneUnpowered, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.59f).margin(0.01f));
  }

  SECTION("Sailplane - Powered") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::SailplanePowered, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.61f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::SailplanePowered, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.63f).margin(0.01f));
  }

  SECTION("Homebuilt - Metal / Wood") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::HomebuiltMetalOrWood, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.57f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::HomebuiltMetalOrWood, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.59f).margin(0.01f));
  }

  SECTION("Homebuilt - Composite") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::HomebuiltComposite, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.55f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::HomebuiltComposite, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.57f).margin(0.01f));
  }

  SECTION("General Aviation - Single Engine") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::GeneralSingleEngine, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.54f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::GeneralSingleEngine, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.56f).margin(0.01f));
  }

  SECTION("General Aviation - Twin Engine") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::GeneralTwinEngine, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.67f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::GeneralTwinEngine, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.69f).margin(0.01f));
  }

  SECTION("Agricultural") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::Agricultural, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.58f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::Agricultural, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.60f).margin(0.01f));
  }

  SECTION("Twin Turboprop") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::TwinTurboprop, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.64f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::TwinTurboprop, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.66f).margin(0.01f));
  }

  SECTION("Flying Boat") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::FlyingBoat, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.72f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::FlyingBoat, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.75f).margin(0.01f));
  }

  SECTION("Jet Trainer") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTrainer, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.70f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTrainer, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.73f).margin(0.01f));
  }

  SECTION("Jet Fighter") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetFighter, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.81f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetFighter, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.85f).margin(0.01f));
  }

  SECTION("Military Cargo / Bomber") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::MilitaryCargoOrBomber, 3500.0f,
                                    false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.53f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::MilitaryCargoOrBomber, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.55f).margin(0.01f));
  }

  SECTION("Jet Transport") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTransport, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.63f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::JetTransport, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.65f).margin(0.01f));
  }

  SECTION("UAV - Tac Recce and UCAV") {
    REQUIRE(
        empty_weight_frac_sizer(AircraftType::UAVTacRecceAndUCAV, 3500.0f, false)
            .compute_empty_weight_frac() == Catch::Approx(0.45f).margin(0.01f));

    REQUIRE(
        empty_weight_frac_sizer(AircraftType::UAVTacRecceAndUCAV, 3500.0f, true)
            .compute_empty_weight_frac() == Catch::Approx(0.47f).margin(0.01f));
  }

  SECTION("UAV - High Altitude") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVHighAltitude, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.63f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVHighAltitude, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.66f).margin(0.01f));
  }

  SECTION("UAV - Small") {
    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVSmall, 3500.0f, false)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.59f).margin(0.01f));

    REQUIRE(empty_weight_frac_sizer(AircraftType::UAVSmall, 3500.0f, true)
                .compute_empty_weight_frac() ==
            Catch::Approx(0.62f).margin(0.01f));
  }

  SECTION("Invalid Requirement Arguments") {
    constexpr float invalid_values[] = {
        0.0f,
        -1.0f,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };

    for (const float invalid_value : invalid_values) {
      REQUIRE_THROWS_AS(
          empty_weight_frac_sizer(AircraftType::SailplaneUnpowered,
                                  invalid_value, false)
              .compute_empty_weight_frac(),
          std::invalid_argument);
    }
  }
}

TEST_CASE("Fuel Fraction Calculation", "[compute_fuel_frac]") {
  SECTION("Turbo Jet") {
    REQUIRE(fuel_frac_sizer(EngineType::PureTurbojet, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.29f).margin(0.01f));
  }

  SECTION("Low-Bypass Turbofan") {
    REQUIRE(fuel_frac_sizer(EngineType::LowBypassTurbofan, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.27f).margin(0.01f));
  }

  SECTION("High-Bypass Turbofan") {
    REQUIRE(fuel_frac_sizer(EngineType::HighBypassTurbofan, {1, 1, 1, 1, 1})
                .compute_fuel_frac() == Catch::Approx(0.18f).margin(0.01f));
  }

  SECTION("Invalid Mission Count Arguments") {
    REQUIRE_THROWS_AS(
        fuel_frac_sizer(EngineType::HighBypassTurbofan, {}).compute_fuel_frac(),
        std::invalid_argument);

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
    constexpr float invalid_values[] = {
        0.0f,
        -1.0f,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };

    for (const float invalid_value : invalid_values) {
      auto reqs =
          AircraftRequirements{invalid_value, EngineType::HighBypassTurbofan,
                               300.0f,        15.0f,
                               18000.0f,      0.0f,
                               3500.0f};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs).compute_fuel_frac(),
                        std::invalid_argument);

      reqs = {15000.0f,      EngineType::HighBypassTurbofan,
              invalid_value, 15.0f,
              18000.0f,      0.0f,
              3500.0f};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs).compute_fuel_frac(),
                        std::invalid_argument);

      reqs = {15000.0f, EngineType::HighBypassTurbofan,
              300.0f,   invalid_value,
              18000.0f, 0.0f,
              3500.0f};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs).compute_fuel_frac(),
                        std::invalid_argument);

      reqs = {15000.0f,      EngineType::HighBypassTurbofan,
              300.0f,        15.0f,
              invalid_value, 0.0f,
              3500.0f};
      REQUIRE_THROWS_AS(fuel_frac_sizer(reqs).compute_fuel_frac(),
                        std::invalid_argument);
    }
  }
}

TEST_CASE("Initial Weight Calculation", "[compute_initial_weight]") {
  SECTION("Converges") {
    REQUIRE(initial_weight_sizer(AircraftType::JetTransport, false, 100000.0f,
                                 20000.0f)
                .compute_initial_weight() ==
            Catch::Approx(67142.0f).margin(1.0f));
  }

  SECTION("Invalid Payload Weight") {
    constexpr float invalid_values[] = {
        0.0f,
        -1.0f,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };

    for (const float invalid_value : invalid_values) {
      REQUIRE_THROWS_AS(initial_weight_sizer(AircraftType::JetTransport, false,
                                             100000.0f, invalid_value)
                            .compute_initial_weight(),
                        std::invalid_argument);
    }
  }

  SECTION("Invalid Weight Fraction Sum") {
    REQUIRE_THROWS_AS(
        initial_weight_sizer(AircraftType::JetFighter, true, 3500.0f, 1000.0f)
            .compute_initial_weight(),
        std::domain_error);
  }
}
