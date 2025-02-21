/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_OUTPUT_LAYER_IMPL
#define MW_ONEDNN_OUTPUT_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

class MWOutputLayerImpl final : public MWCNNLayerImpl {
  public:
    MWOutputLayerImpl(MWCNNLayer*, MWTargetNetworkImpl*);
    ~MWOutputLayerImpl();
    void propagateSize() override;
    void predict() override;
};
} // namespace MWOnednnTarget
#endif
