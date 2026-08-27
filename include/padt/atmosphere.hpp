#pragma once

enum class DayType { StandardDay, Count };

struct Atmosphere {};

Atmosphere compute_mil_std_3013b_atmosphere(float altitude, DayType day_type);
