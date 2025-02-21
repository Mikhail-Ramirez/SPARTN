/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_SOFTMAX_LAYER
#define MW_SOFTMAX_LAYER

#include "MWCNNLayer.hpp"

#include "shared_layers_export_macros.hpp"

class MWTargetNetworkImplBase;
class MWTensorBase;

// SoftmaxLayer
class DLCODER_EXPORT_CLASS MWSoftmaxLayer : public MWCNNLayer {
  public:
    MWSoftmaxLayer() {}
    ~MWSoftmaxLayer() {}

    void createSoftmaxLayer(MWTargetNetworkImplBase*, MWTensorBase*, const char*, int);
    void propagateSize();
};

#endif
