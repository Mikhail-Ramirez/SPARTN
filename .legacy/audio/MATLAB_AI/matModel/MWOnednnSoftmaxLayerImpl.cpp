#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#else
#include "MWTensorBase.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnSoftmaxLayerImpl.hpp"
 namespace MWOnednnTarget { MWSoftmaxLayerImpl::MWSoftmaxLayerImpl(MWCNNLayer* 
layer, MWTargetNetworkImpl* ntwk_impl) : MWCNNLayerImpl(layer, ntwk_impl) { } 
MWSoftmaxLayerImpl::~MWSoftmaxLayerImpl() { } void 
MWSoftmaxLayerImpl::propagateSize() { clearLayerMapsAndVectors(); MWTensorBase* 
ipTensor = getLayer()->getInputTensor(0); MWTensorBase* opTensor = 
getLayer()->getOutputTensor(0); MWOnednnUtils::reorderInputsTo(this); auto 
prevMemoryDesc = getReorderedIpMemory()->get_desc(); bool isInpMemPrimitiveTNC 
= MWOnednnUtils::checkformatType(prevMemoryDesc, ipTensor, 
dnnl::memory::format_tag::tnc); dnnl::memory::desc layerMemoryDescriptor; if 
(isInpMemPrimitiveTNC) { dnnl::memory::dims layerMemoryDimensions = { 
ipTensor->getSequenceLength() * ipTensor->getBatchSize(), 
ipTensor->getChannels(), ipTensor->getHeight(), ipTensor->getWidth()}; 
layerMemoryDescriptor = dnnl::memory::desc( layerMemoryDimensions, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::nchw); } else { 
layerMemoryDescriptor = getReorderedIpMemory()->get_desc(); } softmax_d = 
std::unique_ptr<dnnl::softmax_forward::desc>(new dnnl::softmax_forward::desc( 
dnnl::prop_kind::forward_inference, layerMemoryDescriptor, 1)); softmax_pd = 
std::unique_ptr<dnnl::softmax_forward::primitive_desc>( new 
dnnl::softmax_forward::primitive_desc(*softmax_d, *eFaDPmxDdzHlRYSAoMmX->eng)); 
if (isInpMemPrimitiveTNC) { assert(opTensor->getHeight() == 1 && 
opTensor->getWidth() == 1); layerMemoryDescriptor = dnnl::memory::desc( 
{ipTensor->getSequenceLength(), ipTensor->getBatchSize(), 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc); } setLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*eFaDPmxDdzHlRYSAoMmX->eng)); softmax = 
std::unique_ptr<dnnl::softmax_forward>(new dnnl::softmax_forward(*softmax_pd)); 
pipeline_memory_args.push_back( {{DNNL_ARG_FROM, *getReorderedIpMemory()}, 
{DNNL_ARG_TO, *getLayerMemory()}}); pipeline.push_back(*softmax); return; } 
void MWSoftmaxLayerImpl::predict() { executeInputsReorderPrimitive(); auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_SFMX_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } } 