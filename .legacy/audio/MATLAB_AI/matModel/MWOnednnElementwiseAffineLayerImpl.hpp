/* Copyright 2019-2022 The MathWorks, Inc. */
#ifndef MW_ONEDNN_ELEMENTWISE_AFFINE_LAYER_IMPL
#define MW_ONEDNN_ELEMENTWISE_AFFINE_LAYER_IMPL

#include "MWOnednnCNNLayerImpl.hpp"
#include "dnnl.hpp"
#include <memory>

/**
 *  Codegen class for Scaling Factor layer
 **/
class MWCNNLayer;
namespace MWOnednnTarget {
class MWTargetNetworkImpl;
class MWElementwiseAffineLayerImpl final : public MWCNNLayerImpl {
  public:
    MWElementwiseAffineLayerImpl(MWCNNLayer* layer,
                                 MWTargetNetworkImpl* ntwk_impl,
                                 int scale_H,
                                 int scale_W,
                                 int scale_C,
                                 int offset_H,
                                 int offset_W,
                                 int offset_C,
                                 bool isClipped,
                                 int lowerbound,
                                 int upperbound,
                                 const char* scale_file,
                                 const char* offsetfile);
    ~MWElementwiseAffineLayerImpl();

    void predict() override;
    void cleanup() override;
    void propagateSize() override;

  private:
    std::shared_ptr<dnnl::memory> intermediateOutput;

    void loadScale(const char*);
    void loadOffset(const char*);
    float* puSFZkRJmyuFPfQRswDK;
    float* gzSTokDHvkXefhiGDcWL;
    std::shared_ptr<float> intermediateOp;
    double rSmEWccbJFfPGddhPemm;
    double shEncNmxJsMuJKwbrwok;
    double qWwjVYwfnvEnFKlgpqwA;
    double hnewnpwgzKmOdualajhn;
    double jLyhrFjMmVnNjoeDJCwH;
    double hljcfGWsvZXJZNrImpJB;
    bool ZinudJuZuGitiNTsJpBR;
    int dJcdBfQQLhIAYHPxwQeg;
    int vFNECEAeLZsYsUxvlgqL;

    /* if the scale and offset are both scalar values then use this primitive*/
    std::unique_ptr<dnnl::eltwise_forward::desc> scaling_d;
    std::unique_ptr<dnnl::eltwise_forward::primitive_desc> scaling_pd;
    std::unique_ptr<dnnl::eltwise_forward::primitive> scaling;


    void clipAffineOutput();
    void channelWiseOffsetAdd();
    void createScalarAffine();
    void createVectorAffine();
};


} // namespace MWOnednnTarget
#endif
