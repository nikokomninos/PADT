#include <iostream>
#include <padt/atmosphere.hpp>
#include <padt/weights.hpp>

int main() {
  // AircraftConfig config = {AircraftType::HomebuiltMetalOrWood,
  //                          EngineType::HighBypassTurbofan, false};
  // AircraftRequirements reqs = {
  //     1.0, 1.0, 1.0, 1.0, 0.0, 3500.0};
  // MissionLegs mission = {1, 1, 1, 1, 1};

  // InitialAircraftSizing sizing{config, reqs, mission, 100.0};

  // double frac = sizing.compute_initial_weight();
  // std::cout << frac << "\n";

  auto atm = interpolate_atmospheric_conditions(50'000, DayType::Standard);
  std::cout << atm.press_ratio.value() << "\n";
  return 0;
}
