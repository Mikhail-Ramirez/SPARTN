//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// matModel_initialize.cpp
//
// Code generation for function 'matModel_initialize'
//

// Include files
#include "matModel_initialize.h"
#include "matModel_data.h"
#include "omp.h"

// Function Definitions
void matModel_initialize()
{
  omp_init_nest_lock(&matModel_nestLockGlobal);
  isInitialized_matModel = true;
}

// End of code generation (matModel_initialize.cpp)
