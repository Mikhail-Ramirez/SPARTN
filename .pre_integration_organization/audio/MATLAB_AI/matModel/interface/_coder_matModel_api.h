//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_matModel_api.h
//
// Code generation for function 'matModel'
//

#ifndef _CODER_MATMODEL_API_H
#define _CODER_MATMODEL_API_H

// Include files
#include "coder_bounded_array.h"
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Type Definitions
struct cell_wrap_0 {
  coder::bounded_array<char_T, 7U, 2U> f1;
};

struct categorical_tag_0 {
  uint8_T codes;
  cell_wrap_0 categoryNames[2];
};
using categorical = categorical_tag_0;

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void matModel(real32_T y[44100], categorical *prediction);

void matModel_api(const mxArray *prhs, const mxArray **plhs);

void matModel_atexit();

void matModel_initialize();

void matModel_terminate();

void matModel_xil_shutdown();

void matModel_xil_terminate();

#endif
// End of code generation (_coder_matModel_api.h)
