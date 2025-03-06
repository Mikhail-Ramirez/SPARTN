/* Copyright 2020-2022 The MathWorks, Inc. */

#ifndef MW_ONEDNN_AVG_POOLING_LAYER_IMPL
#define MW_ONEDNN_AVG_POOLING_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"
#include <memory>

class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;

// AvgPooling2DLayer
class MWAvgPoolingLayerImpl final : public MWCNNLayerImpl {
  public:
    MWAvgPoolingLayerImpl(MWCNNLayer*,
                          MWTargetNetworkImpl*,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          int,
                          bool);
    ~MWAvgPoolingLayerImpl();

    void predict() override;
    void propagateSize() override;

  private:
    // these will be -1 in the global average pooling case
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
    bool GnxRkpzrPZimKtYYHSuG;

    /*onednn primitives to handle the reorder for optimization purpose*/
    std::unique_ptr<dnnl::pooling_forward::desc> pool_d;
    std::unique_ptr<dnnl::pooling_forward::primitive_desc> pool_pd;
    std::unique_ptr<dnnl::pooling_forward::primitive> pool;
};
} // namespace MWOnednnTarget
#endif
