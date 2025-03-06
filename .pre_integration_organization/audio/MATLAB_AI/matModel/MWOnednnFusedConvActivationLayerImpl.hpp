/* Copyright 2018-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_FUSED_CONV_ACTIVATION_LAYER_IMPL
#define MW_ONEDNN_FUSED_CONV_ACTIVATION_LAYER_IMPL

#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWActivationFunctionType.hpp"
#else
#include "MWActivationFunctionType.hpp"
#endif

#include "MWOnednnCNNLayerImpl.hpp"
#include "MWOnednnUtils.hpp"

#include "dnnl.hpp"

#include <memory>
#include <vector>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

class MWFusedConvActivationLayerImpl final : public MWCNNLayerImpl {
  private:
    float* vIWQzNvYZSuxmOTVDFhU;
    float* MdSWZSOAjugbWppryHbR;
    int AzTsxYcYjIEJsGQbeYHm; // Filter height for CONV and FC
    int BLjrjqvCcCommiXWQLjs;  // Filter width for CONV and FC
    int CGbFsczkgkhjcHoCKzBx;
    int BlRIQPyqJZORKENzSdYf;
    int CDJtexcMbXMWAmnNZsNf;

    int GsZlHFuhbvjLtRMDjXnW;
    int HJHXkKmgFxxIOsIvRRnF;
    int CqtPRJvHlGJFssiPzsOm;
    int CpMjJjtGOeWOzwxpAAQP;
    int ECTnqgWHyHCHCLBZlffd;
    int EvebzoroiuKkIxwjkGnD;
    int AIXLuRgdeiqpaCehGSYD;
    int ATEikvMQPqBefhJzjzhc;
    bool with_bias = true;

    int fxxCPKTclxXPxrdMAkwi;
    int fXhhiexIRPLyKXApPmmy;

    dnnl::primitive_attr nlIRrOJaFuVaywxOqOyb;
    dnnl::post_ops RAtlBpdedvgxUsgDTsch;

    /* @Key - Activation function Enum
           @Value - dnnl algorithm for corresponding activation function
        */
    std::map<MWActivationFunctionType::ACTIVATION_FCN_ENUM, dnnl::algorithm>
        AwZQzUhuWVLGrWgLHRuM{
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, dnnl::algorithm::eltwise_relu},
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::LEAKY_RELU,
             dnnl::algorithm::eltwise_relu},
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::ELU, dnnl::algorithm::eltwise_elu},
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::CLIPPED_RELU,
             dnnl::algorithm::eltwise_bounded_relu},
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::SIGMOID,
             dnnl::algorithm::eltwise_logistic},
            {MWActivationFunctionType::ACTIVATION_FCN_ENUM::TANH, dnnl::algorithm::eltwise_tanh}};

  public:
    MWFusedConvActivationLayerImpl(MWCNNLayer*,
                                   MWTargetNetworkImpl*,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   const char*,
                                   const char*,
                                   double,
                                   MWActivationFunctionType::ACTIVATION_FCN_ENUM);
    ~MWFusedConvActivationLayerImpl();


    void predict() override;
    void propagateSize() override;
    void cleanup() override;
    void setLearnables(std::vector<float*>) override;

  private:
    void loadWeights(const char*);
    void loadBias(const char*);

    // ConvLayer related config params
    std::shared_ptr<dnnl::memory> weights;
    std::shared_ptr<dnnl::memory> bias;

    dnnl::memory::desc reorderedWeightsDesc;
    std::unique_ptr<dnnl::reorder> addition_reorder; // to handle the reorder

    std::unique_ptr<dnnl::memory::desc> conv_src_md;
    std::unique_ptr<dnnl::memory::desc> conv_weights_md;
    std::unique_ptr<dnnl::memory::desc> conv_bias_md;
    std::unique_ptr<dnnl::memory::desc> conv_dst_md;

    /*reorder related settings*/

    std::shared_ptr<dnnl::memory> conv_src_memory;
    std::shared_ptr<dnnl::memory> conv_weights_memory;

    std::unique_ptr<dnnl::reorder> conv_reorder_src;
    std::unique_ptr<dnnl::reorder> conv_reorder_weights;

    std::unique_ptr<dnnl::convolution_forward::desc> conv_d;
    std::unique_ptr<dnnl::convolution_forward::primitive_desc> conv_pd;
    std::unique_ptr<dnnl::convolution_forward::primitive> conv;
};
} // namespace MWOnednnTarget
#endif
