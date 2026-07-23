#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <stdexcept>
#include <weights.hpp>

namespace {
InitialSizingInput empty_weight_frac_input(AircraftType aircraft_type, float design_weight,
                               bool swing_wing) {
  return {
      {aircraft_type, swing_wing},
      {0.0f, EngineType::PureTurbojet, 0.0f, 0.0f, 0.0f, 0.0f, design_weight},
      0.0f};
}

InitialSizingInput fuel_frac_input(EngineType engine_type, MissionLegs mission) {
        return {
                {AircraftType::JetFighter, false},
                {15000.0f, engine_type, 300.0, 15.0f, 18000.0f, 0.0f},
                0.0f,
                mission
        };
}

} // namespace

TEST_CASE("Empty Weight Fraction Calculations", "[compute_empty_weight_frac]") {
  SECTION("Sailplane - Unpowered") {
    auto input = empty_weight_frac_input(AircraftType::SailplaneUnpowered, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.57).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.59).margin(0.01));
  }

  SECTION("Sailplane - Powered") {
    auto input = empty_weight_frac_input(AircraftType::SailplanePowered, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.61).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.63).margin(0.01));
  }

  SECTION("Homebuilt - Metal / Wood") {
    auto input = empty_weight_frac_input(AircraftType::HomebuiltMetalOrWood, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.57).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.59).margin(0.01));
  }

  SECTION("Homebuilt - Composite") {
    auto input = empty_weight_frac_input(AircraftType::HomebuiltComposite, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.55).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.57).margin(0.01));
  }

  SECTION("General Aviation - Single Engine") {
    auto input = empty_weight_frac_input(AircraftType::GeneralSingleEngine, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.54).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.56).margin(0.01));
  }

  SECTION("General Aviation - Twin Engine") {
    auto input = empty_weight_frac_input(AircraftType::GeneralTwinEngine, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.67).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.69).margin(0.01));
  }

  SECTION("Agricultural") {
    auto input = empty_weight_frac_input(AircraftType::Agricultural, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.58).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.60).margin(0.01));
  }

  SECTION("Twin Turboprop") {
    auto input = empty_weight_frac_input(AircraftType::TwinTurboprop, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.64).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.66).margin(0.01));
  }

  SECTION("Flying Boat") {
    auto input = empty_weight_frac_input(AircraftType::FlyingBoat, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.72).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.75).margin(0.01));
  }

  SECTION("Jet Trainer") {
    auto input = empty_weight_frac_input(AircraftType::JetTrainer, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.70).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.73).margin(0.01));
  }

  SECTION("Jet Fighter") {
    auto input = empty_weight_frac_input(AircraftType::JetFighter, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.81).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.85).margin(0.01));
  }

  SECTION("Military Cargo / Bomber") {
    auto input = empty_weight_frac_input(AircraftType::MilitaryCargoOrBomber, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.53).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.55).margin(0.01));
  }

  SECTION("Jet Transport") {
    auto input = empty_weight_frac_input(AircraftType::JetTransport, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.63).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.65).margin(0.01));
  }

  SECTION("UAV - Tac Recce and UCAV") {
    auto input = empty_weight_frac_input(AircraftType::UAVTacRecceAndUCAV, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.45).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.47).margin(0.01));
  }

  SECTION("UAV - High Altitude") {
    auto input = empty_weight_frac_input(AircraftType::UAVHighAltitude, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.63).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.66).margin(0.01));
  }

  SECTION("UAV - Small") {
    auto input = empty_weight_frac_input(AircraftType::UAVSmall, 3500, false);
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.59).margin(0.01));

    input.config.is_swing_wing = true;
    REQUIRE(compute_empty_weight_frac(input) ==
            Catch::Approx(0.62).margin(0.01));
  }

  SECTION("Invalid design_weight Arguments") {
    auto input = empty_weight_frac_input(AircraftType::SailplaneUnpowered, 0.0, false);
    REQUIRE_THROWS_AS(compute_empty_weight_frac(input), std::invalid_argument);

    input.reqs.design_weight = -3500;
    REQUIRE_THROWS_AS(compute_empty_weight_frac(input), std::invalid_argument);

    input.reqs.design_weight = std::numeric_limits<float>::quiet_NaN();
    REQUIRE_THROWS_AS(compute_empty_weight_frac(input), std::invalid_argument);

    input.reqs.design_weight = std::numeric_limits<float>::infinity();
    REQUIRE_THROWS_AS(compute_empty_weight_frac(input), std::invalid_argument);

    input.reqs.design_weight = -std::numeric_limits<float>::infinity();
    REQUIRE_THROWS_AS(compute_empty_weight_frac(input), std::invalid_argument);
  }
}

TEST_CASE("Fuel Fraction Calculation", "[compute_fuel_frac]") {
  SECTION("Turbo Jet") {
    auto input = fuel_frac_input(EngineType::PureTurbojet, {1, 1, 1, 1, 1});
    REQUIRE(compute_fuel_frac(input) == 
            Catch::Approx(0.29).margin(0.01)); 
  }

  SECTION("Low-Bypass Turbofan") {
    auto input = fuel_frac_input(EngineType::LowBypassTurbofan, {1, 1, 1, 1, 1});
    REQUIRE(compute_fuel_frac(input) == 
            Catch::Approx(0.27).margin(0.01)); 
  }

  SECTION("High-Bypass Turbofan") {
    auto input = fuel_frac_input(EngineType::HighBypassTurbofan, {1, 1, 1, 1, 1});
    REQUIRE(compute_fuel_frac(input) == 
            Catch::Approx(0.18).margin(0.01)); 
  }

  SECTION("Invalid design_weight Arguments") {
    auto input = fuel_frac_input(EngineType::HighBypassTurbofan, {});
    REQUIRE_THROWS_AS(compute_fuel_frac(input), std::invalid_argument);


    input.mission.num_of_to = -1;
    REQUIRE_THROWS_AS(compute_fuel_frac(input), std::invalid_argument);


    input.mission.num_of_to = std::numeric_limits<float>::quiet_NaN();
    REQUIRE_THROWS_AS(compute_fuel_frac(input), std::invalid_argument);


    input.mission.num_of_to = std::numeric_limits<float>::infinity();
    REQUIRE_THROWS_AS(compute_fuel_frac(input), std::invalid_argument);


    input.mission.num_of_to = -std::numeric_limits<float>::infinity();
    REQUIRE_THROWS_AS(compute_fuel_frac(input), std::invalid_argument);
  }

}