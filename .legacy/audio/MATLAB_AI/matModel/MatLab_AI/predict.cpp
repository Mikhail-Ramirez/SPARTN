//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// predict.cpp
//
// Code generation for function 'predict'
//

// Include files
#include "predict.h"
#include "dlnetwork.h"
#include "matModel_internal_types.h"
#include <cstring>

// Function Definitions
namespace coder {
namespace internal {
void dlnetwork_predict(binDroneModel0_0 &obj,
                       const float varargin_1_Data[21760],
                       float varargout_1_Data[2])
{
  float inputDataT_0_f1[21760];
  for (int p{0}; p < 2; p++) {
    for (int i{0}; i < 128; i++) {
      for (int i1{0}; i1 < 85; i1++) {
        inputDataT_0_f1[(i1 + 85 * i) + 10880 * p] =
            varargin_1_Data[(i + (i1 << 7)) + 10880 * p];
      }
    }
  }
  memcpy(obj.getInputDataPointer(0), inputDataT_0_f1,
         obj.getLayerOutputSize(0, 0));
  obj.activations(17);
  memcpy(varargout_1_Data, obj.getLayerOutput(17, 0),
         obj.getLayerOutputSize(17, 0));
}

} // namespace internal
} // namespace coder

// End of code generation (predict.cpp)
