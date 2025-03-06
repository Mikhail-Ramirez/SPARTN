#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#include "layer/MWCNNLayer.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#include "MWCNNLayer.hpp"
#endif
#include "MWOnednnCustomLayerBase.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
#include "MWOnednnUtils.hpp"
#include <cstring>
 MWOnednnCustomLayerBase::MWOnednnCustomLayerBase() : m_ntwkImpl(nullptr) { 
m_isCustomLayer = true; } void 
MWOnednnCustomLayerBase::setupLayer(MWOnednnTarget::MWTargetNetworkImpl* 
targetImpl) { m_ntwkImpl = targetImpl; } void 
MWOnednnCustomLayerBase::cleanup() { this->MWCNNLayer::cleanup(); 
this->cleanupLayer(); } void MWOnednnCustomLayerBase::allocate() { for (size_t 
iOut = 0; iOut < getNumOutputs(); iOut++) { MWTensorBase* opTensor = 
getOutputTensor(iOut); static_cast<MWTensor<float>*>(opTensor)->setData( 
m_ntwkImpl->getBufferPtr(opTensor->getopBufIndex())); 
this->setCustomLayerMemoryData(opTensor); } } void 
MWOnednnCustomLayerBase::deallocate() { for (size_t iOut = 0; iOut < 
getNumOutputs(); iOut++) { 
static_cast<MWTensor<float>*>(getOutputTensor(iOut))->setData((float*)NULL); } 
} void MWOnednnCustomLayerBase::layerMemoryReorder(MWTensorBase* ipTensor, int 
bufIndex) { std::shared_ptr<dnnl::memory> srcLayerImplMemory = 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor); std::shared_ptr<dnnl::memory> 
customLayerMemory = getCustomLayerMemory(ipTensor); format_type ipFormatTag = 
MWOnednnUtils::getformatType(srcLayerImplMemory->get_desc(), ipTensor); 
format_type layerFormatTag = 
MWOnednnUtils::getformatType(customLayerMemory->get_desc(), ipTensor); bool 
reorderTNCToNCHW = ipFormatTag == format_type::TNC_FORMAT && layerFormatTag == 
format_type::NCHW_FORMAT; if ((srcLayerImplMemory->get_desc() != 
customLayerMemory->get_desc()) && !reorderTNCToNCHW) { 
customLayerMemory->set_data_handle(m_ntwkImpl->getPermuteBuffer(bufIndex)); 
auto s = dnnl::stream(*m_ntwkImpl->eng); dnnl::reorder layerMemoryReorderPrim = 
dnnl::reorder(*srcLayerImplMemory, *customLayerMemory); 
layerMemoryReorderPrim.execute(s, *srcLayerImplMemory, *customLayerMemory); } 
else { memcpy(m_ntwkImpl->getPermuteBuffer(bufIndex), 
static_cast<MWTensor<float>*>(ipTensor)->getData(), sizeof(float) * 
ipTensor->getNumElements()); } } void 
MWOnednnCustomLayerBase::createLayerMemory(MWTensorBase* aTensor, 
MWTensorBase::DIMSLABEL implLayout[]) { if (!getCustomLayerMemory(aTensor)) { 
const int size = 4; int outDims[size]; MWTensorBase::DIMSLABEL srcLayout[size] 
= {MWTensorBase::N, MWTensorBase::C, MWTensorBase::H, MWTensorBase::W}; 
aTensor->getDimsByLayout(implLayout, size, outDims); int strides[size]; 
MWTensorBase::getTransformStrides(srcLayout, implLayout, outDims, size, 
strides); dnnl::memory::dims layerMemoryStrides = {strides[0], strides[1], 
strides[2], strides[3]}; dnnl::memory::dims layerMemoryDimensions = 
{aTensor->getBatchSize(), aTensor->getChannels(), aTensor->getHeight(), 
aTensor->getWidth()}; dnnl::memory::desc layerMemoryDescriptor = 
dnnl::memory::desc( layerMemoryDimensions, dnnl::memory::data_type::f32, 
layerMemoryStrides); std::shared_ptr<dnnl::memory> layerMemory = 
std::make_shared<dnnl::memory>(layerMemoryDescriptor, *m_ntwkImpl->eng, 
static_cast<MWTensor<float>*>(aTensor)->getData()); 
setCustomLayerMemory(aTensor, layerMemory); int bufSize = 
(int)(layerMemory->get_desc().get_size() / sizeof(float)); maxBufSize = 
(maxBufSize < bufSize) ? bufSize : maxBufSize; } } 
std::shared_ptr<dnnl::memory> 
MWOnednnCustomLayerBase::getCustomLayerMemory(MWTensorBase* aTensor) { if 
(!m_customLayerMemory.empty()) { std::map<MWTensorBase*, 
std::shared_ptr<dnnl::memory>>::iterator it = 
m_customLayerMemory.find(aTensor); if (it != m_customLayerMemory.end()) { 
return it->second; } } return nullptr; } void 
MWOnednnCustomLayerBase::setCustomLayerMemory(MWTensorBase* aTensor, 
std::shared_ptr<dnnl::memory> aLayerMemory) { if 
(m_customLayerMemory.find(aTensor) == m_customLayerMemory.end()) { 
m_customLayerMemory[aTensor] = aLayerMemory; } else { 
assert(m_customLayerMemory[aTensor] == aLayerMemory); } } void 
MWOnednnCustomLayerBase::setCustomLayerMemoryData(MWTensorBase* aTensor) { 
std::shared_ptr<dnnl::memory> customLayerMemory = 
getCustomLayerMemory(aTensor); 
customLayerMemory->set_data_handle(static_cast<MWTensor<float>*>(aTensor)->getData()); 
} int MWOnednnCustomLayerBase::getMaxBufSize() { return maxBufSize; } 
MWOnednnCustomLayerBase::~MWOnednnCustomLayerBase() { }