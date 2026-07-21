#include "weights.hpp"

#include <utility>
#include <array>
#include <cstdio>

namespace {
  using EmptyWeightFrac = std::pair<float, float>;

  // Raymer Table 3.1
  constexpr std::array<EmptyWeightFrac, static_cast<size_t>(AircraftType::Count)> empty_weight_frac_table = {
        EmptyWeightFrac(0.86f, -0.05f), // SailplaneUnpowered
        EmptyWeightFrac(0.91f, -0.05f), // SailplanePowered
        EmptyWeightFrac(1.19f, -0.09f), // HomebuiltMetalOrWood
        EmptyWeightFrac(1.15f, -0.09f), // HomebuiltComposite
        EmptyWeightFrac(2.36f, -0.18f), // GeneralSingleEngine
        EmptyWeightFrac(1.51f, -0.10f), // GeneralTwinEngine
        EmptyWeightFrac(0.74f, -0.03f), // Agricultural
        EmptyWeightFrac(0.96f, -0.05f), // TwinTurboprop
        EmptyWeightFrac(1.09f, -0.05f), // FlyingBoat
        EmptyWeightFrac(1.59f, -0.10f), // JetTrainer
        EmptyWeightFrac(2.35f, -0.13f), // JetFighter
        EmptyWeightFrac(0.93f, -0.07f), // MilitaryCargoOrBomber
        EmptyWeightFrac(1.02f, -0.06f), // JetTransport
        EmptyWeightFrac(1.67f, -0.16f), // UAVTacRecceAndUCAV
        EmptyWeightFrac(2.75f, -0.18f), // UAVHighAltitude
        EmptyWeightFrac(0.97f, -0.06f), // UAVSmall
  };
}

float compute_empty_weight_frac(InitialSizingInput &input) {
  auto [A, C] = empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)];

  std::printf("A: %f, C: %f\n", A, C);
  return 0.0f;
}
