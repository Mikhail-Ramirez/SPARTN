//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// applyFilterBank.cpp
//
// Code generation for function 'applyFilterBank'
//

// Include files
#include "applyFilterBank.h"
#include <algorithm>
#include <cstring>

// Function Definitions
namespace coder {
namespace audio {
namespace internal {
void applyFilterBank(const double filterBank[65664], const float Z[87210],
                     float y[21760])
{
  static float b_filterBank[65664];
  float temp[128];
  for (int i{0}; i < 65664; i++) {
    b_filterBank[i] = static_cast<float>(filterBank[i]);
  }
  for (int channel{0}; channel < 170; channel++) {
    std::memset(&temp[0], 0, 128U * sizeof(float));
    for (int j{0}; j < 513; j++) {
      float b_Z;
      b_Z = Z[j + 513 * channel];
      for (int i{0}; i < 128; i++) {
        temp[i] += b_Z * b_filterBank[i + (j << 7)];
      }
    }
    std::copy(&temp[0], &temp[128], &y[channel * 128]);
  }
}

} // namespace internal
} // namespace audio
} // namespace coder

// End of code generation (applyFilterBank.cpp)
