/* Copyright 2020-2022 The MathWorks, Inc. */

#include "MWSoftmaxLayer.hpp"
#include "MWCNNLayerImplBase.hpp"
#include "MWTensorBase.hpp"
#include "MWCNNLayer.hpp"
#include "MWTargetNetworkImplBase.hpp"
#include "MWLayerImplFactory.hpp"

// Create SoftmaxLayer
void MWSoftmaxLayer::createSoftmaxLayer(MWTargetNetworkImplBase* ntwk_impl,
                                        MWTensorBase* m_in,
                                        const char* outFormat,
                                        int outbufIdx) {
    setInputTensor(m_in);
    allocateOutputTensor<float>(-1, -1, -1, -1, -1, NULL, outFormat);

    getOutputTensor(0)->setopBufIndex(outbufIdx);

    MWLayerImplFactory* factory = ntwk_impl->getLayerImplFactory();
    m_impl = factory->createSoftmaxLayerImpl(this, ntwk_impl);
}

void MWSoftmaxLayer::propagateSize() {
    resizeOutputTensor(getInputTensor()->getHeight(), getInputTensor()->getWidth(),
                       getInputTensor()->getChannels(), getInputTensor()->getBatchSize(),
                       getInputTensor()->getSequenceLength());

    m_impl->propagateSize();
}
