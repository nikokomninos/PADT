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
  Count // hack to get enum class length for array
};

// Raymer Tables 3.3 & 3.4
enum class EngineType {
  PureTurbojet,
  LowBypassTurbofan,
  HighBypassTurbofan,
  // PistonPropFixedPitch,
  // PistonPropVariablePitch,
  // Turboprop,
  Count
};

// Aircraft type configuration
struct AircraftConfig {
  AircraftType aircraft_type;
  bool is_swing_wing;
};

// Aircraft/mission requirements configuration
struct AircraftRequirements {
  float R; // range [feet]
  EngineType engine_type;
  float v;             // speed [feet/second]
  float ld;            // lift-to-drag [unitless]
  float loiter_time;   // loiter time [seconds]
  float cruise_alt;    // cruise altitude [feet]
  float design_weight; // design weight [lbs]
};

// Mission leg numbers and types
struct MissionLegs {
  unsigned int num_of_to;     // number of takoffs
  unsigned int num_of_climb;  // number of climbs
  unsigned int num_of_cruise; // number of cruises
  unsigned int num_of_loiter; // number of loiters
  unsigned int num_of_ldg;    // number of landings
};

class InitialAircraftSizing {
public:
  InitialAircraftSizing(AircraftConfig config, AircraftRequirements reqs,
                        MissionLegs mission, float payload_weight);
  float compute_empty_weight_frac() const;
  float compute_fuel_frac() const;
  float compute_initial_weight();

  const AircraftConfig &config() const;
  const AircraftRequirements &reqs() const;
  const MissionLegs &mission() const;
  const float &payload_weight() const;

private:
  AircraftConfig m_config;
  AircraftRequirements m_reqs;
  MissionLegs m_mission;
  float m_payload_weight;
};
