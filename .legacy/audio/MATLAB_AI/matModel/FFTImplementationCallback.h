//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// FFTImplementationCallback.h
//
// Code generation for function 'FFTImplementationCallback'
//

#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
namespace coder {
namespace internal {
namespace fft {
class FFTImplementationCallback {
public:
  static void r2br_r2dit_trig(const float x[174080], creal32_T y[174080]);

protected:
  static void doHalfLengthRadix2(const float x[174080], int xoffInit,
                                 creal32_T y[1024]);
};

} // namespace fft
} // namespace internal
} // namespace coder

#endif
// End of code generation (FFTImplementationCallback.h)
