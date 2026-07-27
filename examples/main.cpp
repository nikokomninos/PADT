#include <iostream>
#include <weights.hpp>

int main() {
  AircraftConfig config = {AircraftType::HomebuiltMetalOrWood, false};
  AircraftRequirements reqs = {
      1.0f, EngineType::HighBypassTurbofan, 1.0f, 1.0f, 1.0f, 0.0f, 3500.0f};
  MissionLegs mission = {1, 1, 1, 1, 1};

  InitialAircraftSizing sizing{config, reqs, mission, 100.0f};

  float frac = sizing.compute_initial_weight();
  std::cout << frac << "\n";
}
