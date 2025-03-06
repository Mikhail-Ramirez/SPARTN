//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_matModel_api.cpp
//
// Code generation for function 'matModel'
//

// Include files
#include "_coder_matModel_api.h"
#include "_coder_matModel_mex.h"
#include "coder_bounded_array.h"

// Variable Definitions
emlrtCTX emlrtRootTLSGlobal{nullptr};

emlrtContext emlrtContextGlobal{
    true,                                                 // bFirstTime
    false,                                                // bInitialized
    131643U,                                              // fVersionInfo
    nullptr,                                              // fErrorFunction
    "matModel",                                           // fFunctionName
    nullptr,                                              // fRTCallStack
    false,                                                // bDebugMode
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, // fSigWrd
    nullptr                                               // fSigMem
};

// Function Declarations
static real32_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                     const emlrtMsgIdentifier *msgId))[44100];

static real32_T (*emlrt_marshallIn(const emlrtStack &sp,
                                   const mxArray *b_nullptr,
                                   const char_T *identifier))[44100];

static real32_T (*emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[44100];

static const mxArray *emlrt_marshallOut(const emlrtStack &sp,
                                        const categorical u);

// Function Definitions
static real32_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                     const emlrtMsgIdentifier *msgId))[44100]
{
  static const int32_T dims{44100};
  int32_T i;
  real32_T(*ret)[44100];
  boolean_T b{false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "single", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (real32_T(*)[44100])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static real32_T (*emlrt_marshallIn(const emlrtStack &sp,
                                   const mxArray *b_nullptr,
                                   const char_T *identifier))[44100]
{
  emlrtMsgIdentifier thisId;
  real32_T(*y)[44100];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = emlrt_marshallIn(sp, emlrtAlias(b_nullptr), &thisId);
  emlrtDestroyArray(&b_nullptr);
  return y;
}

static real32_T (*emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[44100]
{
  real32_T(*y)[44100];
  y = b_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *emlrt_marshallOut(const emlrtStack &sp,
                                        const categorical u)
{
  const mxArray *propValues[5];
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *e_y;
  const mxArray *f_y;
  const mxArray *g_y;
  const mxArray *m;
  const mxArray *m1;
  const mxArray *m2;
  const mxArray *m3;
  const mxArray *m4;
  const mxArray *m5;
  const mxArray *y;
  int32_T iv[2];
  int32_T i;
  const char_T *propClasses[5]{
      "matlab.internal.coder.categorical", "matlab.internal.coder.categorical",
      "matlab.internal.coder.categorical", "matlab.internal.coder.categorical",
      "matlab.internal.coder.categorical"};
  const char_T *propNames[5]{"codes", "categoryNames", "isProtected",
                             "isOrdinal", "numCategoriesUpperBound"};
  y = nullptr;
  m = nullptr;
  m1 = nullptr;
  m2 = nullptr;
  m3 = nullptr;
  m4 = nullptr;
  emlrtAssign(&y, emlrtCreateClassInstance2022a(
                      (emlrtCTX)&sp, "matlab.internal.coder.categorical"));
  m = nullptr;
  b_y = nullptr;
  m5 = emlrtCreateNumericMatrix(1, 1, mxUINT8_CLASS, mxREAL);
  *static_cast<uint8_T *>(emlrtMxGetData(m5)) = u.codes;
  emlrtAssign(&b_y, m5);
  emlrtAssign(&m, b_y);
  propValues[0] = m;
  m1 = nullptr;
  c_y = nullptr;
  i = 2;
  emlrtAssign(&c_y, emlrtCreateCellArrayR2014a(1, &i));
  d_y = nullptr;
  iv[0] = 1;
  iv[1] = u.categoryNames[0].f1.size[1];
  m5 = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)&sp, u.categoryNames[0].f1.size[1],
                           m5, &u.categoryNames[0].f1.data[0]);
  emlrtAssign(&d_y, m5);
  emlrtSetCell(c_y, 0, d_y);
  d_y = nullptr;
  iv[0] = 1;
  iv[1] = u.categoryNames[1].f1.size[1];
  m5 = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)&sp, u.categoryNames[1].f1.size[1],
                           m5, &u.categoryNames[1].f1.data[0]);
  emlrtAssign(&d_y, m5);
  emlrtSetCell(c_y, 1, d_y);
  emlrtAssign(&m1, c_y);
  propValues[1] = m1;
  m2 = nullptr;
  e_y = nullptr;
  m5 = emlrtCreateLogicalScalar(false);
  emlrtAssign(&e_y, m5);
  emlrtAssign(&m2, e_y);
  propValues[2] = m2;
  m3 = nullptr;
  f_y = nullptr;
  m5 = emlrtCreateLogicalScalar(false);
  emlrtAssign(&f_y, m5);
  emlrtAssign(&m3, f_y);
  propValues[3] = m3;
  m4 = nullptr;
  g_y = nullptr;
  m5 = emlrtCreateDoubleScalar(2.0);
  emlrtAssign(&g_y, m5);
  emlrtAssign(&m4, g_y);
  propValues[4] = m4;
  emlrtSetAllProperties((emlrtCTX)&sp, &y, 0, 5, (const char_T **)&propNames[0],
                        (const char_T **)&propClasses[0], &propValues[0]);
  emlrtAssign(&y,
              emlrtConvertInstanceToRedirectSource(
                  (emlrtCTX)&sp, y, 0, "matlab.internal.coder.categorical"));
  return y;
}

void matModel_api(const mxArray *prhs, const mxArray **plhs)
{
  categorical prediction;
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  real32_T(*y)[44100];
  st.tls = emlrtRootTLSGlobal;
  // Marshall function inputs
  y = emlrt_marshallIn(st, emlrtAlias(prhs), "y");
  // Invoke the target function
  matModel(*y, &prediction);
  // Marshall function outputs
  *plhs = emlrt_marshallOut(st, prediction);
}

void matModel_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  matModel_xil_terminate();
  matModel_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void matModel_initialize()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void matModel_terminate()
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

// End of code generation (_coder_matModel_api.cpp)
