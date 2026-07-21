#include <iostream>
#include <weights.hpp>

int main (int argc, char *argv[]) {
  AircraftConfig config = { AircraftType::HomebuiltMetalOrWood, false };
  AircraftRequirements reqs = { 0, EngineType::HighBypassTurbofan, 0, 0, 0, 0, 3500 };
  float payload_weight = 0.0f;

  InitialSizingInput input = { config, reqs, payload_weight };

  float frac = compute_empty_weight_frac(input);
  std::cout << frac << "\n";
}
