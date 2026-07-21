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
  float v; // speed [feet/second]
  float ld; // lift-to-drag [unitless]
  float loiter_time; // loiter time [seconds]
  float cruise_alt; // cruise altitude [feet]
  float design_weight; // design weight [lbs]
};

// Complete initial sizing input/requirements
struct InitialSizingInput {
  AircraftConfig config;
  AircraftRequirements reqs;
  float payload_weight; // payload weight [lbs]
};

float compute_empty_weight_frac(const InitialSizingInput &input);
