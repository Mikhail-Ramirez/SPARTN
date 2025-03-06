//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// applyFilterBank.h
//
// Code generation for function 'applyFilterBank'
//

#ifndef APPLYFILTERBANK_H
#define APPLYFILTERBANK_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace audio {
namespace internal {
void applyFilterBank(const double filterBank[65664], const float Z[87210],
                     float y[21760]);

}
} // namespace audio
} // namespace coder

#endif
// End of code generation (applyFilterBank.h)
