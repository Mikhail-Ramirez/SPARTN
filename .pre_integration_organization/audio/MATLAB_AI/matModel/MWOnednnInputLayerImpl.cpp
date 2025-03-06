#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWCNNLayer.hpp"
#include "layer/MWTensorBase.hpp"
#else
#include "MWCNNLayer.hpp"
#include "MWTensorBase.hpp"
#endif
#include "MWOnednnInputLayerImpl.hpp"
#include "MWOnednnCNNLayerImpl.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
#include "dnnl.hpp"
#include <memory>
 namespace MWOnednnTarget { MWInputLayerImpl::MWInputLayerImpl(MWCNNLayer* 
layer, MWTargetNetworkImpl* ntwk_impl) : MWCNNLayerImpl(layer, ntwk_impl) { } 
void MWInputLayerImpl::propagateSize() { MWTensorBase* ipTensor = 
getLayer()->getInputTensor(0); dnnl::memory::dims layerMemoryDimensions = 
{ipTensor->getBatchSize(), ipTensor->getChannels(), ipTensor->getHeight(), 
ipTensor->getWidth()}; auto layerMemoryDescriptor = dnnl::memory::desc( 
layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nchw); setLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*eFaDPmxDdzHlRYSAoMmX->eng)); return; } void MWInputLayerImpl::predict() {
#if MW_INPUT_TAP
 reorderedLayerOutputTap(0);
#endif
 } } 