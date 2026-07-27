#include <iostream>
#include <weights.hpp>

int main() {
  AircraftConfig config = {AircraftType::HomebuiltMetalOrWood, false};
  AircraftRequirements reqs = {
      1.0f, EngineType::HighBypassTurbofan, 1.0f, 1.0f, 1.0f, 0.0f, 3500.0f};
  MissionLegs mission = {1, 1, 1, 1, 1};

  InitialAircraftSizing sizing{config, reqs, mission, 0.0f};

  float frac = sizing.compute_empty_weight_frac();
  std::cout << frac << "\n";
}
