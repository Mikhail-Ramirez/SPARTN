//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_matModel_info.cpp
//
// Code generation for function 'matModel'
//

// Include files
#include "_coder_matModel_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[5]{
      "789ce554cd4ec24010de1a342446c583be85468c46e4240246a355106a4ca809054628dd"
      "1fd2961f9f428f1e7d05e3e3f8085ebc7b11680b6de3a68d989ae85c"
      "a65fbed9f966673b8384135140082d23cb0a8b965fb271c2f673c86b7e5ef0c509de7034"
      "8f629e730e7f6ffb3aa3260c4c0b5085c0e4648311952ad42cdf7500",
      "e96030dc83c698b955319455022537381f2172e4a22660448dbeb32da86ba52e417acb98"
      "5688dd60d28f2ae7beb1807ef8cddf0f7fdc7fd17bfca69e937f2740"
      "cfe12bd24d362d4b06e886aca9b449e51cab770950d390c54cf92c7328d7549ad3190591"
      "35006f12c574d759e5d4b112b24ebf9fc6c7c7fea3ff2a44a9b7bffa",
      "f21ca59e63bfa537e0e40bfb9fad73f4123e5e552f8a6dcab692e22949e2e25512d7b25a"
      "ceb54f037482ea401c1c55fe07cef9b07ddce0e44ff8f84a3e705e87"
      "136a8f2afab9775e0ba8cfe18f0177401701973a503775d6d41562a0d9f7a8a3bbc0adc3"
      "628c614f304cf59e66d44b73f5bc7c8877f9b235e34742d1cd7f4f8a",
      "769fbebfc50fa2d473ecafefd3ed7c2aab319a52da26a8fdebc2ae7929edfd817dfa0950"
      "1c93ca",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 2920U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[9]{"Version",
                                 "ResolvedFunctions",
                                 "Checksum",
                                 "EntryPoints",
                                 "CoverageInfo",
                                 "IsPolymorphic",
                                 "PropertyList",
                                 "UUID",
                                 "ClassEntryPointIsHandle"};
  const char_T *epFieldName[8]{
      "Name",     "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "FullPath", "TimeStamp",      "Constructor",     "Visible"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 8, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 1);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("matModel"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString("C:\\Users\\kingn\\Documents\\MATLAB\\matModel.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739654.78898148146));
  emlrtSetField(xEntryPoints, 0, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 9, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("23.2.0.2485118 (R2023b) Update 6"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("K6RzSpe1pexIRhJeXzNXH"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

// End of code generation (_coder_matModel_info.cpp)
