#include <padt/weights.hpp>

#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {
using EmptyWeightFrac = std::pair<double, double>;
using FuelFrac = std::pair<double, double>;

// Raymer Table 3.1 - "Empty Weight Fractions vs W_0"
inline constexpr std::array<EmptyWeightFrac, 16> empty_weight_frac_table{
    EmptyWeightFrac(0.86, -0.05), // SailplaneUnpowered
    EmptyWeightFrac(0.91, -0.05), // SailplanePowered
    EmptyWeightFrac(1.19, -0.09), // HomebuiltMetalOrWood
    EmptyWeightFrac(1.15, -0.09), // HomebuiltComposite
    EmptyWeightFrac(2.36, -0.18), // GeneralSingleEngine
    EmptyWeightFrac(1.51, -0.10), // GeneralTwinEngine
    EmptyWeightFrac(0.74, -0.03), // Agricultural
    EmptyWeightFrac(0.96, -0.05), // TwinTurboprop
    EmptyWeightFrac(1.09, -0.05), // FlyingBoat
    EmptyWeightFrac(1.59, -0.10), // JetTrainer
    EmptyWeightFrac(2.35, -0.13), // JetFighter
    EmptyWeightFrac(0.93, -0.07), // MilitaryCargoOrBomber
    EmptyWeightFrac(1.02, -0.06), // JetTransport
    EmptyWeightFrac(1.67, -0.16), // UAVTacRecceAndUCAV
    EmptyWeightFrac(2.75, -0.18), // UAVHighAltitude
    EmptyWeightFrac(0.97, -0.06), // UAVSmall
};

// Raymer Table 3.3 - "Specific Fuel Consumption, C [1/hr]"
inline constexpr std::array<FuelFrac, 3> fuel_frac_table{
    FuelFrac(0.9, 0.8), // PureTurbojet
    FuelFrac(0.8, 0.7), // LowBypassTurbofan
    FuelFrac(0.5, 0.4), // HighBypassTurbofan
};

/**
 * @brief Helper to check that an argument has a positive count
 *
 * @param value The value being checked
 * @param name The name of the variable being checked
 */
void require_positive(double value, std::string_view name) {
  if (!std::isfinite(value) || value <= 0.0) {
    throw std::invalid_argument(std::string{name} +
                                " must be greater than 0.0");
  }
}

/**
 * @brief Helper to check that an argument has a nonzero count
 *
 * @param value The value being checked
 * @param name The name of the variable being checked
 */
void require_nonzero(unsigned int value, std::string_view name) {
  if (value == 0) {
    throw std::invalid_argument(std::string{name} + " must be at least 1");
  }
}

#define REQUIRE_POSITIVE(value) require_positive((value), #value)
#define REQUIRE_NONZERO(value) require_nonzero((value), #value)

} // namespace

InitialAircraftSizing::InitialAircraftSizing(AircraftConfig config,
                                             AircraftRequirements reqs,
                                             MissionLegs mission,
                                             double payload_weight)
    : m_config{config}, m_reqs{reqs}, m_mission{mission},
      m_payload_weight{payload_weight} {
  REQUIRE_POSITIVE(m_reqs.design_weight);
  REQUIRE_POSITIVE(m_reqs.R);
  REQUIRE_POSITIVE(m_reqs.v);
  REQUIRE_POSITIVE(m_reqs.ld);
  REQUIRE_POSITIVE(m_reqs.loiter_time);
  REQUIRE_NONZERO(m_mission.num_of_to);
  REQUIRE_NONZERO(m_mission.num_of_climb);
  REQUIRE_NONZERO(m_mission.num_of_cruise);
  REQUIRE_NONZERO(m_mission.num_of_loiter);
  REQUIRE_NONZERO(m_mission.num_of_ldg);
  REQUIRE_POSITIVE(m_payload_weight);
}

double InitialAircraftSizing::compute_empty_weight_frac() const {
  const auto W_0{m_reqs.design_weight};

  const auto A{std::get<0>(
      empty_weight_frac_table[static_cast<size_t>(m_config.aircraft_type)])};
  const auto C{std::get<1>(
      empty_weight_frac_table[static_cast<size_t>(m_config.aircraft_type)])};
  const auto K_vs{m_config.is_swing_wing ? 1.04 : 1.0};

  const auto empty_weight_frac{A * std::pow(W_0, C) * K_vs};

  return empty_weight_frac;
}

double InitialAircraftSizing::compute_fuel_frac() const {
  const auto number_of_takeoffs{m_mission.num_of_to};
  const auto number_of_climbs{m_mission.num_of_climb};
  const auto number_of_cruises{m_mission.num_of_cruise};
  const auto number_of_loiters{m_mission.num_of_loiter};
  const auto number_of_landings{m_mission.num_of_ldg};
  // TODO default back to declaration after other engine types
  // are implemented
  auto ld_cruise{0.0};
  auto ld_loiter{0.0};

  if (m_config.engine_type == EngineType::HighBypassTurbofan ||
      m_config.engine_type == EngineType::LowBypassTurbofan ||
      m_config.engine_type == EngineType::PureTurbojet) {
    ld_cruise = 0.866 * m_reqs.ld;
    ld_loiter = m_reqs.ld;
  }

  const auto C_cruise{
      std::get<0>(fuel_frac_table[static_cast<size_t>(m_config.engine_type)]) /
      3600.0};
  const auto C_loiter{
      std::get<1>(fuel_frac_table[static_cast<size_t>(m_config.engine_type)]) /
      3600.0};

  const auto fuel_frac_to = 0.970 * number_of_takeoffs;
  const auto fuel_frac_climb = 0.985 * number_of_climbs;
  const auto fuel_frac_ldg = 0.995 * number_of_landings;

  const auto fuel_frac_cruise{
      std::exp(-(m_reqs.R * C_cruise) / (m_reqs.v * ld_cruise)) *
      number_of_cruises};

  const auto fuel_frac_loiter{
      std::exp(-(m_reqs.loiter_time * C_loiter) / ld_loiter) *
      number_of_loiters};

  const auto fuel_frac_mission{fuel_frac_to * fuel_frac_climb *
                               fuel_frac_cruise * fuel_frac_loiter *
                               fuel_frac_ldg};
  const auto fuel_frac = 1.06 * (1 - fuel_frac_mission);

  return fuel_frac;
}

double InitialAircraftSizing::compute_initial_weight() {
  constexpr auto tolerance{1e-4};
  constexpr std::size_t max_iterations{20};

  auto err{1.0};
  std::size_t iter{0};
  auto initial_weight{m_reqs.design_weight};

  while (err >= tolerance) {
    const auto empty_weight_frac = compute_empty_weight_frac();
    const auto fuel_frac = compute_fuel_frac();
    const auto denominator = 1.0 - fuel_frac - empty_weight_frac;

    if (!std::isfinite(denominator) || denominator <= 0.0) {
      throw std::domain_error(
          "fuel and empty weight fractions must sum to less than 1.0");
    }

    initial_weight = m_payload_weight / denominator;

    err = std::abs((initial_weight - m_reqs.design_weight) /
                   m_reqs.design_weight);

    m_reqs.design_weight = initial_weight;
    iter += 1;

    if (iter >= max_iterations) {
      throw std::runtime_error("max iterations exceeded");
    }
  }

  return initial_weight;
}

#undef REQUIRE_NONZERO
#undef REQUIRE_POSITIVE
