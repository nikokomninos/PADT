#include <iostream>
#include <padt/weights.hpp>
//#include <padt/atmosphere.hpp>

int main() {
  AircraftConfig config = {AircraftType::HomebuiltMetalOrWood,
                           EngineType::HighBypassTurbofan, false};
  AircraftRequirements reqs = {
      1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3500.0f};
  MissionLegs mission = {1, 1, 1, 1, 1};

  InitialAircraftSizing sizing{config, reqs, mission, 100.0f};

  float frac = sizing.compute_initial_weight();
  std::cout << frac << "\n";
  //compute_mil_std_3013b_atmosphere(-15'001.f, DayType::StandardDay);
  return 0;
}
