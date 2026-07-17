#include "weights.hpp"

#include <utility>
#include <array>
#include <cstdio>

namespace {
  using EmptyWeightFrac = std::pair<float, float>;

  constexpr std::array<EmptyWeightFrac, 13> empty_weight_frac_table = {
        EmptyWeightFrac(0.86f, -0.05f),
        EmptyWeightFrac(0.91f, -0.05f),
        EmptyWeightFrac(1.19f, -0.09f),
        EmptyWeightFrac(0.99f, -0.09f),
        EmptyWeightFrac(2.36f, -0.18f),
        EmptyWeightFrac(1.51f, -0.10f),
        EmptyWeightFrac(0.74f, -0.03f),
        EmptyWeightFrac(0.96f, -0.05f),
        EmptyWeightFrac(1.09f, -0.05f),
        EmptyWeightFrac(1.59f, -0.10f),
        EmptyWeightFrac(2.34f, -0.13f),
        EmptyWeightFrac(0.93f, -0.07f),
        EmptyWeightFrac(1.02f, -0.06f)
  };
}

float compute_empty_weight_frac(InitialSizingInput &input) {
  auto [A, C] = empty_weight_frac_table[static_cast<size_t>(input.config.aircraft_type)];

  std::printf("A: %f, C: %f\n", A, C);
}
