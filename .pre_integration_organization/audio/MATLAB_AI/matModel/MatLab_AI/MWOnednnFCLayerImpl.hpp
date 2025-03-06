/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_FC_LAYER_IMPL
#define MW_ONEDNN_FC_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"

#include <memory>
#include <vector>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

// FullyConnectedLayer
class MWFCLayerImpl final : public MWCNNLayerImpl {

  public:
    MWFCLayerImpl(MWCNNLayer*, MWTargetNetworkImpl*, int, int, const char*, const char*);
    ~MWFCLayerImpl();

    void predict() override;
    void propagateSize() override;
    void cleanup() override;
    void setLearnables(std::vector<float*>) override;

  private:
    void loadWeights(const char*);
    void loadBias(const char*);
    void prepareWeights();

    float* vIWQzNvYZSuxmOTVDFhU;
    float* MdSWZSOAjugbWppryHbR;

    int fxxCPKTclxXPxrdMAkwi; // number of elements to load for weights
    int fXhhiexIRPLyKXApPmmy;    // number of elements to load for bias

    std::shared_ptr<dnnl::memory::desc> weights_md;
    std::shared_ptr<dnnl::memory::desc> bias_md;

    std::shared_ptr<dnnl::memory> weights;
    std::shared_ptr<dnnl::memory> bias;

    std::shared_ptr<dnnl::memory> fc_src_memory;
    std::shared_ptr<dnnl::memory> fc_weights_memory;

    std::unique_ptr<dnnl::reorder> fc_reorder_src;
    std::unique_ptr<dnnl::reorder> fc_reorder_weights;

    std::unique_ptr<dnnl::inner_product_forward::desc> ip_d;
    std::unique_ptr<dnnl::inner_product_forward::primitive_desc> ip_pd;
    std::unique_ptr<dnnl::inner_product_forward::primitive> ip;
};
} // namespace MWOnednnTarget
#endif
