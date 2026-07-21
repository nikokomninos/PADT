#include "weights.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <utility>

namespace {
using EmptyWeightFrac = std::pair<float, float>;
using FuelFrac = std::pair<float, float>;

// Raymer Table 3.1 - "Empty Weight Fractions vs W_0"
constexpr std::array<EmptyWeightFrac, static_cast<size_t>(AircraftType::Count)>
    empty_weight_frac_table = {
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

// Raymer Table 3.3 - "Specific Fuel Consumption, C"
constexpr std::array<FuelFrac, static_cast<size_t>(EngineType::Count)>
    fuel_frac_table = {
        FuelFrac(0.9f, 0.8f), // PureTurbojet
        FuelFrac(0.8f, 0.7f), // LowBypassTurbofan
        FuelFrac(0.5f, 0.4f), // HighBypassTurbofan
};

} // namespace

// Computes the empty weight fraction:
//
// W_e / W_0 = A * (W_0)^C * K_s
//
// Where:
//
// A = Constant
// C = Constant
// W_0 = Design gross takeoff weight
// K_s = Variable sweep constant
float compute_empty_weight_frac(const InitialSizingInput &input) {
  const float W_0 = input.reqs.design_weight;

  if (!std::isfinite(W_0) || W_0 <= 0.0f) {
    throw std::invalid_argument("design_weight must be greater than 0.0");
  }

  const float A = std::get<0>(
      empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)]);
  const float C = std::get<1>(
      empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)]);
  const float K_us = input.config.is_swing_wing ? 1.04f : 1.0f;

  const float empty_weight_frac = A * std::pow(W_0, C) * K_us;

  return empty_weight_frac;
}
