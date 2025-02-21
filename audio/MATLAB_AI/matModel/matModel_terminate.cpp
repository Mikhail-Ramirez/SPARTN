//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// matModel_terminate.cpp
//
// Code generation for function 'matModel_terminate'
//

// Include files
#include "matModel_terminate.h"
#include "matModel_data.h"
#include "omp.h"

// Function Definitions
void matModel_terminate()
{
  omp_destroy_nest_lock(&matModel_nestLockGlobal);
  isInitialized_matModel = false;
}

// End of code generation (matModel_terminate.cpp)
