/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_MAX_POOLING_LAYER_IMPL
#define MW_ONEDNN_MAX_POOLING_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"
#include <memory>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

class MWMaxPoolingLayerImpl final : public MWCNNLayerImpl {
  public:
    // Create MaxPooling2DLayer with PoolSize = [ PoolH PoolW ]
    //                                Stride = [ StrideH StrideW ]
    //                               Padding = [ PaddingH_T PaddingH_B PaddingW_L PaddingW_R ]
    MWMaxPoolingLayerImpl(MWCNNLayer*,
                          MWTargetNetworkImpl*,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          bool,
                          int);
    ~MWMaxPoolingLayerImpl();


  public:
    void predict() override;
    float* getIndexData();
    void propagateSize() override;

    std::shared_ptr<dnnl::pooling_forward::primitive_desc> getPoolPrimitiveDesc();

    dnnl::memory::dims getPoolKernel();
    dnnl::memory::dims getPoolStrides();
    dnnl::memory::dims getPoolPadTL();
    dnnl::memory::dims getPoolPadBR();


  private:
    /*onednn configurations*/

    float* UzaGmBLFEwmwaFXebUma; // indices output
    int eybNKlJCSDUvsznWynwK;
    bool BRSPqxNffoBYKqpSVHne;

    // these will be -1 in the global max pooling case
    int FeVcBgtQmTLtmnNcJGMY;
    int GeeOVBfQrpMacIFBLKOo;
    
    int FLuSVNoPhAFKtLUchSvv;
    int GFienSVKLlDQuZeqAdLC;
    int GsZlHFuhbvjLtRMDjXnW;
    int HJHXkKmgFxxIOsIvRRnF;
    int CqtPRJvHlGJFssiPzsOm;
    int CpMjJjtGOeWOzwxpAAQP;
    int ECTnqgWHyHCHCLBZlffd;
    int EvebzoroiuKkIxwjkGnD;
    int fSKMHAqIghbYYgyIpNDw;

    dnnl::memory::dims FpguQZSermqZCMRiUfML;
    dnnl::memory::dims GFggoMvRWucDMqzlWzCl;
    dnnl::memory::dims FwLnexHgxHRquTKmNpoa;
    dnnl::memory::dims FrpxvsDMwwgbpqHXWxmN;

    /*onednn primitives to handle the reorder for optimization purpose*/
    std::shared_ptr<dnnl::pooling_forward::desc> pool_d;
    std::shared_ptr<dnnl::pooling_forward::primitive_desc> pool_pd;
    std::shared_ptr<dnnl::pooling_forward::primitive> pool;
};
} // namespace MWOnednnTarget
#endif
