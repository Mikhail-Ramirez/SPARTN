#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include <memory>
#include <cassert>
#include <cstring>
#include <cstdio>
 namespace MWOnednnTarget { MWCNNLayerImpl::MWCNNLayerImpl(MWCNNLayer* layer, 
MWTargetNetworkImpl* ntwk_impl) : MWCNNLayerImplBase(layer) , 
eFaDPmxDdzHlRYSAoMmX(ntwk_impl) { } std::shared_ptr<dnnl::memory> 
MWCNNLayerImpl::getLayerMemory(int index) { if (layerMemory.size() >= 1) { 
std::map<int, std::shared_ptr<dnnl::memory>>::iterator it = 
layerMemory.find(index); return it->second; } else { return nullptr; } } 
std::shared_ptr<dnnl::memory> MWCNNLayerImpl::getReorderedIpMemory(int index) { 
if (reorderLayerMemory.size() >= 1) { std::map<int, 
std::shared_ptr<dnnl::memory>>::iterator it = reorderLayerMemory.find(index); 
return it->second; } else { return nullptr; } } std::shared_ptr<dnnl::reorder> 
MWCNNLayerImpl::getReorderPrim(int index) { if (reorderPrim.size() >= 1) { 
std::map<int, std::shared_ptr<dnnl::reorder>>::iterator it = 
reorderPrim.find(index); return it->second; } else { return nullptr; } } void 
MWCNNLayerImpl::setLayerMemory(std::shared_ptr<dnnl::memory> other, int index) 
{ layerMemory[index] = other; } void 
MWCNNLayerImpl::setReorderPrim(std::shared_ptr<dnnl::reorder> other, int index) 
{ reorderPrim[index] = other; } void 
MWCNNLayerImpl::setReorderLayerMemory(std::shared_ptr<dnnl::memory> other, int 
index) { reorderLayerMemory[index] = other; } void 
MWCNNLayerImpl::allocateOutput(int outIdx) { if 
(getLayer()->getOutputTensor(outIdx)->isFloat()) { 
this->allocateOutputImpl<float>(outIdx); } else { 
assert(getLayer()->getOutputTensor(outIdx)->isInt8()); 
this->allocateOutputImpl<signed char>(outIdx); } } template <class T> void 
MWCNNLayerImpl::allocateOutputImpl(int outIdx) { MWTensorBase* opTensorBase = 
getLayer()->getOutputTensor(outIdx); MWTensor<T>* opTensor = 
static_cast<MWTensor<T>*>(opTensorBase); int outBufIndex = 
opTensor->getopBufIndex(); bool bufferReuse = outBufIndex >= 0; int inIdx = 
getLayer()->getInPlaceIndex(outIdx); bool inPlace = inIdx != -1; if 
(!bufferReuse) {  if (inPlace) {  
opTensor->setData((T*)getReorderedIpMemory(inIdx)->get_data_handle()); } else { 
 int eqOmMKQRpqBqRQCnJmxt = 
static_cast<int>(this->getLayerMemory()->get_desc().get_size() / sizeof(T)); 
opTensor->setData((T*)calloc(eqOmMKQRpqBqRQCnJmxt, sizeof(T))); } } else {  
assert(opTensorBase->isFloat()); assert(!inPlace); 
static_cast<MWTensor<float>*>(opTensorBase) 
->setData(eFaDPmxDdzHlRYSAoMmX->getBufferPtr(outBufIndex)); } 
MWOnednnUtils::getLayerMemoryPrimitive(opTensor)->set_data_handle(opTensor->getData()); 
} template void MWCNNLayerImpl::allocateOutputImpl<float>(int); template void 
MWCNNLayerImpl::allocateOutputImpl<signed char>(int); void 
MWCNNLayerImpl::deallocateOutput(int outIdx) { if 
(getLayer()->getOutputTensor(outIdx)->isFloat()) { 
this->deallocateOutputImpl<float>(outIdx); } else { 
assert(getLayer()->getOutputTensor(outIdx)->isInt8()); 
this->deallocateOutputImpl<signed char>(outIdx); } } template <class T> void 
MWCNNLayerImpl::deallocateOutputImpl(int outIdx) { MWTensorBase* opTensorBase = 
getLayer()->getOutputTensor(outIdx); MWTensor<T>* opTensor = 
static_cast<MWTensor<T>*>(opTensorBase); bool doBufReleaseFlag = 
opTensor->getopBufIndex() < 0 && getLayer()->getInPlaceIndex(outIdx) == -1; T* 
opDataBuf = opTensor->getData(); if (opDataBuf) { if (doBufReleaseFlag) { 
free(opDataBuf); } opTensor->setData((T*)nullptr); } return; } template void 
MWCNNLayerImpl::deallocateOutputImpl<float>(int); template void 
MWCNNLayerImpl::deallocateOutputImpl<signed char>(int); void 
MWCNNLayerImpl::clearLayerMapsAndVectors() { reorderLayerMemory.clear(); 
reorderPrim.clear(); pipeline.clear(); pipeline_memory_args.clear(); 
pipeline_weights_memory_args.clear(); pipeline_weights.clear(); 
pipeline_ipReorder_memory_args.clear(); pipeline_ipReorder.clear(); } void 
MWCNNLayerImpl::executeWeightsReorderPrimitive() { if 
(!pipeline_weights.empty()) { auto s = dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); 
assert(pipeline_weights.size() == pipeline_weights_memory_args.size()); for 
(int i = 0; i < static_cast<int>(pipeline_weights.size()); i++) { 
pipeline_weights[i].execute(s, pipeline_weights_memory_args[i]); } } } void 
MWCNNLayerImpl::executeInputsReorderPrimitive() { if 
(!pipeline_ipReorder.empty()) { auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline_ipReorder.size() == 
pipeline_ipReorder_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline_ipReorder.size()); i++) { 
pipeline_ipReorder[i].execute(s, pipeline_ipReorder_memory_args[i]); } } }
#if MW_LAYERS_TAP
 int tap_count = 0; void mw_interm_tap(float* inp, int size, int count) { FILE* 
fp; int i; std::string fileName{"taps/mw_interm_tap_"};
#define TXT_FILE 1
#if TXT_FILE
 fileName = fileName + std::to_string(count) + ".txt"; fp = 
fopen(fileName.c_str(), "w"); if (fp == NULL) { std::string errmsg = 
std::string("Error opening text file .Create taps folder to open") + fileName; 
printf("Error! Unable to open file %s\n", fileName); throw 
std::runtime_error(errmsg.c_str()); } for (i = 0; i < size; i++) { fprintf(fp, 
"%f\n", inp[i]); }
#else
 fileName = fileName + std::to_string(count) + ".bin"; fp = 
fopen(fileName.c_str(), "wb"); if (fp == NULL) { std::string errmsg = 
std::string("Error opening binary file .Create taps folder to open") + 
fileName; printf("Error! Unable to open file %s\n", fileName); throw 
std::runtime_error(errmsg.c_str()); } fwrite(inp, 4, size, fp);
#endif
 fclose(fp); } void MWCNNLayerImpl::reorderedLayerOutputTap(int portIndex) { 
MWTensorBase* opTensorBase = getLayer()->getOutputTensor(portIndex); 
MWTensor<float>* opTensor = static_cast<MWTensor<float>*>(opTensorBase); 
MWCNNLayerImpl* layerImpl = 
static_cast<MWCNNLayerImpl*>(atVCyzqXZAZxwlkRLBRA->getImpl()); int layerOutputSize 
= opTensor->getBatchSize() * opTensor->getSequenceLength() * 
opTensor->getChannels() * opTensor->getHeight() * opTensor->getWidth(); float* 
layerData = (float*)calloc(layerOutputSize, sizeof(float)); 
std::shared_ptr<dnnl::memory> currentLayerMemory = 
MWOnednnUtils::getLayerMemoryPrimitive(opTensor); bool isMemPrimitiveNC = 
MWOnednnUtils::checkformatType(currentLayerMemory->get_desc(), opTensor, 
dnnl::memory::format_tag::nc); bool isMemPrimitiveNCHW = 
MWOnednnUtils::checkformatType( currentLayerMemory->get_desc(), opTensor, 
dnnl::memory::format_tag::nchw); bool isMemPrimitiveTNC = 
MWOnednnUtils::checkformatType( currentLayerMemory->get_desc(), opTensor, 
dnnl::memory::format_tag::tnc); auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); if (!(isMemPrimitiveNC || 
isMemPrimitiveNCHW || isMemPrimitiveTNC)) { 
MWOnednnUtils::configureReorder(layerImpl, opTensor, 
dnnl::memory::format_tag::nchw); pipeline.clear(); 
pipeline_memory_args.clear(); pipeline_memory_args.push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(opTensor)}, {DNNL_ARG_TO, 
*getReorderedIpMemory()}}); pipeline.push_back(*layerImpl->getReorderPrim()); 
assert(pipeline.size() == pipeline_memory_args.size()); pipeline[0].execute(s, 
pipeline_memory_args[0]); memcpy(layerData, 
(float*)layerImpl->getReorderedIpMemory()->get_data_handle(), layerOutputSize * 
sizeof(float)); } else { memcpy(layerData, opTensor->getData(), layerOutputSize 
* sizeof(float)); } mw_interm_tap(layerData, layerOutputSize, tap_count++); if 
(layerData) { free(layerData); layerData = NULL; } }
#endif
 } 