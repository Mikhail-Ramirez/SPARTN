#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#include "layer/MWTargetTypes.hpp"
#include "layer/MWOutputLayer.hpp"
#include "layer/MWTargetNetworkImplBase.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#include "MWTargetTypes.hpp"
#include "MWOutputLayer.hpp"
#include "MWTargetNetworkImplBase.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnCustomLayerBase.hpp"
#include "MWOnednnOutputLayerImpl.hpp"
#include "dnnl.hpp"
#ifdef PRECOMPILE_LAYERFILES
#include "MWOnednnLayerImplFactory_precompile.hpp"
#else
#include "MWOnednnLayerImplFactory.hpp"
#endif 
#include <cstring>
#include <vector>
#include <map>
#include <memory>
 namespace MWOnednnTarget { MWTargetNetworkImpl::MWTargetNetworkImpl() : 
MWTargetNetworkImplBase(MWTargetType::ONEDNN_TARGET, new 
MWOnednnLayerImplFactory) { } void MWTargetNetworkImpl::allocate(int 
numBufsToAlloc, MWCNNLayer* layers[], int numLayers, int numPermuteBufs) { 
numBufs = numBufsToAlloc; int maxCustomLayerBufSize = -1; for (int i = 0; i < 
numLayers; i++) { if (layers[i]->isCustomLayer()) { int layerBufSize = 
static_cast<MWOnednnCustomLayerBase*>(layers[i])->getMaxBufSize(); 
maxCustomLayerBufSize = (maxCustomLayerBufSize < layerBufSize) ? layerBufSize : 
maxCustomLayerBufSize; } } nNULvWnBXnnWdpEkHPAH.reserve(numPermuteBufs); if 
(maxCustomLayerBufSize > 0 && numPermuteBufs > 0) { for (int i = 0; i < 
numBufs; i++) { nNULvWnBXnnWdpEkHPAH.push_back(new 
float[maxCustomLayerBufSize]); } } int maxBufSize = maxCustomLayerBufSize; for 
(int i = 0; i < numLayers; i++) { if ((layers[i]->getImpl() != nullptr) && 
(dynamic_cast<MWOutputLayer*>(layers[i]) == nullptr)) { MWCNNLayerImpl* 
layerImpl = static_cast<MWCNNLayerImpl*>(layers[i]->getImpl()); int 
layerBufSize = (int)(layerImpl->getLayerMemory()->get_desc().get_size() / 
sizeof(float)); maxBufSize = (maxBufSize < layerBufSize) ? layerBufSize : 
maxBufSize; } } memBuffer.reserve(numBufs); for (int i = 0; i < numBufs; i++) { 
memBuffer.push_back(new float[maxBufSize]); } } void 
MWTargetNetworkImpl::setReorderPrim(std::shared_ptr<dnnl::reorder> other) { 
reorderPrim = other; } void 
MWTargetNetworkImpl::setReorderLayerMemory(std::shared_ptr<dnnl::memory> other) 
{ reorderLayerMemory = other; } void MWTargetNetworkImpl::preSetup() { eng = 
std::unique_ptr<dnnl::engine>(new dnnl::engine(dnnl::engine::kind::cpu, 0)); 
Cpu cpu; isAVX512 = cpu.has(Cpu::tAVX512F); } float* 
MWTargetNetworkImpl::getLayerOutput(MWCNNLayer* layers[], int layerIndex, int 
portIndex) { MWTensorBase* opTensor = 
layers[layerIndex]->getOutputTensor(portIndex); float* opData = 
getLayerActivation(opTensor); return opData; } std::shared_ptr<dnnl::memory> 
MWTargetNetworkImpl::getReorderLayerMemory() { return reorderLayerMemory; } 
std::shared_ptr<dnnl::reorder> MWTargetNetworkImpl::getReorderPrim() { return 
reorderPrim; } float* MWTargetNetworkImpl::getLayerActivation(MWTensorBase* 
opTensorBase) { MWTensor<float>* opTensor = 
static_cast<MWTensor<float>*>(opTensorBase); MWCNNLayer* layer = 
opTensor->getOwner(); MWCNNLayerImplBase* layerImpl = layer->getImpl(); if 
(layerImpl == nullptr && !layer->isCustomLayer()) { return 
getLayerActivation(opTensor->getOwner()->getInputTensor()); } else { if 
(dynamic_cast<MWOutputLayerImpl*>(layerImpl)) { return opTensor->getData(); } 
else { std::shared_ptr<dnnl::memory> currentLayerMemory = 
MWOnednnUtils::getLayerMemoryPrimitive(opTensor); auto currentLayerMemoryDesc = 
currentLayerMemory->get_desc(); format_type formatTag = 
MWOnednnUtils::getformatType(currentLayerMemoryDesc, opTensor); if 
(MWOnednnUtils::isReorderNeeded(formatTag) || (formatTag == 
format_type::NCHW8C_FORMAT)) { MWOnednnUtils::TargetImplconfigureReorder(this, 
opTensor, dnnl::memory::format_tag::nchw); std::vector<dnnl::primitive> 
activationsPipeline; std::vector<std::unordered_map<int, dnnl::memory>> 
activationsPipeline_memory_args; 
activationsPipeline.push_back(*this->getReorderPrim()); auto s = 
dnnl::stream(*eng); activationsPipeline_memory_args.push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(opTensor)}, {DNNL_ARG_TO, 
*this->getReorderLayerMemory()}}); assert(activationsPipeline.size() == 
activationsPipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(activationsPipeline.size()); i++) { 
activationsPipeline[i].execute(s, activationsPipeline_memory_args[i]); } float* 
activationsOutput = 
static_cast<float*>(this->getReorderLayerMemory()->get_data_handle()); return 
activationsOutput; } return opTensor->getData(); } } } void 
MWTargetNetworkImpl::deallocate() { for (int i = 0; i < 
static_cast<int>(memBuffer.size()); i++) { if (memBuffer[i] != nullptr) { 
delete[] memBuffer[i]; } } memBuffer.clear(); for (int i = 0; i < 
static_cast<int>(nNULvWnBXnnWdpEkHPAH.size()); i++) { if 
(nNULvWnBXnnWdpEkHPAH[i] != nullptr) { delete[] 
nNULvWnBXnnWdpEkHPAH[i]; } } nNULvWnBXnnWdpEkHPAH.clear(); } float* 
MWTargetNetworkImpl::getBufferPtr(int bufferIndex) { assert(bufferIndex < 
numBufs); return memBuffer[bufferIndex]; } float* 
MWTargetNetworkImpl::getPermuteBuffer(int bufferIndex) { assert(bufferIndex < 
static_cast<int>(nNULvWnBXnnWdpEkHPAH.size())); return 
nNULvWnBXnnWdpEkHPAH[bufferIndex]; } } 