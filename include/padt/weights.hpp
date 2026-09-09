#pragma once

// Raymer Table 3.1 - Aircraft types
enum class AircraftType {
  SailplaneUnpowered,
  SailplanePowered,
  HomebuiltMetalOrWood,
  HomebuiltComposite,
  GeneralSingleEngine,
  GeneralTwinEngine,
  Agricultural,
  TwinTurboprop,
  FlyingBoat,
  JetTrainer,
  JetFighter,
  MilitaryCargoOrBomber,
  JetTransport,
  UAVTacRecceAndUCAV,
  UAVHighAltitude,
  UAVSmall,
};

// Raymer Tables 3.3 & 3.4
enum class EngineType {
  PureTurbojet,
  LowBypassTurbofan,
  HighBypassTurbofan,
  // PistonPropFixedPitch,
  // PistonPropVariablePitch,
  // Turboprop,
};

// Aircraft type configuration
struct AircraftConfig {
  AircraftType aircraft_type;
  EngineType engine_type;
  bool is_swing_wing;
};

// Aircraft/mission requirements configuration
struct AircraftRequirements {
  double R;             // range [feet]
  double v;             // speed [feet/second]
  double ld;            // lift-to-drag [unitless]
  double loiter_time;   // loiter time [seconds]
  double cruise_alt;    // cruise altitude [feet]
  double design_weight; // design weight [lbs]
};

// Mission leg numbers and types
struct MissionLegs {
  unsigned int num_of_to;     // number of takoffs
  unsigned int num_of_climb;  // number of climbs
  unsigned int num_of_cruise; // number of cruises
  unsigned int num_of_loiter; // number of loiters
  unsigned int num_of_ldg;    // number of landings
};

/**
 * @brief This class contains information and functions related to
 * calculating the initial sizing of an aircraft, including the aircraft
 * configuration, mission requirements, and mission legs
 */
class InitialAircraftSizing {
public:
  InitialAircraftSizing(AircraftConfig config, AircraftRequirements reqs,
                        MissionLegs mission, double payload_weight);

  /**
   * @brief Computes the empty weight fraction
   *
   * @details W_e / W_0 = A * (W_0)^C * K_s
   *
   * Where:
   *
   * A = Constant
   * C = Constant
   * W_0 = Design gross takeoff weight
   * K_vs = Variable sweep constant
   *
   * @note Pre-condition: W_0 is positive
   */
  double compute_empty_weight_frac() const;

  /**
   * @brief Computes the fuel fraction
   *
   * @details W_f = 1.06(1 - W_mission)
   *
   * W_mission = W_to * W_climb * W_cruise * W_loiter * W_ldg
   *
   * Where:
   *
   * W_to = W_climb = W_ldg = const * number of times the phase occurs
   *
   * W_cruise = e^(-RC/v(l/d)) * number of times the phase occurs
   * W_loiter = e^(-EC/(l/d)) * number of times the phase occurs
   *
   * R - Range [ft]
   * C - SFC [1/s]
   * v - airspeed [ft/s]
   * l/d - lift to drag ratio
   * E - loiter time [s]
   *
   * @note Pre-conditions:
   *
   * num_of_to is nonzero
   * num_of_climbs is nonzero
   * num_of_cruises is nonzero
   * num_of_loiters is nonzero
   * num_of_landings is nonzero
   * R is positive
   * v is positive
   * ld is positive
   * loiter_time is positive
   */
  double compute_fuel_frac() const;

  /**
   * @brief Iteratively solves for the initial aircraft weight
   *
   * @details W_0 = W_payload / (1 - W_f / W_0 - W_e / W_0)
   *
   * The estimate starts from the configured design weight and updates that
   * working design weight until the relative change is within tolerance. Throws
   * if the inputs cannot produce a positive finite solution or if the estimate
   * does not converge within the iteration limit.
   *
   * @note Pre-condition: payload_weight is positive
   */
  double compute_initial_weight();

  const AircraftConfig &config() const;
  const AircraftRequirements &reqs() const;
  const MissionLegs &mission() const;
  const double &payload_weight() const;

private:
  AircraftConfig m_config;
  AircraftRequirements m_reqs;
  MissionLegs m_mission;
  double m_payload_weight;
};
