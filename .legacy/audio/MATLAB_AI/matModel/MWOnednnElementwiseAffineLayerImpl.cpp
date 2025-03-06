#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnElementwiseAffineLayerImpl.hpp"
#include <memory>
#include <cmath>
#include <cassert>
#include <cstdio>
#include <algorithm>
 namespace MWOnednnTarget { 
MWElementwiseAffineLayerImpl::MWElementwiseAffineLayerImpl(MWCNNLayer* layer, 
MWTargetNetworkImpl* ntwk_impl, int scale_H, int scale_W, int scale_C, int 
offset_H, int offset_W, int offset_C, bool isClipped, int lowerbound, int 
upperbound, const char* scale_file, const char* offset_file) : 
MWCNNLayerImpl(layer, ntwk_impl) , intermediateOp(NULL) , 
rSmEWccbJFfPGddhPemm(scale_H) , shEncNmxJsMuJKwbrwok(scale_W) , 
qWwjVYwfnvEnFKlgpqwA(scale_C) , hnewnpwgzKmOdualajhn(offset_H) , 
jLyhrFjMmVnNjoeDJCwH(offset_W) , hljcfGWsvZXJZNrImpJB(offset_C) , 
ZinudJuZuGitiNTsJpBR(isClipped) , dJcdBfQQLhIAYHPxwQeg(lowerbound) , 
vFNECEAeLZsYsUxvlgqL(upperbound) { long int sRECVoNNtDdcBOWgDyar = (long 
int)(rSmEWccbJFfPGddhPemm * shEncNmxJsMuJKwbrwok * qWwjVYwfnvEnFKlgpqwA); long int 
iPqBiFnIJMxelVhQBZex = (long int)(hnewnpwgzKmOdualajhn * jLyhrFjMmVnNjoeDJCwH * 
hljcfGWsvZXJZNrImpJB); puSFZkRJmyuFPfQRswDK = (float*)calloc(sRECVoNNtDdcBOWgDyar, 
sizeof(float)); gzSTokDHvkXefhiGDcWL = (float*)calloc(iPqBiFnIJMxelVhQBZex, 
sizeof(float)); loadScale(scale_file); loadOffset(offset_file); } 
MWElementwiseAffineLayerImpl::~MWElementwiseAffineLayerImpl() { } void 
MWElementwiseAffineLayerImpl::createScalarAffine() { long int 
sRECVoNNtDdcBOWgDyar = (long int)(rSmEWccbJFfPGddhPemm * shEncNmxJsMuJKwbrwok * 
qWwjVYwfnvEnFKlgpqwA); long int iPqBiFnIJMxelVhQBZex = (long 
int)(hnewnpwgzKmOdualajhn * jLyhrFjMmVnNjoeDJCwH * hljcfGWsvZXJZNrImpJB); 
std::shared_ptr<dnnl::memory> srcLayerMemory = getReorderedIpMemory(); 
std::shared_ptr<dnnl::memory> dstLayerMemory = getLayerMemory(); float 
temp_offset = *gzSTokDHvkXefhiGDcWL; float temp_scale = *puSFZkRJmyuFPfQRswDK; if 
(sRECVoNNtDdcBOWgDyar == 1 && iPqBiFnIJMxelVhQBZex != 1) { dstLayerMemory = 
intermediateOutput; temp_offset = 0.0f; } else if ((sRECVoNNtDdcBOWgDyar != 1 && 
iPqBiFnIJMxelVhQBZex == 1)) { srcLayerMemory = intermediateOutput; temp_scale = 
1.0f; } if ((sRECVoNNtDdcBOWgDyar == 1 && iPqBiFnIJMxelVhQBZex == 1) || 
(sRECVoNNtDdcBOWgDyar == 1 && iPqBiFnIJMxelVhQBZex != 1) || (sRECVoNNtDdcBOWgDyar != 
1 && iPqBiFnIJMxelVhQBZex == 1)) { scaling_d = 
std::unique_ptr<dnnl::eltwise_forward::desc>(new dnnl::eltwise_forward::desc( 
dnnl::prop_kind::forward_inference, dnnl::algorithm::eltwise_linear, 
getReorderedIpMemory()->get_desc(), temp_scale, temp_offset)); scaling_pd = 
std::unique_ptr<dnnl::eltwise_forward::primitive_desc>( new 
dnnl::eltwise_forward::primitive_desc(*scaling_d, *eFaDPmxDdzHlRYSAoMmX->eng)); 
scaling = std::unique_ptr<dnnl::eltwise_forward>(new 
dnnl::eltwise_forward(*scaling_pd)); pipeline_memory_args.push_back( 
{{DNNL_ARG_FROM, *srcLayerMemory}, {DNNL_ARG_TO, *dstLayerMemory}}); 
pipeline.push_back(*scaling); } } void 
MWElementwiseAffineLayerImpl::createVectorAffine() { long int 
sRECVoNNtDdcBOWgDyar = (long int)(rSmEWccbJFfPGddhPemm * shEncNmxJsMuJKwbrwok * 
qWwjVYwfnvEnFKlgpqwA); long int iPqBiFnIJMxelVhQBZex = (long 
int)(hnewnpwgzKmOdualajhn * jLyhrFjMmVnNjoeDJCwH * hljcfGWsvZXJZNrImpJB); bool 
offsetVectorImpl = (hnewnpwgzKmOdualajhn == 1 && jLyhrFjMmVnNjoeDJCwH == 1 && 
iPqBiFnIJMxelVhQBZex > 1); bool scaleVectorImpl = (rSmEWccbJFfPGddhPemm == 1 && 
shEncNmxJsMuJKwbrwok == 1 && sRECVoNNtDdcBOWgDyar > 1); 
std::shared_ptr<dnnl::memory> srcLayerMemory1 = getReorderedIpMemory(); 
std::shared_ptr<dnnl::memory> dstLayerMemory1 = getLayerMemory(); if 
(scaleVectorImpl && !offsetVectorImpl) { dstLayerMemory1 = intermediateOutput; 
} else if (!scaleVectorImpl && offsetVectorImpl) { srcLayerMemory1 = 
intermediateOutput; } } void MWElementwiseAffineLayerImpl::propagateSize() { 
clearLayerMapsAndVectors(); MWTensorBase* ipTensor = 
getLayer()->getInputTensor(0); int pckLLTEdVPoCZLRwyDnM = 
ipTensor->getSequenceLength(); int dMxIKDGTITyhdLqIHBLA = ipTensor->getBatchSize(); int 
PfisSEEWDaQFynnzlcin = ipTensor->getChannels(); int SIBpKtDURUWQaaenbwrC = 
ipTensor->getHeight(); int vIWQzNvYZSuxmOTVDFhU = ipTensor->getWidth(); auto 
prevMemoryDesc = MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc(); 
format_type formatTag = MWOnednnUtils::getformatType(prevMemoryDesc, ipTensor); 
bool isSequenecIp = (formatTag == format_type::TNC_FORMAT); format_type 
reorderIpTo = isSequenecIp ? (format_type::TNC_FORMAT) : 
(format_type::NCHW_FORMAT); MWOnednnUtils::reorderInputsTo(this, reorderIpTo); 
intermediateOp = std::shared_ptr<float>( new float[pckLLTEdVPoCZLRwyDnM * dMxIKDGTITyhdLqIHBLA 
* PfisSEEWDaQFynnzlcin * SIBpKtDURUWQaaenbwrC * vIWQzNvYZSuxmOTVDFhU], std::default_delete<float[]>()); 
intermediateOutput = std::make_shared<dnnl::memory>( 
getReorderedIpMemory()->get_desc(), *eFaDPmxDdzHlRYSAoMmX->eng, 
intermediateOp.get()); 
setLayerMemory(std::make_shared<dnnl::memory>(getReorderedIpMemory()->get_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng)); createScalarAffine(); createVectorAffine(); } void 
MWElementwiseAffineLayerImpl::predict() { executeInputsReorderPrimitive(); long 
int sRECVoNNtDdcBOWgDyar = (long int)(rSmEWccbJFfPGddhPemm * shEncNmxJsMuJKwbrwok * 
qWwjVYwfnvEnFKlgpqwA); long int iPqBiFnIJMxelVhQBZex = (long 
int)(hnewnpwgzKmOdualajhn * jLyhrFjMmVnNjoeDJCwH * hljcfGWsvZXJZNrImpJB); 
MWTensorBase* opTensorBase = getLayer()->getOutputTensor(0); MWTensor<float>* 
opTensor = static_cast<MWTensor<float>*>(opTensorBase); if (sRECVoNNtDdcBOWgDyar 
== 1 && iPqBiFnIJMxelVhQBZex == 1) { auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); } clipAffineOutput(); return; } MWTensorBase* 
ipTensor = getLayer()->getInputTensor(0); float* ipData = 
(float*)getReorderedIpMemory()->get_data_handle(); float* opData = 
intermediateOp.get(); long int XYbzSmRQGatVJtGmDZSo = 
ipTensor->getHeight() * ipTensor->getWidth(); long int 
XVcMnvCXvZpKICKIjgZi = XYbzSmRQGatVJtGmDZSo * 
ipTensor->getChannels(); if (sRECVoNNtDdcBOWgDyar == 1) { auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); } } else if (rSmEWccbJFfPGddhPemm == 1 && 
shEncNmxJsMuJKwbrwok == 1 && sRECVoNNtDdcBOWgDyar > 1) { for (int n = 0; n < 
ipTensor->getBatchSize() * ipTensor->getSequenceLength(); n++) { for (int c = 
0; c < ipTensor->getChannels(); c++) { for (int wh = 0; wh < 
XYbzSmRQGatVJtGmDZSo; wh++) { *opData++ = *ipData++ * 
puSFZkRJmyuFPfQRswDK[c]; } } } } else if (XYbzSmRQGatVJtGmDZSo == 
sRECVoNNtDdcBOWgDyar) { for (int n = 0; n < ipTensor->getBatchSize() * 
ipTensor->getSequenceLength(); n++) { for (int c = 0; c < 
ipTensor->getChannels(); c++) { for (int i = 0; i < 
XYbzSmRQGatVJtGmDZSo; ++i) { opData[i + c * 
XYbzSmRQGatVJtGmDZSo] = ipData[i + c * 
XYbzSmRQGatVJtGmDZSo] * puSFZkRJmyuFPfQRswDK[i]; } } ipData += 
XVcMnvCXvZpKICKIjgZi; opData += XVcMnvCXvZpKICKIjgZi; } } else 
{ for (int n = 0; n < ipTensor->getBatchSize() * ipTensor->getSequenceLength(); 
n++) { for (int i = 0; i < XVcMnvCXvZpKICKIjgZi; ++i) { opData[i] = 
ipData[i] * puSFZkRJmyuFPfQRswDK[i]; } ipData += XVcMnvCXvZpKICKIjgZi; 
opData += XVcMnvCXvZpKICKIjgZi; } } ipData = intermediateOp.get(); 
opData = opTensor->getData(); if (iPqBiFnIJMxelVhQBZex == 1) { auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); } } else if (hnewnpwgzKmOdualajhn == 1 && 
jLyhrFjMmVnNjoeDJCwH == 1 && iPqBiFnIJMxelVhQBZex > 1) { for (int n = 0; n < 
ipTensor->getBatchSize() * ipTensor->getSequenceLength(); n++) { for (int c = 
0; c < ipTensor->getChannels(); c++) { for (int wh = 0; wh < 
XYbzSmRQGatVJtGmDZSo; wh++) { *opData++ = *ipData++ + 
gzSTokDHvkXefhiGDcWL[c]; } } } } else if (XYbzSmRQGatVJtGmDZSo == 
iPqBiFnIJMxelVhQBZex) { for (int n = 0; n < ipTensor->getBatchSize() * 
ipTensor->getSequenceLength(); n++) { for (int c = 0; c < 
ipTensor->getChannels(); c++) { for (int i = 0; i < 
XYbzSmRQGatVJtGmDZSo; ++i) { opData[i + c * 
XYbzSmRQGatVJtGmDZSo] = ipData[i + c * 
XYbzSmRQGatVJtGmDZSo] + gzSTokDHvkXefhiGDcWL[i]; } } ipData += 
XVcMnvCXvZpKICKIjgZi; opData += XVcMnvCXvZpKICKIjgZi; } } else 
{ for (int n = 0; n < ipTensor->getBatchSize() * ipTensor->getSequenceLength(); 
n++) { for (int i = 0; i < XVcMnvCXvZpKICKIjgZi; ++i) { opData[i] = 
ipData[i] + gzSTokDHvkXefhiGDcWL[i]; } ipData += XVcMnvCXvZpKICKIjgZi; 
opData += XVcMnvCXvZpKICKIjgZi; } } clipAffineOutput();
#if MW_AFFINE_TAP
 mw_interm_tap(opData, XVcMnvCXvZpKICKIjgZi, tap_count++);
