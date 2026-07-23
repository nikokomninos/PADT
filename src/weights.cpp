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

// Raymer Table 3.3 - "Specific Fuel Consumption, C [1/hr]"
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
// K_vs = Variable sweep constant
float compute_empty_weight_frac(const InitialSizingInput &input) {
  const float W_0 = input.reqs.design_weight;

  if (!std::isfinite(W_0) || W_0 <= 0.0f) {
    throw std::invalid_argument("design_weight must be greater than 0.0");
  }

  const float A = std::get<0>(
      empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)]);
  const float C = std::get<1>(
      empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)]);
  const float K_vs = input.config.is_swing_wing ? 1.04f : 1.0f;

  const float empty_weight_frac = A * std::pow(W_0, C) * K_vs;

  return empty_weight_frac;
}

// Computes the fuel fraction
//
// W_f = 1.06(1 - W_mission)
//
// W_mission = W_to * W_climb * W_cruise * W_loiter * W_ldg
//
// Where: 
//
// W_to = W_climb = W_ldg = const * number of times the phase occurs
//
// W_cruise = e^(-RC/v(l/d)) * number of times the phase occurs
// W_loiter = e^(-EC/(l/d)) * number of times the phase occurs
//
// R - Range [ft]
// C - SFC [1/s]
// v - airspeed [ft/s]
// l/d - lift to drag ratio
// E - loiter time [s]
float compute_fuel_frac(const InitialSizingInput &input) {
    const int number_of_takeoffs = input.mission.num_of_to;
    const int number_of_climbs = input.mission.num_of_climb;
    const int number_of_cruises = input.mission.num_of_cruise;
    const int number_of_loiters = input.mission.num_of_loiter;
    const int number_of_landings = input.mission.num_of_ldg;
    float ld_cruise;
    float ld_loiter;

    if (input.reqs.engine_type == EngineType::HighBypassTurbofan || input.reqs.engine_type == EngineType::LowBypassTurbofan 
        || input.reqs.engine_type == EngineType::PureTurbojet) {
        ld_cruise = 0.866*input.reqs.ld;
        ld_loiter = input.reqs.ld;
    }


    if (!std::isfinite(number_of_takeoffs) || number_of_takeoffs <= 0) {
        throw std::invalid_argument("there must be at least one takoff");
    }

    if (!std::isfinite(number_of_climbs) || number_of_climbs <= 0) {
        throw std::invalid_argument("there must be at least one climb");
    }

    if (!std::isfinite(number_of_cruises) || number_of_cruises <= 0) {
        throw std::invalid_argument("there must be at least one cruise");
    }

    if (!std::isfinite(number_of_loiters) || number_of_loiters <= 0) {
        throw std::invalid_argument("there must be at least one loiter");
    }

    if (!std::isfinite(number_of_landings) || number_of_landings <= 0) {
        throw std::invalid_argument("there must be at least one landing");
    }

    const float C_cruise = std::get<0>(
      fuel_frac_table[static_cast<size_t>(input.reqs.engine_type)])/3600.0f;
    const float C_loiter = std::get<1>(
      fuel_frac_table[static_cast<size_t>(input.reqs.engine_type)])/3600.0f;

    const float fuel_frac_to = 0.970*number_of_takeoffs;
    const float fuel_frac_climb = 0.985*number_of_climbs;
    const float fuel_frac_ldg = 0.995*number_of_landings;

    const float fuel_frac_cruise = std::exp(-(input.reqs.R*C_cruise)/(input.reqs.v*ld_cruise))*number_of_cruises; 

    const float fuel_frac_loiter = std::exp(-(input.reqs.loiter_time*C_loiter)/ld_loiter)*number_of_loiters;

    const float fuel_frac_mission = fuel_frac_to*fuel_frac_climb*fuel_frac_cruise*fuel_frac_loiter*fuel_frac_ldg;
    const float fuel_frac = 1.06*(1-fuel_frac_mission);

    return fuel_frac;
}