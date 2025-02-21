/* Copyright 2017-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_TARGET_NETWORK_IMPL
#define MW_ONEDNN_TARGET_NETWORK_IMPL

#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTargetNetworkImplBase.hpp"
#include "layer/shared_layers_export_macros.hpp"
#else
#include "MWTargetNetworkImplBase.hpp"
#include "shared_layers_export_macros.hpp"
#endif

#include "dnnl.hpp"

#include <vector>

class MWCNNLayer;
class MWTensorBase;
namespace MWOnednnTarget {

class DLCODER_EXPORT_CLASS MWTargetNetworkImpl final : public MWTargetNetworkImplBase {
    // class used by generated DeepLearningNetwork.cpp/.cu file
  public:
    MWTargetNetworkImpl();
    ~MWTargetNetworkImpl() {
    }
    void allocate(int numBufsToAlloc, MWCNNLayer* layers[], int numLayers, int numPermuteBufs);
    void deallocate() override;
    void preSetup() override;
    void postSetup() {
    }
    void cleanup() override {
    }

    std::vector<float*> memBuffer;
    float* getLayerOutput(MWCNNLayer* layers[], int layerIndex, int portIndex);
    float* getLayerActivation(MWTensorBase*);

    std::unique_ptr<dnnl::engine> eng;
    float* getBufferPtr(int bufferIndex);

    void setReorderLayerMemory(std::shared_ptr<dnnl::memory> other);
    void setReorderPrim(std::shared_ptr<dnnl::reorder> other);
    std::shared_ptr<dnnl::memory> getReorderLayerMemory();
    std::shared_ptr<dnnl::reorder> getReorderPrim();

    float* getPermuteBuffer(int index); // used for input reordering for custom layers

    bool isAVX512;

  private:
    std::shared_ptr<dnnl::memory> reorderLayerMemory;
    std::shared_ptr<dnnl::reorder> reorderPrim; // to handle the reorder
    std::vector<float*> nNULvWnBXnnWdpEkHPAH;
};
} // namespace MWOnednnTarget
#endif
