#include <iostream>
#include <padt/weights.hpp>
#include <padt/atmosphere.hpp>

int main() {
  // AircraftConfig config = {AircraftType::HomebuiltMetalOrWood,
  //                          EngineType::HighBypassTurbofan, false};
  // AircraftRequirements reqs = {
  //     1.0, 1.0, 1.0, 1.0, 0.0, 3500.0};
  // MissionLegs mission = {1, 1, 1, 1, 1};

  // InitialAircraftSizing sizing{config, reqs, mission, 100.0};

  // double frac = sizing.compute_initial_weight();
  // std::cout << frac << "\n";

  auto atm = compute_std_day_atmosphere_by_eq(80'000.0);
  std::cout <<
    "Degrees K: " << atm.temp_K << "\n"
    << "Degrees F: " << atm.temp_F << "\n"
    << "Pressure psf: " << atm.press_psf << "\n"
    << "\n";
  return 0;
}
