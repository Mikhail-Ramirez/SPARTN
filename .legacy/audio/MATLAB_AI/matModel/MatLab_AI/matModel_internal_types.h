//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// matModel_internal_types.h
//
// Code generation for function 'matModel'
//

#ifndef MATMODEL_INTERNAL_TYPES_H
#define MATMODEL_INTERNAL_TYPES_H

// Include files
#include "matModel_types.h"
#include "rtwtypes.h"
#include "MWCNNLayer.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
#include "MWTensorBase.hpp"

// Type Definitions
class binDroneModel0_0 {
public:
  binDroneModel0_0();
  void setSize();
  void resetState();
  void setup();
  void activations(int layerIdx);
  void cleanup();
  float *getLayerOutput(int layerIndex, int portIndex);
  int getLayerOutputSize(int layerIndex, int portIndex);
  float *getInputDataPointer(int b_index);
  float *getInputDataPointer();
  float *getOutputDataPointer(int b_index);
  float *getOutputDataPointer();
  int getBatchSize();
  int getOutputSequenceLength(int layerIndex, int portIndex);
  ~binDroneModel0_0();

private:
  void allocate();
  void postsetup();
  void deallocate();

public:
  bool isInitialized;
  bool matlabCodegenIsDeleted;

private:
  int numLayers;
  MWTensorBase *inputTensors[1];
  MWTensorBase *outputTensors[1];
  MWCNNLayer *layers[18];
  MWOnednnTarget::MWTargetNetworkImpl *targetImpl;
};

#endif
// End of code generation (matModel_internal_types.h)
