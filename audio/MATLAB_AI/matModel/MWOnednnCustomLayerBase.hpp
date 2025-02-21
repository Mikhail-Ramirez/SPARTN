/* Copyright 2019-2022 The MathWorks, Inc. */

// Target Specific (ONEDNN) header for Custom Layer Base Class
#ifndef MW_ONEDNN_CUSTOM_LAYER_BASE
#define MW_ONEDNN_CUSTOM_LAYER_BASE

#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWCNNLayer.hpp"
#include "layer/shared_layers_export_macros.hpp"
#else
#include "MWCNNLayer.hpp"
#include "shared_layers_export_macros.hpp"
#endif

#include "dnnl.hpp"

#include <map>

namespace MWOnednnTarget {
class MWTargetNetworkImpl;
}
class MWTensorBase;

class DLCODER_EXPORT_CLASS MWOnednnCustomLayerBase : public MWCNNLayer {

  public:
    MWOnednnCustomLayerBase();
    ~MWOnednnCustomLayerBase();

    std::shared_ptr<dnnl::memory> getCustomLayerMemory(MWTensorBase* aTensor);
    int getMaxBufSize();
    void allocate();
    void deallocate();
    void cleanup();

  private:
    std::map<MWTensorBase*, std::shared_ptr<dnnl::memory>> m_customLayerMemory;
    void setCustomLayerMemory(MWTensorBase* aTensor, std::shared_ptr<dnnl::memory> aLayerMemory);
    int maxBufSize = -1;

  protected:
    MWOnednnTarget::MWTargetNetworkImpl* m_ntwkImpl;

    void setupLayer(MWOnednnTarget::MWTargetNetworkImpl*);
    void createLayerMemory(MWTensorBase* aTensor, MWTensorBase::DIMSLABEL implLayout[]);
    void setCustomLayerMemoryData(MWTensorBase* aTensor);
    void layerMemoryReorder(MWTensorBase* srcTensor, int bufIndex);

    virtual void cleanupLayer(){};
};

#endif