#endif
 return; } void MWElementwiseAffineLayerImpl::clipAffineOutput() { 
MWTensorBase* opTensorBase = getLayer()->getOutputTensor(0); MWTensor<float>* 
opTensor = static_cast<MWTensor<float>*>(opTensorBase); float* outputBuffer = 
opTensor->getData(); if (!ZinudJuZuGitiNTsJpBR) { return; } for (int n = 0; n < 
opTensor->getNumElements(); n++) { outputBuffer[n] = outputBuffer[n] > 
vFNECEAeLZsYsUxvlgqL ? vFNECEAeLZsYsUxvlgqL : outputBuffer[n]; 
outputBuffer[n] = outputBuffer[n] < dJcdBfQQLhIAYHPxwQeg ? 
dJcdBfQQLhIAYHPxwQeg : outputBuffer[n]; } } void 
MWElementwiseAffineLayerImpl::cleanup() { if (puSFZkRJmyuFPfQRswDK) { 
free(puSFZkRJmyuFPfQRswDK); puSFZkRJmyuFPfQRswDK = NULL; } if (gzSTokDHvkXefhiGDcWL) { 
free(gzSTokDHvkXefhiGDcWL); gzSTokDHvkXefhiGDcWL = NULL; } } void 
MWElementwiseAffineLayerImpl::loadScale(const char* sxuOMwKXOKfuExclRaSe) { 
FILE* SDWKEQTZaTFZByPlzUDR = MWCNNLayer::openBinaryFile(sxuOMwKXOKfuExclRaSe); 
assert(SDWKEQTZaTFZByPlzUDR); long int eVAFqeShtGZAZluKdMvQ = (long int)(rSmEWccbJFfPGddhPemm 
* shEncNmxJsMuJKwbrwok * qWwjVYwfnvEnFKlgpqwA); 
MWCNNLayer::call_fread(puSFZkRJmyuFPfQRswDK, sizeof(float), eVAFqeShtGZAZluKdMvQ, 
SDWKEQTZaTFZByPlzUDR, sxuOMwKXOKfuExclRaSe); fclose(SDWKEQTZaTFZByPlzUDR); } void 
MWElementwiseAffineLayerImpl::loadOffset(const char* jaqKGCwoANNDMHgAsehk) { 
FILE* SDWKEQTZaTFZByPlzUDR = MWCNNLayer::openBinaryFile(jaqKGCwoANNDMHgAsehk); 
assert(SDWKEQTZaTFZByPlzUDR); long int eVAFqeShtGZAZluKdMvQ = (long int)(hnewnpwgzKmOdualajhn 
* jLyhrFjMmVnNjoeDJCwH * hljcfGWsvZXJZNrImpJB); 
MWCNNLayer::call_fread(gzSTokDHvkXefhiGDcWL, sizeof(float), eVAFqeShtGZAZluKdMvQ, 
SDWKEQTZaTFZByPlzUDR, jaqKGCwoANNDMHgAsehk); fclose(SDWKEQTZaTFZByPlzUDR); } } 