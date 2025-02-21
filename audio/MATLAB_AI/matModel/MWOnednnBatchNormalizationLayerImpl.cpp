#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnBatchNormalizationLayerImpl.hpp"
#include <cstdio>
#include <cassert>
#include <vector>
 class MWTensorBase; namespace MWOnednnTarget {
#if MW_NORM_TAP
 extern int tap_count; extern void mw_interm_tap(float* inp, int size, int count);
#endif
 MWBatchNormalizationLayerImpl::~MWBatchNormalizationLayerImpl() { } void 
MWBatchNormalizationLayerImpl::loadWeights(const char* RqCYCrGsNvzKYrRMXbsI, int 
channels, float* OwenhowBxTAXHXmJpIKd) { FILE* SDWKEQTZaTFZByPlzUDR = 
MWCNNLayer::openBinaryFile(RqCYCrGsNvzKYrRMXbsI); assert(SDWKEQTZaTFZByPlzUDR); 
MWCNNLayer::call_fread(OwenhowBxTAXHXmJpIKd, sizeof(float), channels, 
SDWKEQTZaTFZByPlzUDR, RqCYCrGsNvzKYrRMXbsI); fclose(SDWKEQTZaTFZByPlzUDR); } void 
MWBatchNormalizationLayerImpl::setLearnables(std::vector<float*> learnables) { 
assert(learnables.size() == 2); float* hDaNSVZAofAENeIAiWEw = learnables[0]; 
memcpy(puSFZkRJmyuFPfQRswDK + enPbWLzEmxYCBmzGJutZ, hDaNSVZAofAENeIAiWEw, 
sizeof(float) * enPbWLzEmxYCBmzGJutZ); float* pzUAoBDvaKAtdsmkQuct = 
learnables[1]; memcpy(puSFZkRJmyuFPfQRswDK, pzUAoBDvaKAtdsmkQuct, sizeof(float) * 
enPbWLzEmxYCBmzGJutZ); } 
MWBatchNormalizationLayerImpl::MWBatchNormalizationLayerImpl( MWCNNLayer* 
layer, MWTargetNetworkImpl* ntwk_impl, float JwxFdqOKggeawILBfGgg, const 
char* MCrRCXUsCsGPMgQbvMOt, const char* MEmIeGILUZNEWEagSzRk, const char* 
MNuwXDSoGEYeABeVTwOh, const char* MUmglsoWcEiRiAZsclur, int 
numChannels) : MWCNNLayerImpl{layer, ntwk_impl} , 
enPbWLzEmxYCBmzGJutZ{numChannels} , 
REXdEoRjxuQJkqgIDihy{JwxFdqOKggeawILBfGgg} { puSFZkRJmyuFPfQRswDK = 
(float*)calloc(2 * enPbWLzEmxYCBmzGJutZ, sizeof(float)); 
uOjfVTZSbCZATdZVDwrL = (float*)calloc(enPbWLzEmxYCBmzGJutZ, sizeof(float)); 
ugnnrhsgTeWucrMPCJUc = (float*)calloc(enPbWLzEmxYCBmzGJutZ, 
sizeof(float)); loadWeights(MEmIeGILUZNEWEagSzRk, enPbWLzEmxYCBmzGJutZ, 
puSFZkRJmyuFPfQRswDK); loadWeights(MCrRCXUsCsGPMgQbvMOt, enPbWLzEmxYCBmzGJutZ, 
puSFZkRJmyuFPfQRswDK + enPbWLzEmxYCBmzGJutZ); 
loadWeights(MNuwXDSoGEYeABeVTwOh, enPbWLzEmxYCBmzGJutZ, 
uOjfVTZSbCZATdZVDwrL); loadWeights(MUmglsoWcEiRiAZsclur, 
enPbWLzEmxYCBmzGJutZ, ugnnrhsgTeWucrMPCJUc); auto weights_md = 
dnnl::memory::desc( {{2, enPbWLzEmxYCBmzGJutZ}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nc}); auto meanVariance_md = dnnl::memory::desc( 
{{enPbWLzEmxYCBmzGJutZ}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::x}); c_weights = std::unique_ptr<dnnl::memory>( new 
dnnl::memory(weights_md, *eFaDPmxDdzHlRYSAoMmX->eng, puSFZkRJmyuFPfQRswDK)); c_mean 
= std::unique_ptr<dnnl::memory>( new dnnl::memory(meanVariance_md, 
*eFaDPmxDdzHlRYSAoMmX->eng, uOjfVTZSbCZATdZVDwrL)); c_variance = 
std::unique_ptr<dnnl::memory>( new dnnl::memory(meanVariance_md, 
*eFaDPmxDdzHlRYSAoMmX->eng, ugnnrhsgTeWucrMPCJUc)); } void 
MWBatchNormalizationLayerImpl::propagateSize() { clearLayerMapsAndVectors(); 
MWOnednnUtils::reorderInputsTo(this); 
setLayerMemory(std::make_shared<dnnl::memory>(getReorderedIpMemory()->get_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng)); bnrm_desc = 
std::unique_ptr<dnnl::batch_normalization_forward::desc>( new 
dnnl::batch_normalization_forward::desc( dnnl::prop_kind::forward_inference, 
getReorderedIpMemory()->get_desc(), REXdEoRjxuQJkqgIDihy, 
dnnl::normalization_flags::use_scale_shift | 
dnnl::normalization_flags::use_global_stats)); bnrm_prim_desc = 
std::unique_ptr<dnnl::batch_normalization_forward::primitive_desc>( new 
dnnl::batch_normalization_forward::primitive_desc(*bnrm_desc, 
*eFaDPmxDdzHlRYSAoMmX->eng)); bn = 
std::unique_ptr<dnnl::batch_normalization_forward::primitive>( new 
dnnl::batch_normalization_forward(*bnrm_prim_desc)); pipeline.push_back(*bn); 
pipeline_memory_args.push_back({{DNNL_ARG_FROM, *getReorderedIpMemory()}, 
{DNNL_ARG_TO, *getLayerMemory()}, {DNNL_ARG_MEAN, *c_mean}, {DNNL_ARG_VARIANCE, 
*c_variance}, {DNNL_ARG_WEIGHTS, *c_weights}}); return; } void 
MWBatchNormalizationLayerImpl::predict() { executeInputsReorderPrimitive(); 
auto s = dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_NORM_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } void MWBatchNormalizationLayerImpl::cleanup() { if 
(puSFZkRJmyuFPfQRswDK) { free(puSFZkRJmyuFPfQRswDK); puSFZkRJmyuFPfQRswDK = NULL; } if 
(uOjfVTZSbCZATdZVDwrL) { free(uOjfVTZSbCZATdZVDwrL); uOjfVTZSbCZATdZVDwrL 
= NULL; } if (ugnnrhsgTeWucrMPCJUc) { free(ugnnrhsgTeWucrMPCJUc); 
ugnnrhsgTeWucrMPCJUc = NULL; } return; } } 