//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// matModel.cpp
//
// Code generation for function 'matModel'
//

// Include files
#include "matModel.h"
#include "categorical.h"
#include "dlnetwork.h"
#include "matModel_data.h"
#include "matModel_initialize.h"
#include "matModel_internal_types.h"
#include "matModel_types.h"
#include "melSpectrogram.h"
#include "predict.h"
#include "coder_bounded_array.h"
#include <algorithm>
#include <cmath>

// Function Definitions
void matModel(const float y[44100], coder::categorical *prediction)
{
  static float Y[88200];
  static float b_y[88200];
  static const char cv1[7]{'N', 'o', 'D', 'r', 'o', 'n', 'e'};
  static const char cv[5]{'D', 'r', 'o', 'n', 'e'};
  binDroneModel0_0 netTrained;
  cell_wrap_0 r;
  cell_wrap_0 r1;
  float maxval[2];
  float f;
  float y_tmp;
  int ibtile;
  if (!isInitialized_matModel) {
    matModel_initialize();
  }
  netTrained.matlabCodegenIsDeleted = true;
  coder::internal::dlnetwork_setup(netTrained);
  netTrained.matlabCodegenIsDeleted = false;
  r.f1.size[0] = 1;
  r.f1.size[1] = 5;
  for (ibtile = 0; ibtile < 5; ibtile++) {
    r.f1.data[ibtile] = cv[ibtile];
  }
  r1.f1.size[0] = 1;
  r1.f1.size[1] = 7;
  for (ibtile = 0; ibtile < 7; ibtile++) {
    r1.f1.data[ibtile] = cv1[ibtile];
  }
  prediction->categoryNames[0] = r;
  prediction->categoryNames[1] = r1;
  for (int jtilecol{0}; jtilecol < 2; jtilecol++) {
    ibtile = jtilecol * 44100;
    std::copy(&y[0], &y[44100], &Y[ibtile]);
  }
  // ensure all samples 1 second in length
  //  mid side encoding
  for (ibtile = 0; ibtile < 44100; ibtile++) {
    y_tmp = Y[ibtile + 44100];
    f = Y[ibtile];
    b_y[ibtile] = f + y_tmp;
    b_y[ibtile + 44100] = f - y_tmp;
  }
  std::copy(&b_y[0], &b_y[88200], &Y[0]);
  for (ibtile = 0; ibtile < 2; ibtile++) {
    maxval[ibtile] = Y[44100 * ibtile];
    for (int jtilecol{0}; jtilecol < 44099; jtilecol++) {
      bool p;
      f = Y[(jtilecol + 44100 * ibtile) + 1];
      if (std::isnan(f)) {
        p = false;
      } else {
        y_tmp = maxval[ibtile];
        if (std::isnan(y_tmp)) {
          p = true;
        } else {
          p = (y_tmp < f);
        }
      }
      if (p) {
        maxval[ibtile] = f;
      }
    }
  }
  if ((maxval[0] < maxval[1]) ||
      (std::isnan(maxval[0]) && (!std::isnan(maxval[1])))) {
    y_tmp = maxval[1];
  } else {
    y_tmp = maxval[0];
  }
  for (ibtile = 0; ibtile < 88200; ibtile++) {
    Y[ibtile] /= y_tmp;
  }
  float dlX_Data[21760];
  coder::melSpectrogram(Y, dlX_Data);
  //  Reshape the array to dimensions(Number of bands)-by-(Number of hops)-by-
  //  (Number of channels)-by-(Number of segments). When you feed an image into
  //  a neural network, the first two dimensions are the height and width of
  //  the image, the third dimension is the channels, and the fourth dimension
  //  separates the individual images.
  coder::internal::dlnetwork_predict(netTrained, dlX_Data, maxval);
  if (maxval[0] > maxval[1]) {
    prediction->codes = 1U;
  } else {
    prediction->codes = 2U;
  }
  if (!netTrained.matlabCodegenIsDeleted) {
    netTrained.matlabCodegenIsDeleted = true;
    coder::internal::dlnetwork_delete(netTrained);
  }
}

// End of code generation (matModel.cpp)
