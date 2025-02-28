#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnCustomLayerBase.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
 void MWOnednnUtils::configureReorder(MWOnednnTarget::MWCNNLayerImpl* 
layerImpl, MWTensorBase* srcTensor, format_type dstDataFormat, int index) { 
std::shared_ptr<dnnl::memory> ipLayerMemory = 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensor); dnnl::memory::dims 
layerMemoryDimensions; if (dstDataFormat == format_type::TNC_FORMAT) { 
layerMemoryDimensions = {srcTensor->getSequenceLength(), 
srcTensor->getBatchSize(), srcTensor->getChannels()}; } else { 
layerMemoryDimensions = {srcTensor->getBatchSize() * 
srcTensor->getSequenceLength(), srcTensor->getChannels(), 
srcTensor->getHeight(), srcTensor->getWidth()}; } dnnl::memory::desc 
layerMemoryDescriptor; if (dstDataFormat != format_type::NCWH_FORMAT) { 
dnnl::memory::format_tag formatTag = 
MWOnednnUtils::getOneDNNFormatTag(dstDataFormat); layerMemoryDescriptor = 
dnnl::memory::desc(layerMemoryDimensions, dnnl::memory::data_type::f32, 
formatTag); } else {  assert(dstDataFormat == format_type::NCWH_FORMAT); const 
int size = 4; int outDims_nchw[size]; int strides_nchw[size]; 
MWTensorBase::DIMSLABEL NCWHLayout[size] = {MWTensorBase::N, MWTensorBase::C, 
MWTensorBase::W, MWTensorBase::H}; MWTensorBase::DIMSLABEL srcLayout[size] = 
{MWTensorBase::N, MWTensorBase::C, MWTensorBase::H, MWTensorBase::W}; 
srcTensor->getDimsByLayout(NCWHLayout, size, outDims_nchw); 
MWTensorBase::getTransformStrides(srcLayout, NCWHLayout, outDims_nchw, size, 
strides_nchw); dnnl::memory::dims layerMemoryStrides = {strides_nchw[0], 
strides_nchw[1], strides_nchw[2], strides_nchw[3]}; layerMemoryDescriptor = 
dnnl::memory::desc( layerMemoryDimensions, dnnl::memory::data_type::f32, 
layerMemoryStrides); } layerImpl->setReorderLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, 
*(layerImpl->getTargetImpl())->eng), index); layerImpl->setReorderPrim( 
std::make_shared<dnnl::reorder>(*ipLayerMemory, 
*layerImpl->getReorderedIpMemory(index)), index); } void 
MWOnednnUtils::TargetImplconfigureReorder(MWOnednnTarget::MWTargetNetworkImpl* 
targetImpl, MWTensorBase* srcTensor, dnnl::memory::format_tag dstDataFormat) { 
std::shared_ptr<dnnl::memory> ipLayerMemory = 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensor); dnnl::memory::dims 
layerMemoryDimensions = { srcTensor->getBatchSize() * 
srcTensor->getSequenceLength(), srcTensor->getChannels(), 
srcTensor->getHeight(), srcTensor->getWidth()}; auto layerMemoryDescriptor = 
dnnl::memory::desc(layerMemoryDimensions, dnnl::memory::data_type::f32, 
dstDataFormat); targetImpl->setReorderLayerMemory( 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, *(targetImpl)->eng)); 
targetImpl->setReorderPrim( std::make_shared<dnnl::reorder>(*ipLayerMemory, 
*targetImpl->getReorderLayerMemory())); } format_type 
MWOnednnUtils::getformatType(dnnl::memory::desc inputdesc, MWTensorBase* 
srcTensor) { dnnl::memory::dims layerMemoryDimensionsNC = { 
srcTensor->getSequenceLength() * srcTensor->getBatchSize(), 
srcTensor->getChannels() * srcTensor->getHeight() * srcTensor->getWidth()}; 
dnnl::memory::dims layerMemoryDimensions = { srcTensor->getSequenceLength() * 
srcTensor->getBatchSize(), srcTensor->getChannels(), srcTensor->getHeight(), 
srcTensor->getWidth()}; dnnl::memory::dims layerMemoryDimensionsTNC = { 
srcTensor->getSequenceLength(), srcTensor->getBatchSize(), 
srcTensor->getChannels()}; auto layerMemoryDescriptorNC = dnnl::memory::desc( 
layerMemoryDimensionsNC, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nc); if (inputdesc == layerMemoryDescriptorNC) { 
return format_type::NC_FORMAT; } auto layerMemoryDescriptorNCHW = 
dnnl::memory::desc( layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nchw); if (inputdesc == layerMemoryDescriptorNCHW) { 
return format_type::NCHW_FORMAT; } auto layerMemoryDescriptorNHWC = 
dnnl::memory::desc( layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nhwc); if (inputdesc == layerMemoryDescriptorNHWC) { 
return format_type::NHWC_FORMAT; } auto layerMemoryDescriptorNCHW8C = 
dnnl::memory::desc( layerMemoryDimensions, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nChw8c); if (inputdesc == 
layerMemoryDescriptorNCHW8C) { return format_type::NCHW8C_FORMAT; } auto 
layerMemoryDescriptorNCHW16C = dnnl::memory::desc( layerMemoryDimensions, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::nChw16c); if (inputdesc 
== layerMemoryDescriptorNCHW16C) { return format_type::NCHW16C_FORMAT; } const 
int size = 4; int outDims_nchw[size]; int strides_nchw[size]; 
MWTensorBase::DIMSLABEL NCWHLayout[size] = {MWTensorBase::N, MWTensorBase::C, 
MWTensorBase::W, MWTensorBase::H}; MWTensorBase::DIMSLABEL srcLayout[size] = 
{MWTensorBase::N, MWTensorBase::C, MWTensorBase::H, MWTensorBase::W}; 
srcTensor->getDimsByLayout(NCWHLayout, size, outDims_nchw); 
MWTensorBase::getTransformStrides(srcLayout, NCWHLayout, outDims_nchw, size, 
strides_nchw); dnnl::memory::dims layerMemoryStrides = {strides_nchw[0], 
strides_nchw[1], strides_nchw[2], strides_nchw[3]}; auto 
layerMemoryDescriptorNCWH = dnnl::memory::desc(layerMemoryDimensions, 
dnnl::memory::data_type::f32, layerMemoryStrides); if (inputdesc == 
layerMemoryDescriptorNCWH) { return format_type::NCWH_FORMAT; } 
MWTensorBase::DIMSLABEL HWCNLayout[size] = {MWTensorBase::H, MWTensorBase::W, 
MWTensorBase::C, MWTensorBase::N}; srcTensor->getDimsByLayout(HWCNLayout, size, 
outDims_nchw); MWTensorBase::getTransformStrides(srcLayout, HWCNLayout, 
outDims_nchw, size, strides_nchw); layerMemoryStrides = {strides_nchw[0], 
strides_nchw[1], strides_nchw[2], strides_nchw[3]}; auto 
layerMemoryDescriptorHWCN = dnnl::memory::desc(layerMemoryDimensions, 
dnnl::memory::data_type::f32, layerMemoryStrides); if (inputdesc == 
layerMemoryDescriptorHWCN) { return format_type::HWCN_FORMAT; } auto 
layerMemoryDescriptorTNC = dnnl::memory::desc( layerMemoryDimensionsTNC, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::tnc); if (inputdesc == 
layerMemoryDescriptorTNC) { return format_type::TNC_FORMAT; } return 
format_type::UNKNOWN_FORMAT; } bool 
MWOnednnUtils::checkformatType(dnnl::memory::desc inputdesc, MWTensorBase* 
srcTensor, dnnl::memory::format_tag dstDataFormat) { dnnl::memory::dims 
layerMemoryDimensions = { srcTensor->getSequenceLength() * 
srcTensor->getBatchSize(), srcTensor->getChannels(), srcTensor->getHeight(), 
srcTensor->getWidth()}; if (dstDataFormat == dnnl::memory::format_tag::nc) { 
layerMemoryDimensions = { srcTensor->getSequenceLength() * 
srcTensor->getBatchSize(), srcTensor->getChannels() * srcTensor->getHeight() * 
srcTensor->getWidth()}; } else if (dstDataFormat == 
dnnl::memory::format_tag::tnc) { layerMemoryDimensions = 
{srcTensor->getSequenceLength(), srcTensor->getBatchSize(), 
srcTensor->getChannels()}; } auto layerMemoryDescriptor = 
dnnl::memory::desc(layerMemoryDimensions, dnnl::memory::data_type::f32, 
dstDataFormat); bool isformatMatched = (layerMemoryDescriptor == inputdesc); 
return isformatMatched; } std::shared_ptr<dnnl::memory> 
MWOnednnUtils::getLayerMemoryPrimitive(MWTensorBase* aTensor) { auto 
owningLayer = aTensor->getOwner(); auto owningLayerImpl = 
owningLayer->getImpl(); if (owningLayerImpl == nullptr) { if 
(owningLayer->isCustomLayer()) { return 
dynamic_cast<MWOnednnCustomLayerBase*>(owningLayer) 
->getCustomLayerMemory(aTensor); } else { int inIndex = 
owningLayer->getInPlaceIndex(aTensor->getSourcePortIndex()); bool 
isSingleIpLayer = (owningLayer->getNumInputs() == 1); assert(isSingleIpLayer || 
(inIndex != -1)); if (isSingleIpLayer) { return 
MWOnednnUtils::getLayerMemoryPrimitive(owningLayer->getInputTensor(0)); } else 
{ return 
MWOnednnUtils::getLayerMemoryPrimitive(owningLayer->getInputTensor(inIndex)); } 
} } else { auto layerMemory = 
static_cast<MWOnednnTarget::MWCNNLayerImpl*>(owningLayerImpl) 
->getLayerMemory(aTensor->getSourcePortIndex()); if (layerMemory) { return 
layerMemory; } else { assert(owningLayer->getNumInputs() == 1); return 
MWOnednnUtils::getLayerMemoryPrimitive(owningLayer->getInputTensor(0)); } } } 
bool MWOnednnUtils::isReorderNeeded(format_type formatTag) { return ((formatTag 
!= format_type::NC_FORMAT) && (formatTag != format_type::NCHW_FORMAT) && 
(formatTag != format_type::TNC_FORMAT) && (formatTag != 
format_type::NCHW8C_FORMAT)); } void 
MWOnednnUtils::reorderInputsTo(MWOnednnTarget::MWCNNLayerImpl* layerImpl, bool& 
isMixedIp, format_type formatIpTo) { isMixedIp = areIpsMixed(layerImpl); if 
(isMixedIp) { assert(layerImpl->getLayer()->getOutputTensor(0)->getDataFormat() 
== "CB"); formatIpTo = format_type::NCHW_FORMAT; } if (formatIpTo == 
format_type::OPTIMAL_FORMAT && !isMixedIp) {  bool isAVX512 = 
layerImpl->getTargetImpl()->isAVX512; formatIpTo = isAVX512 ? 
format_type::NCHW16C_FORMAT : format_type::NCHW8C_FORMAT; for (int k = 0; k < 
static_cast<int>(layerImpl->getLayer()->getNumInputs()); k++) { MWTensorBase* 
srcTensork = layerImpl->getLayer()->getInputTensor(static_cast<int>(k)); auto 
prevMemoryDesck = 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensork)->get_desc(); format_type 
ipFormatTag = MWOnednnUtils::getformatType(prevMemoryDesck, srcTensork); 
assert(ipFormatTag != format_type::NC_FORMAT);  if ((isAVX512 && ipFormatTag != 
format_type::NCHW16C_FORMAT) || (!isAVX512 && (ipFormatTag != 
format_type::NCHW8C_FORMAT))) { if (ipFormatTag == format_type::TNC_FORMAT) {  
formatIpTo = format_type::TNC_FORMAT; break; } else if ((isAVX512 && 
srcTensork->getChannels() % 16 <= 8 && srcTensork->getChannels() % 16 != 0) || 
(!isAVX512 && srcTensork->getChannels() % 8 <= 4 && srcTensork->getChannels() % 
8 != 0)) { formatIpTo = format_type::NCHW_FORMAT; break; } } } } for (int k = 
0; k < static_cast<int>(layerImpl->getLayer()->getNumInputs()); k++) { 
MWTensorBase* srcTensork = 
layerImpl->getLayer()->getInputTensor(static_cast<int>(k)); auto 
prevMemoryDesck = 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensork)->get_desc(); format_type 
ipFormatTag = MWOnednnUtils::getformatType(prevMemoryDesck, srcTensork); if 
((ipFormatTag == formatIpTo) || (ipFormatTag == format_type::TNC_FORMAT && 
formatIpTo == format_type::NCHW_FORMAT)) {  
layerImpl->setReorderLayerMemory(MWOnednnUtils::getLayerMemoryPrimitive(srcTensork), 
k); } else { MWOnednnUtils::configureReorder(layerImpl, srcTensork, formatIpTo, 
k); 
layerImpl->getIpReorderPipeline().push_back(*(layerImpl->getReorderPrim(k))); 
layerImpl->getIpReorderArgsPipeline().push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(srcTensork)}, {DNNL_ARG_TO, 
*(layerImpl->getReorderedIpMemory(k))}}); } } } void 
MWOnednnUtils::reorderInputsTo(MWOnednnTarget::MWCNNLayerImpl* layerImpl, 
format_type formatIpTo) { bool isMixedIp = false; reorderInputsTo(layerImpl, 
isMixedIp, formatIpTo); } bool 
MWOnednnUtils::areIpsMixed(MWOnednnTarget::MWCNNLayerImpl* layerImpl) { bool 
isMixedIp = false; MWTensorBase* srcTensork = 
layerImpl->getLayer()->getInputTensor(static_cast<int>(0)); format_type 
ipFormatTag = MWOnednnUtils::getformatType( 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensork)->get_desc(), srcTensork); 
bool isSequenceIp = (ipFormatTag == format_type::TNC_FORMAT); for (int k = 1; k 
< static_cast<int>(layerImpl->getLayer()->getNumInputs()); k++) { srcTensork = 
layerImpl->getLayer()->getInputTensor(static_cast<int>(k)); auto 
prevMemoryDesck = 
MWOnednnUtils::getLayerMemoryPrimitive(srcTensork)->get_desc(); ipFormatTag = 
MWOnednnUtils::getformatType(prevMemoryDesck, srcTensork); if (ipFormatTag == 
format_type::TNC_FORMAT) { if (!isSequenceIp) { 
assert(layerImpl->getLayer()->getOutputTensor(0)->getDataFormat() == "CB"); 
isMixedIp = true; break; } } else { if (isSequenceIp) { 
assert(layerImpl->getLayer()->getOutputTensor(0)->getDataFormat() == "CB"); 
isMixedIp = true; break; } } } return isMixedIp; } dnnl::memory::format_tag 
MWOnednnUtils::getOneDNNFormatTag(format_type formatType) { assert((formatType 
== format_type::NCHW_FORMAT) || (formatType == format_type::NHWC_FORMAT) || 
(formatType == format_type::NCHW8C_FORMAT) || (formatType == 
format_type::NCHW16C_FORMAT) || (formatType == format_type::TNC_FORMAT)); 
switch (formatType) { case format_type::NCHW_FORMAT: return 
dnnl::memory::format_tag::nchw; case format_type::NHWC_FORMAT: return 
dnnl::memory::format_tag::nhwc; case format_type::NCHW8C_FORMAT: return 
dnnl::memory::format_tag::nChw8c; case format_type::NCHW16C_FORMAT: return 
dnnl::memory::format_tag::nChw16c; default: assert(formatType == 
format_type::TNC_FORMAT); return dnnl::memory::format_tag::tnc; } }