#include <weights.hpp>

int main (int argc, char *argv[]) {
  AircraftConfig config = { AircraftType::SailplaneUnpowered, false };
  AircraftRequirements reqs = { 0, EngineType::HighBypassTurbofan, 0, 0, 0, 0, 0 };
  float payload_weight = 0.0f;

  InitialSizingInput input = { config, reqs, payload_weight };

  compute_empty_weight_frac(input);
}
