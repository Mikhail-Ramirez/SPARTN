/* Copyright 2017-2022 The MathWorks, Inc. */


#ifndef MW_ONEDNN_BATCH_NORMALIZATION_LAYER_IMPL
#define MW_ONEDNN_BATCH_NORMALIZATION_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"

#include <memory>
#include <vector>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

/**
 * Codegen class for Batch Normalization Layer
 *
 * This layer performs a simple scale and offset of the input data
 * using previously learned weights together with measured mean and
 * variance over the training data.
 */
class MWBatchNormalizationLayerImpl final : public MWCNNLayerImpl {
  public:
    MWBatchNormalizationLayerImpl(MWCNNLayer*,
                                  MWTargetNetworkImpl*,
                                  float,
                                  const char*,
                                  const char*,
                                  const char*,
                                  const char*,
                                  int);
    ~MWBatchNormalizationLayerImpl();

    void predict() override;
    void propagateSize() override;
    void cleanup() override;

    void setLearnables(std::vector<float*>) override;

  private:
    int enPbWLzEmxYCBmzGJutZ;
    const float REXdEoRjxuQJkqgIDihy;

    // Parameters from training
    float* puSFZkRJmyuFPfQRswDK;
    float* uOjfVTZSbCZATdZVDwrL;
    float* ugnnrhsgTeWucrMPCJUc;


    std::unique_ptr<dnnl::memory> c_src;
    std::unique_ptr<dnnl::memory> c_weights;
    std::unique_ptr<dnnl::memory> c_mean;
    std::unique_ptr<dnnl::memory> c_variance;

    // batch norm related config params
    std::unique_ptr<dnnl::batch_normalization_forward::desc> bnrm_desc;
    std::unique_ptr<dnnl::batch_normalization_forward::primitive_desc> bnrm_prim_desc;
    std::unique_ptr<dnnl::batch_normalization_forward::primitive> bn;

    void loadWeights(const char*, int, float*);
};

} // namespace MWOnednnTarget
#endif
