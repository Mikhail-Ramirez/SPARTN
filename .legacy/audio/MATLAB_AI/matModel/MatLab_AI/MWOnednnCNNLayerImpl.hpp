/* Copyright 2017-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_CNN_LAYER_IMPL
#define MW_ONEDNN_CNN_LAYER_IMPL

#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWCNNLayerImplBase.hpp"
#else
#include "MWCNNLayerImplBase.hpp"
#endif

#include "dnnl.hpp"

#include <cstring>
#include <map>
#include <vector>
#include <unordered_map>
#include "dnnl.hpp"

class MWCNNLayer;

namespace MWOnednnTarget {
class MWTargetNetworkImpl;

#if !((DNNL_VERSION_MAJOR == 1) && (DNNL_VERSION_MINOR == 4) && (DNNL_VERSION_PATCH == 0))
#error This version of ONE-DNN library is not supported for deep learning code generation. Use version 1.4.0 instead.
#endif

/*If MW_LAYERS_TAP is enabled, it will tap layer wise output */
#define MW_LAYERS_TAP 0

#if MW_LAYERS_TAP
extern int tap_count;
extern void mw_interm_tap(float* inp, int size, int count);



#define MW_INPUT_TAP 1
#define MW_CONV_TAP 1
#define MW_RELU_TAP 1
#define MW_NORM_TAP 1
#define MW_POOL_TAP 1
#define MW_UNPOOL_TAP 1
#define MW_FC_TAP 1
#define MW_SFMX_TAP 1
#define MW_ADDITION_TAP 1
#define MW_CONCAT_TAP 1
#define MW_TRANSPOSEDCONV_TAP 1
#define MW_SCALING_TAP 1
#define MW_CROP2D_TAP 1
#define MW_YOLOREORG_TAP 1
#define MW_YOLOEXTRACT_TAP 1
#define MW_SIGMOID_TAP 1
#define MW_EXP_TAP 1
#define MW_YOLOSFMAX_TAP 1
#define MW_FLATTEN_CSTYLE_TAP 1
#define MW_TANH_TAP 1
#define MW_ZEROPAD_TAP 1
#define MW_FLATTEN_TAP 1
#define MW_SPLIT_LAYER 1
#define MW_AFFINE_TAP 1
#define MW_SSDMERGE_TAP 1
#define MW_ELU_TAP 1
#define MW_LSTM_TAP 1
#define MW_SEQIP_TAP 1
#define MW_FLATTEN_TAP 1
#else
#define MW_INPUT_TAP 0
#define MW_CONV_TAP 0
#define MW_RELU_TAP 0
#define MW_NORM_TAP 0
#define MW_POOL_TAP 0
#define MW_UNPOOL_TAP 0
#define MW_FC_TAP 0
#define MW_SFMX_TAP 0
#define MW_ADDITION_TAP 0
#define MW_CONCAT_TAP 0
#define MW_TRANSPOSEDCONV_TAP 0
#define MW_SCALING_TAP 0
#define MW_CROP2D_TAP 0
#define MW_YOLOREORG_TAP 0
#define MW_YOLOEXTRACT_TAP 0
#define MW_SIGMOID_TAP 0
#define MW_EXP_TAP 0
#define MW_YOLOSFMAX_TAP 0
#define MW_FLATTEN_CSTYLE_TAP 0
#define MW_TANH_TAP 0
#define MW_ZEROPAD_TAP 0
#define MW_FLATTEN_TAP 0
#define MW_SPLIT_LAYER 0
#define MW_AFFINE_TAP 0
#define MW_SSDMERGE_TAP 0
#define MW_ELU_TAP 0
#define MW_LSTM_TAP 0
#define MW_SEQIP_TAP 0
#define MW_FLATTEN_TAP 0
#endif

class MWCNNLayerImpl : public MWCNNLayerImplBase {
  public:
    MWCNNLayerImpl(MWCNNLayer* layer, MWTargetNetworkImpl* ntwk_impl);
    virtual ~MWCNNLayerImpl() {
    }
    void predict() override {
    }
    void cleanup() override {
    }
    void postSetup() override final {
    }
    void propagateSize() override = 0;
    void allocate() override {
    }
    void deallocate() override {
    }

    void allocateOutput(int) override final;
    template <class T>
    void allocateOutputImpl(int);

    void deallocateOutput(int) override final;
    template <class T>
    void deallocateOutputImpl(int);

    void allocateInput(int) override final {
    }
    void deallocateInput(int) override final {
    }

    void setLearnables(std::vector<float*>) override {
    }

    void resetState() override {
    }
    void updateState() override {
    }

  public:
    std::shared_ptr<dnnl::memory> getLayerMemory(int index = 0);
    void setLayerMemory(std::shared_ptr<dnnl::memory> other, int index = 0);

    std::shared_ptr<dnnl::memory> getReorderedIpMemory(int index = 0);

    std::shared_ptr<dnnl::reorder> getReorderPrim(int index = 0);

    MWTargetNetworkImpl* getTargetImpl() {
        return eFaDPmxDdzHlRYSAoMmX;
    }
    void setReorderLayerMemory(std::shared_ptr<dnnl::memory> other, int index = 0);
    void setReorderPrim(std::shared_ptr<dnnl::reorder> other, int index = 0);
    void reorderedLayerOutputTap(int);
    void clearLayerMapsAndVectors();
    void executeWeightsReorderPrimitive();
    void executeInputsReorderPrimitive();

    std::vector<dnnl::primitive>& getPipeline() {
        return pipeline;
    }
    std::vector<std::unordered_map<int, dnnl::memory>>& getArgumentsPipeline() {
        return pipeline_memory_args;
    }

    std::vector<dnnl::primitive>& getIpReorderPipeline() {
        return pipeline_ipReorder;
    }
    std::vector<std::unordered_map<int, dnnl::memory>>& getIpReorderArgsPipeline() {
        return pipeline_ipReorder_memory_args;
    }

  protected:
    MWTargetNetworkImpl* eFaDPmxDdzHlRYSAoMmX;

    /*onednn related config params*/
    std::map<int, std::shared_ptr<dnnl::memory>> layerMemory;
    std::map<int, std::shared_ptr<dnnl::memory>> reorderLayerMemory;
    std::map<int, std::shared_ptr<dnnl::reorder>> reorderPrim; // to handle the reorder

    std::vector<dnnl::primitive> pipeline;

    std::vector<std::unordered_map<int, dnnl::memory>> pipeline_memory_args;
    std::vector<std::unordered_map<int, dnnl::memory>> pipeline_weights_memory_args;
    std::vector<dnnl::primitive> pipeline_weights;
    std::vector<std::unordered_map<int, dnnl::memory>> pipeline_ipReorder_memory_args;
    std::vector<dnnl::primitive> pipeline_ipReorder;
};
} // namespace MWOnednnTarget
#endif
