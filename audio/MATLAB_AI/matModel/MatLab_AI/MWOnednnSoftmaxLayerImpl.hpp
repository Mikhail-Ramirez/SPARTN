/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_SOFTMAX_LAYER_IMPL
#define MW_ONEDNN_SOFTMAX_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"
#include <memory>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

// SoftmaxLayer
class MWSoftmaxLayerImpl final : public MWCNNLayerImpl {
  public:
    MWSoftmaxLayerImpl(MWCNNLayer*, MWTargetNetworkImpl*);
    ~MWSoftmaxLayerImpl();

    void predict() override;
    void propagateSize() override;

  private:
    std::unique_ptr<dnnl::softmax_forward::desc> softmax_d;
    std::unique_ptr<dnnl::softmax_forward::primitive_desc> softmax_pd;
    std::unique_ptr<dnnl::softmax_forward::primitive> softmax;
};
} // namespace MWOnednnTarget
#endif
