#include <iostream>
#include <padt/weights.hpp>
//#include <padt/atmosphere.hpp>

int main() {
  AircraftConfig config = {AircraftType::HomebuiltMetalOrWood,
                           EngineType::HighBypassTurbofan, false};
  AircraftRequirements reqs = {
      1.0, 1.0, 1.0, 1.0, 0.0, 3500.0};
  MissionLegs mission = {1, 1, 1, 1, 1};

  InitialAircraftSizing sizing{config, reqs, mission, 100.0};

  double frac = sizing.compute_initial_weight();
  std::cout << frac << "\n";
  //compute_mil_std_3013b_atmosphere(-15'001.0, DayType::StandardDay);
  return 0;
}
