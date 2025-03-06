/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_INPUT_LAYER_IMPL
#define MW_ONEDNN_INPUT_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

class MWInputLayerImpl final : public MWCNNLayerImpl {

  public:
    MWInputLayerImpl(MWCNNLayer* layer, MWTargetNetworkImpl* ntwk_impl);
    ~MWInputLayerImpl() {
    }
    void predict() override;
    void cleanup() override {
    }
    void propagateSize() override;
};
} // namespace MWOnednnTarget
#endif
